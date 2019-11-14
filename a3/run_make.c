#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


#include "pmake.h"

extern Rule **record_rules;
extern int length_record_rules;


int getFileModifiedTime_sec(char *path)
{
    struct stat attr;
    if (stat(path, &attr) == 0)//get the attr correct
    { 
       int time_sec = attr.st_mtim.tv_sec;
       return time_sec;
    }
    exit(1);
}

int getFileModifiedTime_nsec(char *path)
{
    struct stat attr;
    if (stat(path, &attr) == 0)//get the attr correct
    { 
       int time_nsec = attr.st_mtim.tv_nsec;
       return time_nsec;
    }
    exit(1);
}


//return 1 is time_a is newer, return 0 time b is newer
int is_first_newer(char *path_a, char *path_b){
    int time_a = getFileModifiedTime_sec(path_a);
    int time_b = getFileModifiedTime_sec(path_b);

    if(time_a > time_b){
          return 1;
    }
    if(time_a < time_b){
          return 0;
    }
    else{
        time_a = getFileModifiedTime_nsec(path_a);
        time_b = getFileModifiedTime_nsec(path_b);
        if(time_a > time_b){
          return 1;
        }  
        if(time_a < time_b){
          return 0;
        }
    }

}

int execute(char** arg){
 //command line argument

return 0;
}

char *latest_target;
Rule *prev_rule;


void run_make(char *target, Rule *rules, int pflag) {
	
    //1. search the rule for the given target
    Rule* first_rule = search_rules(target);
    //the selected target is not in the list
    if(target == NULL){
       first_rule = rules;
    }
    //base Case: the given target not contains any dep
    if(first_rule->dependencies == NULL){
       latest_target = first_rule->dependencies;
      // if()
    }
    //else{
       
   // }



    return NULL;
}

