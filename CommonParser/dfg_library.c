/*** Associated config file : dfg.conf ***/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "confuse.h"
#include "common_interfaces.h"

/******************************************************************************
 *****************                DFG FILE I/O                *****************
 *****************************************************************************/

// FIX - Needs a function header comment
cfg_t * parse_conf_dfg(char *filename){
    cfg_opt_t task_opts[] = {
        CFG_INT("type", 0, CFGF_NODEFAULT ),
        CFG_STR("output",0 , CFGF_NODEFAULT),
        CFG_STR_LIST("inputs", 0, CFGF_NODEFAULT),
        CFG_END()
    };

    cfg_opt_t opts[] = {
        CFG_SEC("task", task_opts, CFGF_MULTI | CFGF_TITLE | CFGF_NODEFAULT | CFGF_NO_TITLE_DUPES),
        CFG_STR("Name", "Unknown DFG file!", CFGF_NONE),
        CFG_STR("Date", "Unknown Creation Date", CFGF_NONE),
        
        CFG_STR_LIST("inputs", 0, CFGF_NODEFAULT),
        CFG_STR_LIST("outputs", 0, CFGF_NODEFAULT),
        CFG_STR_LIST("regs", 0, CFGF_NODEFAULT),
        CFG_END()
    };

    cfg_t * cfg = cfg_init(opts, CFGF_NODEFAULT);
    // FIX - I have no idea what to validate in the DFG files!
        // FIX - validate that each node's input and ouput is one of the options listed at the top
        // FIX - validate that each node has exactly two inputs and only one output?
        // FIX - validate that each node's output is another node's input etc. 
    // cfg_set_validate_func(cfg, "inputs", conf_validate_processor);

    switch (cfg_parse(cfg, filename)) {
        case CFG_SUCCESS:
            break;
        
        case CFG_FILE_ERROR:
            // FIX - Make this error message better (or terminate / return -1 if the file could not be correctly read)
            printf("warning: configuration file '%s' could not be read: %s\n", 
                    filename, strerror(errno));
            break;
            
        case CFG_PARSE_ERROR:
            return NULL;
    }

    return cfg;
}


int initDFG(char * filename, DFG * task_schedule){
    cfg_t * cfgDFG, * node;
    int i, j;
    
    if(filename == NULL)
        return EXIT_FAILURE;
    
    cfgDFG = parse_conf_dfg(filename);
    if(cfgDFG == NULL)
        return EXIT_FAILURE;       // FAILED TO PARSE THE FILE!

    // FIX - Change the strncpy size to a #define static thing
    strncpy(task_schedule->name, cfg_getstr(cfgDFG, "Name"), DEFAULT_BUFF_SIZE);
    strncpy(task_schedule->date, cfg_getstr(cfgDFG, "Date"), DEFAULT_BUFF_SIZE);
    
    // read in all the inputs
    task_schedule->inputs = malloc(sizeof(char *) * (cfg_size(cfgDFG, "inputs") + 1));
    for(i=0; i < cfg_size(cfgDFG, "inputs"); i++){
        task_schedule->inputs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(cfgDFG, "inputs", i)) + 1));
        strcpy(task_schedule->inputs[i], cfg_getnstr(cfgDFG, "inputs", i));
    }
    task_schedule->inputs[i] = NULL;
    
    // read in all the outputs
    task_schedule->outputs = malloc(sizeof(char *) * (cfg_size(cfgDFG, "outputs") + 1));
    for(i=0; i < cfg_size(cfgDFG, "outputs"); i++){
        task_schedule->outputs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(cfgDFG, "outputs", i)) + 1));
        strcpy(task_schedule->outputs[i], cfg_getnstr(cfgDFG, "outputs", i));
    }
    task_schedule->outputs[i] = NULL;
        
    // read in all the registers
    task_schedule->regs = malloc(sizeof(char *) * (cfg_size(cfgDFG, "regs") + 1));
    for(i=0; i < cfg_size(cfgDFG, "regs"); i++){
        task_schedule->regs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(cfgDFG, "regs", i)) + 1));
        strcpy(task_schedule->regs[i], cfg_getnstr(cfgDFG, "regs", i));
    }
    task_schedule->num_regs = i;
    task_schedule->regs[i] = NULL;
    
    // Read in all the nodes
    task_schedule->num_nodes = cfg_size(cfgDFG, "task");
    task_schedule->node = malloc(sizeof(Node) * task_schedule->num_nodes);
    for(j=0; j < cfg_size(cfgDFG, "task"); j++){
        node = cfg_getnsec(cfgDFG, "task", j);
        
        strncpy(task_schedule->node[j].name, cfg_title(node), DEFAULT_BUFF_SIZE);
        task_schedule->node[j].task_type = cfg_getint(node, "type");
        strncpy(task_schedule->node[j].output, cfg_getstr(node, "output"), DEFAULT_BUFF_SIZE);
        
        // read in all the inputs
        task_schedule->node[j].inputs = malloc(sizeof(char *) * (cfg_size(node, "inputs") + 1));
        for(i=0; i < cfg_size(node, "inputs"); i++){
            task_schedule->node[j].inputs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(node, "inputs", i)) + 1));
            strcpy(task_schedule->node[j].inputs[i], cfg_getnstr(node, "inputs", i));
        }
        task_schedule->node[j].inputs[i] = NULL;
    }
    
    cfg_free(cfgDFG);
    return EXIT_SUCCESS;
}


