#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "pmake.h"


//helper functions:
char** split_array_by_space(char* str, int* element_num){

    const char s[2] = " ";
    char *token;
    int element_length;
    int index = 0;
    char** storage_list = malloc(strlen(str)*(sizeof(char *)));//init a array to strore the length


    token = strtok(str, s);

    while(token != NULL){
        element_length = strlen(token);
        char* element = malloc( (element_length + 1) * sizeof(char));
        strcpy(element, token);
        storage_list[index] = element;
        index++;
        token = strtok(NULL, s);
    }

    *element_num = index;//number of valid node in the list: i
    //printf("%d\n", index);
    char** result_list = malloc((index + 1) * sizeof(char *));

    for(int i = 0; i < index; i++){
        result_list[i] = storage_list[i];
    }

    result_list[index] = NULL;

    free(storage_list);

    //debug
    // int i = 0;
    // while(result_list[i] != NULL) {
    //     printf("%s ", result_list[i]) ;
    //         i++;
    //     }

    return result_list;//Remember to free the list at last!!!!!!!!!!!!!!!!
}


//search for the target rule in the list
Rule* search_rules(Rule ** record_rules, int length_record_rules, char* target){
    if((record_rules != NULL) && (length_record_rules != 0)){
        for(int i = 0; i < length_record_rules; i++){
            if(strcmp(record_rules[i]->target, target)== 0){
                return record_rules[i];
            }
        }
    }

    return NULL;

}

Rule* create_rule(char* target, int *length_record_rules, Rule ** record_rules){
    Rule *new_rule = malloc(sizeof(Rule));
    new_rule->target = malloc(strlen(target)+1);
    strcpy(new_rule -> target, target);
    new_rule-> dependencies = NULL;
    new_rule-> actions = NULL;
    new_rule-> next_rule= NULL;
    //add this rule into our record_list
    record_rules[*length_record_rules] = new_rule;
    *length_record_rules += 1;

    return new_rule;

}

Dependency* create_dep(char *rule_name, Rule ** record_rules, int *length_record_rules){
    Dependency* new_dep = malloc(sizeof(Dependency));
    Rule *rule_find = NULL;
    if((*length_record_rules) != 0){//rules is not empty
        rule_find = search_rules(record_rules, *length_record_rules, rule_name);
    }

    if(rule_find == NULL){
        rule_find = create_rule(rule_name, length_record_rules, record_rules);
    }
    
    //record of rules is empty or rule not find:

    new_dep-> rule = rule_find;

    return new_dep;



}





/* Print the list of actions */
void print_actions(Action *act) {
    while(act != NULL) {
        if(act->args == NULL) {
            fprintf(stderr, "ERROR: action with NULL args\n");
            act = act->next_act;
            continue;
        }
        printf("\t");

        int i = 0;
        while(act->args[i] != NULL) {
            printf("%s ", act->args[i]) ;
            i++;
        }
        printf("\n");
        act = act->next_act;
    }    
}

/* Print the list of rules to stdout in makefile format. If the output
   of print_rules is saved to a file, it should be possible to use it to 
   run make correctly.
 */
void print_rules(Rule *rules){
    Rule *cur = rules;
    
    while(cur != NULL) {
        if(cur->dependencies || cur->actions) {
            // Print target
            printf("%s : ", cur->target);
            
            // Print dependencies
            Dependency *dep = cur->dependencies;
            while(dep != NULL){
                if(dep->rule->target == NULL) {
                    fprintf(stderr, "ERROR: dependency with NULL rule\n");
                }
                printf("%s ", dep->rule->target);
                dep = dep->next_dep;
            }
            printf("\n");
            
            // Print actions
            print_actions(cur->actions);
        }
        cur = cur->next_rule;
    }
}

/* Create the rules data structure and return it.
   Figure out what to do with each line from the open file fp
     - If a line starts with a tab it is an action line for the current rule
     - If a line starts with a word character it is a target line, and we will
       create a new rule
     - If a line starts with a '#' or '\n' it is a comment or a blank line 
       and should be ignored. \\helper.c: helper function return 1: 空白： return0: 需要用的行
     - If a line only has space characters ('', '\t', '\n') in it, it should be
       ignored.
 */
