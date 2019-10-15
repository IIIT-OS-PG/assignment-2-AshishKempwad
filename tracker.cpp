/*Server(tracker) t bind the port number*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<netinet/in.h>
#include<ctype.h>
#include<iostream>

#define MAX 512


int main(int argc, char *argv[])
{
     int portno;
     int current_sockfd;
     int new_sockfd; 
  
     socklen_t client_length;
     char buffer[MAX];

     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc<=1) 
     {    perror("ERROR! PROVDE PORT TO PROCEED!");
         
         exit(1);
     }

     current_sockfd=socket(AF_INET,SOCK_STREAM,0);

     if(current_sockfd<0) 
     {
        perror("ERROR opening socket");
        exit(1);
     }


     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);


     if(bind(current_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0) 
     {
        perror("ERROR on binding");
        exit(1);
     }

     listen(current_sockfd,5);
     client_length=sizeof(cli_addr);
     new_sockfd = accept(current_sockfd,(struct sockaddr *) &cli_addr,&client_length);

     if (new_sockfd<0)
     { 
        perror("ERROR on accept");
        exit(1);
     }  

     close(new_sockfd);
     close(current_sockfd);


     return 0; 
}
