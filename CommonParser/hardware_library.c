/*** Associated config file : prr.conf ***/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "confuse.h"
#include "common_interfaces.h"

/******************************************************************************
 *****************              HARDWARE FILE I/O             *****************
 *****************************************************************************/

// FIX - Needs a function header comment
cfg_t * parse_conf_hardware(char *filename){
    cfg_opt_t processor_opts[] = {
        CFG_INT("PRRno", 0, CFGF_NODEFAULT),
        CFG_INT("GPPno",0 , CFGF_NODEFAULT),
        CFG_INT_LIST("PRRConfigTime", 0, CFGF_NODEFAULT),
        CFG_END()
    };

    cfg_opt_t opts[] = {
        CFG_SEC("processors", processor_opts, CFGF_MULTI | CFGF_TITLE | CFGF_NODEFAULT | CFGF_NO_TITLE_DUPES),
        CFG_STR("Name", "defaultName", CFGF_NODEFAULT),
        CFG_STR("Date", "defaultName", CFGF_NODEFAULT),
        CFG_END()
    };

    cfg_t *cfg = cfg_init(opts, CFGF_NODEFAULT);
    // cfg_set_validate_func(cfg, "processors", conf_validate_processor);
        // FIX - validate that the length of the PRRConfigTime array is the same as the # of PRRs

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


int initHardwareLibrary(char * filename, Hardware * hardware_library){
    cfg_t * cfg_hardware, * processor;
    int i, j;
    
    if(filename == NULL)
        return EXIT_FAILURE;
    
    cfg_hardware = parse_conf_hardware(filename);
    if(cfg_hardware == NULL)
        return EXIT_FAILURE;       // FAILED TO PARSE THE FILE!
    
    strncpy(hardware_library->name, cfg_getstr(cfg_hardware, "Name"), DEFAULT_BUFF_SIZE);
    strncpy(hardware_library->date, cfg_getstr(cfg_hardware, "Date"), DEFAULT_BUFF_SIZE);
    
    // Read in all the processors
    hardware_library->num_setups = cfg_size(cfg_hardware, "processors");
    hardware_library->setups = malloc(sizeof(Processing_Elelements) * hardware_library->num_setups);

    for(i=0; i < cfg_size(cfg_hardware, "processors"); i++){
        processor = cfg_getnsec(cfg_hardware, "processors", i);
        
        strncpy(hardware_library->setups[i].name, cfg_title(processor), DEFAULT_BUFF_SIZE);
        hardware_library->setups[i].num_gpps = cfg_getint(processor, "GPPno");
        hardware_library->setups[i].num_prrs = cfg_getint(processor, "PRRno");
        hardware_library->setups[i].prr_config_time = malloc(sizeof(int) * cfg_size(processor, "PRRConfigTime"));
        
        for(j=0; j < cfg_size(processor, "PRRConfigTime"); j++)
            hardware_library->setups[i].prr_config_time[j] = cfg_getnint(processor, "PRRConfigTime", j);
    }
    
    cfg_free(cfg_hardware);
    return EXIT_SUCCESS;
}


void freeHardwareLibrary(Hardware * hardware_library){
    int i;

    for(i = 0; i < hardware_library->num_setups; i++)
        free(hardware_library->setups[i].prr_config_time);
    free(hardware_library->setups);
}



void printHardwareLibrary(Hardware * hardware_library){
    int i, j;
    
    fprintf(stdout, "Name = %s\n", hardware_library->name);
    fprintf(stdout, "Date = %s\n", hardware_library->date);
    
    fprintf(stdout, "\nHardware Configurations:\n");
    
    for(i = 0; i < hardware_library->num_setups; i++){
        fprintf(stdout, "\tName = %s\n", hardware_library->setups[i].name);
        fprintf(stdout, "\tGPPs = %d\n", hardware_library->setups[i].num_gpps);
        fprintf(stdout, "\tPRRs = %d\n", hardware_library->setups[i].num_prrs);
        
        fprintf(stdout, "\tPRR config times = { ");
        for(j=0; j < hardware_library->setups[i].num_prrs; j++)
            fprintf(stdout, "%d, ", hardware_library->setups[i].prr_config_time[j]);
        fprintf(stdout, "}\n\n");
    }
}
