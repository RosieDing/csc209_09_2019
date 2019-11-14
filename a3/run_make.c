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
int latest_time;


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

int getFileModifiedTime(char *path_a){
  int time_a = getFileModifiedTime_sec(path_a)*1000000000 + getFileModifiedTime_nsec(path_a);
  return time_a;
}


//return 1 is time_a is newer, return 0 time b is newer
int is_first_newer(char *path_a, int time_b){
    int time_a = getFileModifiedTime(path_a);
    //int time_b = getFileModifiedTime(path_b);

    if(time_a > time_b){//first is newer
          return 1;
    }
    if(time_a < time_b){//second is newer
          return 0;
    }
    return -1;
}

int execute(char** argv){
  int ret = execvp(argv[0],argv);
  if(ret == -1){
    perror("execl error");
    exit(1);
  }
  printf("exiting main process ----\n");
  return 0;
}




void run_make(char *target, Rule *rules, int pflag) {
	
  //1. search the rule for the given target
  Rule* first_rule = search_rules(target);
  //the selected target is not in the list
  if(target == NULL){
    first_rule = rules;
  }

  //base Case: the given target not contains any dep
  if(first_rule->dependencies == NULL){
    if(latest_time == 0){
      latest_time = getFileModifiedTime(first_rule->target);
    }else{
      int is_new = is_first_newer(first_rule->target, latest_time);
      if(is_new == 1){
        latest_time = getFileModifiedTime(first_rule->target);
      }
    }
  }
  //recursion step: dependency is not NULL
  else{
    latest_time = 0;
    Dependency *cur_dep = first_rule->dependencies;
    while(cur_dep->next_dep != NULL){
      run_make(cur_dep->rule->target, rules, pflag);
    }

    int is_new = is_first_newer(first_rule->target, latest_time);


    if(is_new == 0){//current target is older than all of its deps
      //here for updating the current rule!
      Action *cur_ac = first_rule->actions;

      while(cur_ac != NULL){
        int result;
        //call fork:
        result = fork();
        if(result == -1){//error check
          perror("run_make error fork");
          exit(1);
        }

        else if(result == 0){//child process
          int ret = execute(cur_ac->args);
          if(ret == -1){
            exit(-1);//if execute is failed
          }
        }

        else{//parent
          int status;
          if(wait(&status) ==1){
            perror("run_make error wait");
            exit(1);
          }
          if(WIFEXITED(status) == -1){
            exit(1);// execute failed
          }
        }
        cur_ac = cur_ac->next_act;
      }
    }
  }
}

