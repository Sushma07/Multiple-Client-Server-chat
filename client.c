//  change the ip address while running on different machine
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<pthread.h>
#include<unistd.h>

#define PORT 8080
#define SERVER_IP "192.168.122.1"
#define MAXSZ 256

struct argument_rec{
 int sid;
 char *str;
};

struct argument_send{
 int p; // for break;
 int ssid;
};


void *receive (void *arguments )
{
 //static char empty[4]="NULL";
 struct argument_rec *ar_rec = arguments;
  int newsockfd = ar_rec->sid;
  char *msg = ar_rec-> str;
 while(1){
  int n=recv(newsockfd,msg,MAXSZ,0);
         if(n>0){
           
         printf("Message recieved from & as :%s",msg); }
		   if(n<0) {
		   printf("Error receiving\n");
		   exit (0);
		    }
			           
         
  }//while 
}

void *send_server (void *argument){
 char msg1[MAXSZ];
 struct argument_send *ar_send = argument;
 int newsockfd = ar_send->ssid;
 int n = ar_send->p;
     
    printf(" Format to send: client id (space) msg:\n");
  while(1){
      	fgets(msg1,MAXSZ,stdin);
  	
  			if(strcmp(msg1,"logout\n")==0)
			      {
			       printf("Good bye peers, I quit\n"); 
			       exit(0);
                               pthread_exit(NULL);
                               
			      }
   //n=strlen(msg1)+1;
  	send(newsockfd,msg1,MAXSZ,0);
   }//while

}

int main()
{
pthread_t tid1,tid2;
//int i=1;
struct argument_rec ar_rec;
struct argument_send ar_send;
 int sockfd,newsockfd;
 struct sockaddr_in serverAddress;
 int n,e;
 char msg[MAXSZ],msg2[MAXSZ];
                memset(msg, '0', 256);
		memset(msg2, '0', 256);
 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 
//initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=inet_addr(SERVER_IP);
 serverAddress.sin_port=htons(PORT);

 
//Connection
  newsockfd = connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
  if(newsockfd<0)
  {
  printf("Connection failed\n Make sure server is started\n");
  return 0;
  }
 
  
 
     // Receive
            ar_rec.sid = sockfd;
	    ar_rec.str =  msg;
	    pthread_create(&tid1,NULL, &receive, (void*)&ar_rec);
       
	  

// send to client
	       ar_send.ssid= sockfd;
	       ar_send.p = n;
	     pthread_create(&tid2,NULL,&send_server, (void*)&ar_send);
             pthread_join(tid2, NULL);
             pthread_join(tid1, NULL);
             close(sockfd);
   

return 0;
}




