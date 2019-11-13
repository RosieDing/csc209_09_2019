#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "pmake.h"

extern Rule **record_rules;
extern int length_record_rules;

void run_make(char *target, Rule *rules, int pflag) {
	if(record_rules == NULL || length_record_rules ==0){
		return 0;
	}
    //1. search the rule for the given target
    Rule* first_rule = serach_rules(record_rules, length_record_rules, target);
    //the selected target is not in the list
    if(first_rule == NULL){
    	first_rule = rules;
    }

    //


    return ;
}

