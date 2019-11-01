#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];

  /* The user will type in a user name on one line followed by a password 
     on the next.
     DO NOT add any prompts.  The only output of this program will be one 
	 of the messages defined above.
   */

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  
  int pipe_fd[2];
  if(pipe(pipe_fd) == -1){
    perror("pipe failed");
    exit(1);
  }

  int pid = fork();
  if(pid < 0 ){
    perror("fork");
    exit(1);
  }
  if(pid > 0){//parent process
    
    if(close(pipe_fd[0])== -1){
      perror("parent read");
      exit(1);
    }
    write(pipe_fd[1], user_id, 10);
    write(pipe_fd[1], password, 10);

    int status;
        wait(&status);
        if (WIFEXITED(status)){
            if (WEXITSTATUS(status) == 0){
                printf(SUCCESS);
            } else if (WEXITSTATUS(status) == 2) {
                printf(INVALID);
            } else if (WEXITSTATUS(status) == 3) {
                printf(NO_USER);
            }
        }

  }

  if(pid == 0){
    if(close(pipe_fd[1])== -1){
      perror("child write");
      exit(1);
    }
    if (dup2(pipe_fd[0], fileno(stdin)) == -1) {
      perror("child dup2");
      exit(1);
    }

    if (close(fd[0]) == -1) {
            perror("child close");
            exit(1);
    } 
    execl("./validate", "validate", NULL);



  }

  return 0;
}
