#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include "socket.h"  
#include "message.h"
#include "controller.h"

#define MAXFD(x,y) ((x) >= (y)) ? (x) : (y)

/* Read a message from client_index and echo it back to them.
 * Return the fd if it has been closed or 0 otherwise.
 */
int read_from(int peerfd, char *cig_serialized, struct cignal *cig) {
    
    int fd = peerfd;
    int num_read = read(fd, cig_serialized, CIGLEN);
    cig_serialized[num_read] = '\0'; 

    if (num_read == 0) {//current fd is closed
        return fd;
    }

    //the message sent success!

    printf("RAW MESSAGE: %s\n", cig_serialized);
    //2. unpacked the char into struct cignal
    unpack_cignal(cig_serialized, cig);


    return 0;

}



int main(int argc, char *argv[]){
	int port;
	struct cignal cig;
	// A buffer to store a serialized message
	char *cig_serialized = malloc(sizeof(char)*CIGLEN);
	// An array to registered sensor devices
	int device_record[MAXDEV] = {0};
	
	if(argc == 2){
		port = strtol(argv[1], NULL, 0);
	} else{
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	int gatewayfd = set_up_server_socket(port);
	printf("\nThe Cignal Gateway is now started on port: %d\n\n", port);
	int peerfd;
	
	/* TODO: Implement the body of the server.  
	 *
	 * Use select so that the server process never blocks on any call except
	 * select. If no sensors connect and/or send messsages in a timespan of
	 * 5 seconds then select will return and print the message "Waiting for
	 * Sensors update..." and go back to waiting for an event.
	 * 
	 * The server will handle connections from devices, will read a message from
	 * a sensor, process the message (using process_message), write back
	 * a response message to the sensor client, and close the connection.
	 * After reading a message, your program must print the "RAW MESSAGE"
	 * message below, which shows the serialized message received from the *
	 * client.
	 * 
	 *  Print statements you must use:
     * 	printf("Waiting for Sensors update...\n");
	 * 	printf("RAW MESSAGE: %s\n", YOUR_VARIABLE);
	 */

	// Suppress unused variable warning.  The next 5 ilnes can be removed 
	// after the variables are used.
	// (void)gatewayfd;
	// (void)peerfd;
	// (void)cig;
	// (void)device_record;
	// (void)cig_serialized;

	int max_fd = gatewayfd;
	fd_set all_set;
	FD_ZERO(&all_set);
	FD_SET(gatewayfd, &all_set);

	//struct timeval timeout; //= {5,0};
	//timeout.tv_sec = 800;
	//timeout.tv_usec = 0;

	//int device_list[MAXDEV] = {0};

	while(1) {

       struct timeval timeout;
	   timeout.tv_sec = 5;
	   timeout.tv_usec = 0;

		//1.firstly select through all the fd available
		//--make a copy of all fd
		fd_set copy_all = all_set;
		//--apply select
		int nearby = select(max_fd+1, &copy_all, NULL, NULL, &timeout);
		if(nearby == 0){
			printf("Waiting for Sensors update...\n");
		}
		if(nearby ==-1){
			perror("gateway: select");
            exit(1);
		}

		//2. check user connection --> from listen socket
		if(FD_ISSET(gatewayfd, &copy_all)){
			peerfd = accept_connection(gatewayfd);
			if(peerfd == -1){
				perror("gateway: accept_connection");
				exit(1);
			}
			//new_connect accept success
			max_fd = MAXFD(peerfd, max_fd);
			FD_SET(peerfd, &all_set);
		}

		for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &copy_all) && i != gatewayfd) {
                int pt = read_from(i, cig_serialized, &cig);
                if (pt > 0) {
                	close(i);
                    FD_CLR(i, &all_set);
                    //close the fd
                }
                else {
                	pt = process_message(&cig, device_record);
                	if(pt < 0){
                		perror("process_message");
			            exit(1);
			        }
			        strcpy(cig_serialized,serialize_cignal(cig));
			        cig_serialized[CIGLEN] = '\0';

			        pt = write(peerfd, cig_serialized, CIGLEN);
			        if(pt != CIGLEN){
			        	perror("gateway write failed");
			        	exit(1);
			        }
			        close(peerfd);
			        FD_CLR(peerfd, &all_set);//finish hand shake/conversation-> disconnected!

                }
            }
        }
    }

    return 0;
}


