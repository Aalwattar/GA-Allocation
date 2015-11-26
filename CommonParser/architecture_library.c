/*** Associated config file : arch.conf ***/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "confuse.h"
#include "common_interfaces.h"

/******************************************************************************
 *****************            ARCHITECTURE FILE I/O           *****************
 *****************************************************************************/

// FIX - Needs a function header comment
int conf_validate_value(cfg_t *cfg, cfg_opt_t *opt){
    int value = cfg_opt_getnint(opt, 0);
    
    if (value <= 0) {
        cfg_error(cfg, "Integer option[%d] '%s' must be positive in section '%s'",
        		value,cfg_opt_name(opt) ,cfg_name(cfg));
        return -1;
    }
    
    return 0;
}

// FIX - Needs a function header comment
cfg_t * parse_conf_arch(char *filename){
    cfg_opt_t arch_opts[] = {
        CFG_INT("exec_time", 0, CFGF_NODEFAULT ),
        CFG_INT("config_time", 0, CFGF_NODEFAULT ),
        CFG_INT("config_power", 0, CFGF_NODEFAULT ),
        CFG_INT("exec_power", 0, CFGF_NODEFAULT ),
        CFG_INT("columns", 0, CFGF_NODEFAULT ),
        CFG_INT("rows", 0, CFGF_NODEFAULT ),
        CFG_STR("mode", "HW", CFGF_NODEFAULT ), 
        CFG_END()
    };

    cfg_opt_t task_opts[] = {
        CFG_STR("name", "TASK0", CFGF_NODEFAULT ),
        CFG_INT("id", 0, CFGF_NODEFAULT ),
        CFG_SEC("arch", arch_opts, CFGF_MULTI | CFGF_TITLE | CFGF_NODEFAULT | CFGF_NO_TITLE_DUPES),
        CFG_END()
    };


    cfg_opt_t opts[] = {
        CFG_SEC("Task", task_opts, CFGF_MULTI | CFGF_TITLE | CFGF_NODEFAULT | CFGF_NO_TITLE_DUPES),
        CFG_STR("Name", "Unknown Architecture File" , CFGF_NONE),
        CFG_STR("Date", "Unknown Creation Date" , CFGF_NONE),
        CFG_END()
    };

    cfg_t * cfg = cfg_init(opts, CFGF_NODEFAULT);
    
    // FIX - make the validation more robust (check more!)
    cfg_set_validate_func(cfg, "Task|arch|exec_time", conf_validate_value);
    cfg_set_validate_func(cfg, "Task|arch|config_time", conf_validate_value);
    cfg_set_validate_func(cfg, "Task|arch|config_power", conf_validate_value);
    cfg_set_validate_func(cfg, "Task|arch|exec_power", conf_validate_value);
    cfg_set_validate_func(cfg, "Task|arch|columns", conf_validate_value);
    cfg_set_validate_func(cfg, "Task|arch|rows", conf_validate_value);

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



int initArchLibrary(char * filename, Architecture_Library * arch_library){
    cfg_t * cfgArch_file, * task, * arch;
    int i, j;
    
    if(filename == NULL)
        return EXIT_FAILURE;

    cfgArch_file = parse_conf_arch(filename);
    if(cfgArch_file == NULL)
        return EXIT_FAILURE;       // FAILED TO PARSE THE FILE!
    
    strncpy(arch_library->name, cfg_getstr(cfgArch_file, "Name"), DEFAULT_BUFF_SIZE);
    strncpy(arch_library->date, cfg_getstr(cfgArch_file, "Date"), DEFAULT_BUFF_SIZE);
    
    arch_library->num_tasks = cfg_size(cfgArch_file, "Task");
    arch_library->task = malloc(sizeof (Task_Type) * (arch_library->num_tasks));
    
    for(i = 0; i < arch_library->num_tasks; i++){
        task = cfg_getnsec(cfgArch_file, "Task", i);
        
        strncpy(arch_library->task[i].name, cfg_title(task), DEFAULT_BUFF_SIZE);
        (arch_library->task[i]).id = cfg_getint(task, "id");
        (arch_library->task[i]).num_impl = cfg_size(task, "arch");
        (arch_library->task[i]).impl = malloc(sizeof (Implementation) * (arch_library->task[i]).num_impl);
        
        for(j=0; j < (arch_library->task[i]).num_impl; j++){
            arch = cfg_getnsec(task, "arch", j);
            
            strncpy(((arch_library->task[i]).impl[j]).name, cfg_title(arch), DEFAULT_BUFF_SIZE);
            ((arch_library->task[i]).impl[j]).columns = cfg_getint(arch, "columns");
            ((arch_library->task[i]).impl[j]).rows = cfg_getint(arch, "rows");
            ((arch_library->task[i]).impl[j]).config_time = cfg_getint(arch, "config_time");
            ((arch_library->task[i]).impl[j]).exec_time = cfg_getint(arch, "exec_time");
            ((arch_library->task[i]).impl[j]).config_power = cfg_getint(arch, "config_power");
            ((arch_library->task[i]).impl[j]).exec_power = cfg_getint(arch, "exec_power");
            strcpy(((arch_library->task[i]).impl[j]).mode, cfg_getstr(arch, "mode"));
        }
    }
    
    cfg_free(cfgArch_file);
    return EXIT_SUCCESS;
}


void freeArchLibrary(Architecture_Library * arch_library){
    int i;

    for(i = 0; i < arch_library->num_tasks; i++)
        free((arch_library->task[i]).impl);

    free(arch_library->task);
}



void printArchLibrary(Architecture_Library * arch_library){
    int i, j;

    fprintf(stdout, "Name = %s\n", arch_library->name);
    fprintf(stdout, "Date = %s\n", arch_library->date);
    fprintf(stdout, "TASKS: \n\n");
    
    for(i = 0; i < arch_library->num_tasks; i++){
        fprintf(stdout, "\tname = %s\n", arch_library->task[i].name);
        fprintf(stdout, "\tid = %d\n", arch_library->task[i].id);
        fprintf(stdout, "\tARCHITECTURES: \n\n");

        for(j = 0; j < (arch_library->task[i]).num_impl; j++){
            fprintf(stdout, "\t\tname = %s\n", arch_library->task[i].impl[j].name);
            fprintf(stdout, "\t\t%d\t%d\t%d\t%d\t%d\t%d\n",
                    ((arch_library->task[i]).impl[j]).columns,
                    ((arch_library->task[i]).impl[j]).rows,
                    ((arch_library->task[i]).impl[j]).config_time,
                    ((arch_library->task[i]).impl[j]).exec_time,
                    ((arch_library->task[i]).impl[j]).config_power,
                    ((arch_library->task[i]).impl[j]).exec_power);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}