void freeDFG(DFG * task_schedule){
    int i, j;

    for(i = 0; i < task_schedule->num_nodes; i++){
        for(j=0; task_schedule->node[i].inputs[j]!= NULL; j++)
            free(task_schedule->node[i].inputs[j]);
        
        free(task_schedule->node[i].inputs);
    }
        
    free(task_schedule->node);
    
    for(i=0; task_schedule->inputs[i] != NULL; i++)
        free(task_schedule->inputs[i]);
    free(task_schedule->inputs);
    
    for(i=0; task_schedule->outputs[i] != NULL; i++)
        free(task_schedule->outputs[i]);
    free(task_schedule->outputs);
    
    for(i=0; task_schedule->regs[i] != NULL; i++)
        free(task_schedule->regs[i]);
    free(task_schedule->regs);
}



void printDFG(DFG * task_schedule){
    int i, j;
    
    fprintf(stdout, "Name = %s\n", task_schedule->name);
    fprintf(stdout, "Date = %s\n\n", task_schedule->date);

    fprintf(stdout, "Inputs = { ");
    for(i=0; task_schedule->inputs[i] != NULL; i++)
        fprintf(stdout, "%s, ", task_schedule->inputs[i]);
    fprintf(stdout, "}\n");
    
    fprintf(stdout, "Outputs = { ");
    for(i=0; task_schedule->outputs[i] != NULL; i++)
        fprintf(stdout, "%s, ", task_schedule->outputs[i]);
    fprintf(stdout, "}\n");
    
    fprintf(stdout, "regs = { ");
    for(i=0; task_schedule->regs[i] != NULL; i++)
        fprintf(stdout, "%s, ", task_schedule->regs[i]);
    fprintf(stdout, "}\n");
    fprintf(stdout, "There are %d regs", task_schedule->num_regs);
    
    fprintf(stdout, "\nNODES:\n");
    for(i = 0; i < task_schedule->num_nodes; i++){
        fprintf(stdout, "\tName = %s\n", task_schedule->node[i].name);
        
        fprintf(stdout, "\tInputs = { ");
        for(j=0; task_schedule->node[i].inputs[j]!= NULL; j++)
            fprintf(stdout, "%s, ", task_schedule->node[i].inputs[j]);
        fprintf(stdout, "}\n");
        
        fprintf(stdout, "\tOutput = %s\n", task_schedule->node[i].output);
        fprintf(stdout, "\tType = %d\n\n", task_schedule->node[i].task_type);
    }
}
