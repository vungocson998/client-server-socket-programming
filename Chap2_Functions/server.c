#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>

int main(int argc, char *argv[]){
   /*Check if right input port*/
   if( argc != 2 ){
       printf("Wrong input!\n");
       return -1;
   } 

   int server_socket;
   int server_port;
   struct sockaddr_in server_address;
   int returnStatus;
   char * message;
   char buffer[256]="This is ted!";

   /*Initialize socket and server's port*/
   server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (server_socket == -1){
    printf("Unsuccessfull creating server socket!\n");
    return -1;
   } 
   server_port = atoi(argv[1]);
   printf("Server port: %d\n", server_port);

   /*Clear temp data then set up server_address
   **Why we use bzero() instead using memset() ?*/
   bzero(&server_address, sizeof(server_address));

   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(server_port);

   returnStatus = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
   if (returnStatus == 0){
       printf("Bind completerd!\n");
   }
   else{
       printf("Bind unsuccessfully, exit!\n");
       close(server_socket);
       return -1;
   }

   returnStatus = listen(server_socket, 5);
   if (returnStatus == -1){
       printf("Can not listen to the socket!\n");
       close(server_socket);
       return -1;
   }
   printf("Server is listening on port %d!\n", ntohs(server_address.sin_port));
   
   while(1){
    /*Set up variables to handle client connect*/
    struct sockaddr_in client_address = {0};
    int simpleClient;
    int client_addressLength = sizeof(client_address);

    /*Block on accecpt function call*/
    simpleClient = accept(server_socket, (struct sockaddr *)&client_address, &client_addressLength);

    if (simpleClient == -1){
        printf("Cannot accept connection!\n");
        close(server_socket);
        return -1;
    }

    printf("Type your messages:\n");
    gets(buffer);
    write(simpleClient, buffer, strlen(buffer));
    printf("\nSent: %s", buffer);
    close(simpleClient);
    break;
   }

   close(server_socket);
   return 0;    
}