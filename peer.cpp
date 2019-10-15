/*Peer to Peer file trasnfer*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<pthread.h>
#include<netdb.h>
#include<errno.h>
#include<ctype.h>

#define MAX 1024

using namespace std;



void *servermodule(void* port_no_1);
void *transfer(void* new_sockfd_2);



void *transfer(void* new_sockfd_2)
{
    int read_pointer;
    int sock_fd_1;
    char buffer2[MAX];
    int new_sockfd_1 = *((int*)new_sockfd_2);
    struct sockaddr_in serv_addr,cli_addr;
    char buffer3[MAX];
    int size;
    FILE *fp;

    recv(new_sockfd_1,&buffer3,sizeof(buffer3),0);

    fp=fopen(buffer3,"r");

    if(fp==NULL)
    {
        perror("ERROR! FILE DOES NOT EXIST!");
       exit(1);
    }

    fp=fopen("original.txt","rb");

    fseek(fp,0,SEEK_END);
    size=ftell(fp);
    fseek(fp,0,SEEK_SET);
    send(new_sockfd_1,&size,sizeof(size),0);

    while((read_pointer=fread(buffer2,sizeof(char),sizeof(buffer2),fp))>0 && size>0)
    {
        send(new_sockfd_1,buffer2,read_pointer,0);
        bzero(buffer2,sizeof(buffer2));
        size=size-read_pointer;
    }

    close(new_sockfd_1);
    close(sock_fd_1);

    return 0;
}


void *servermodule(void* port_no_1)
{
    int port_no_2;
    int sockfd_2;
    int new_sockfd_3;
    struct sockaddr_in serv_addr,cli_addr;
    socklen_t clilen;

    sockfd_2=socket(AF_INET,SOCK_STREAM,0);

    if(sockfd_2<=-1) 
    {
        perror("fail to create to socket");
        exit(1);
    }
   
    bzero((char*)&serv_addr,sizeof(serv_addr));
   
    port_no_2=atoi((char*)port_no_1);
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no_2);

    if (bind(sockfd_2,(struct sockaddr*) &serv_addr,sizeof(serv_addr))<0)
    {
        perror("fail to bind");
        exit(1);
    }

    listen(sockfd_2,5);
    clilen = sizeof(cli_addr);

    while(1)
    {
    new_sockfd_3=accept(sockfd_2,(struct sockaddr*) &cli_addr,&clilen);
   
        if(new_sockfd_3<=-1)  //add -1
       {
        perror("Failed to accept!!");
        exit(1);
       }
       pthread_t newthread;
       pthread_create(&newthread,NULL,transfer,(void*) &new_sockfd_3);
       pthread_join(newthread,NULL);

    }
}



int main(int argc, char* argv[])
{


    pthread_t newthread1;
    pthread_create(&newthread1,NULL,servermodule,(void*) argv[3]);

    int sockfd_3;
    int port_no_2;
    int read_pointer_1;
    int file_size_1;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer1[MAX];

    if(argc<4)
    {
        fprintf(stderr,"hostname or portno missing %s\n",argv[0]);
        exit(1);
    }

    port_no_2=atoi(argv[2]);

    sockfd_3=socket(AF_INET,SOCK_STREAM,0);

    if(sockfd_3<=-1) 
    {
        perror("error in socket");
        exit(1);
    }

    server=gethostbyname(argv[1]);

    if(!server) 
    {
        perror("ERROR! INVALID HOST!!");
        exit(1);
        //fprintf(stderr,"invalid host");
    }

     bzero((char *)&serv_addr,sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     bcopy((char *)server->h_addr_list[0],(char*)&serv_addr.sin_addr.s_addr,server->h_length);
     serv_addr.sin_port=htons(port_no_2);

    if(connect(sockfd_3,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
          perror("ERROR !Failed to connect");
          exit(1);
    }

    bzero(buffer1,sizeof(buffer1));
    char buffer_2[MAX]= "original.txt";
    send(sockfd_3,buffer_2,sizeof(buffer_2),0);

    FILE *fp;
    fp=fopen("copy.txt","a");

   

    recv(sockfd_3, &file_size_1,sizeof(file_size_1),0);

    cout<<file_size_1<<endl;

     while((read_pointer_1=recv(sockfd_3,buffer1,sizeof(buffer1),0))>0 && file_size_1 > 0)
    {
        cout<<read_pointer_1<<endl;

        fwrite(buffer1,sizeof(char),read_pointer_1,fp);
        fflush(fp);
        bzero(buffer1,sizeof(buffer1));
        file_size_1 =file_size_1-read_pointer_1;
    }
    pthread_join(newthread1,NULL);

    fclose(fp);
    close(sockfd_3);

    return 0;
}

/* HOW TO RUN
   PEER1
   g++ peer1.cpp -o peer -lpthread
   ./peer 127.0.0.1 (tracker port_no) (port_no)

   PEER2
   ./peer 127.0.0.1 (PEER1 port_no) (port_no)
   */


  