Rule *parse_file(FILE *fp) {//fp: already opened file pointer

    //printf("%s%d\n", "debug",1);

    Rule *rule_list = NULL;//the final rule list we need to return

    Rule *cur_rule_list = NULL;

    Rule *cur_readline_rule = NULL;

    Action *cur_act = NULL;

    //Rule *prev_rule = NULL;
    Rule **record_rules = malloc(sizeof(Rule *) * 256);// for record all the rules
    int length_record_rules = 0;

   // printf("%s%d\n", "debug",2);
    char str[MAXLINE] = {'\0'}; //init line of make file

    if(!fp){
        exit(1);
    }

    int num_element = 0;

    while(fgets(str, MAXLINE, fp)!= NULL){//read the lines
       // printf("%s%d\n", "debug",1);
        int is_empty = is_comment_or_empty(str);

        //if the line is a comment or empty
        if(is_empty == 1){
            continue;
        }


        for(int i =0; i < MAXLINE; i++){
            if(str[i] == '\n'|| str[i] =='#'){ //end of the line
                str[i] = '\0';//replace the '\n'
                break;
            }
        }



        //if the line is an action line
        if(strncmp(str, "\t", 1) == 0){
           // printf("%s\n", "--------start action");

            str[0] = ' ';
            char** refined_list = split_array_by_space(str, &num_element);

            Action* new_act = malloc(sizeof(Action));
            new_act->args = refined_list;
            new_act->next_act = NULL;

            //debug
           // for(int i = 0; i < num_element; i++){
            //printf("%s%s\n", "action:", refined_list[i]);}



            if(cur_act == NULL){//first cur_act of the target
                cur_act = new_act;
                if(cur_readline_rule == NULL){
                    perror("no target exist");
                    exit(1);
                }
                cur_readline_rule->actions= new_act;//add the action under the rule
                //printf("%s\n", cur_readline_rule->target);
                //printf("%s\n", "first arg add");

            }

            else{
                cur_act->next_act = new_act;
                cur_act = new_act;
            }

           // print_actions(new_act);

        }

        //if the line is a target line
        else{
            cur_act = NULL;

            char** refined_list = split_array_by_space(str, &num_element);
            // get the first target line
            char* target = refined_list[0];

            if(*(refined_list[1]) != ':'){
                    perror("target line false");
            }


            Rule* new_rule = search_rules(record_rules, length_record_rules, target);
            if(new_rule == NULL){
                new_rule = create_rule(target, &length_record_rules, record_rules);
            }

            if(rule_list == NULL){
                rule_list = new_rule;
                cur_rule_list = rule_list;
            }else{
                cur_rule_list->next_rule = new_rule;
                cur_rule_list = new_rule;
            }

            cur_readline_rule = new_rule;

            //printf("%s%d\n", "rule_list add success",1);
            //printf("%s\n", new_rule->target);

            //check number of dependency:
            if(num_element > 2){
            //there exits depencies
                int num_dep_left = num_element - 2;

                //Dependency *pre_dep = NULL;
                Dependency *cur_dep = NULL;

                for(int i = 0; i < num_dep_left; i++){//iterate all the deps

                    Dependency* dep = create_dep(refined_list[i+2], record_rules, &length_record_rules);
                    //printf("%s%s\n", "add dep",refined_list[i+2]);
                    if(i == 0){
                        cur_rule_list->dependencies = dep;
                    }

                    cur_rule_list->next_rule = dep->rule;
                    cur_rule_list = dep->rule;

                    if(cur_dep == NULL){
                        cur_dep = dep;
                    }
                    else{
                        cur_dep->next_dep = dep;
                        cur_dep = dep;
                    }

                    //printf("%s\n", "dep end success");
                }

            }

            for (int i = 0; i < num_element; i ++){
                free(refined_list[i]);
            }
            free(refined_list);
            //end
        }
    }
    //printf("%s\n", "finish");
    return rule_list;
}




