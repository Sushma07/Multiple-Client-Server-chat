
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <arpa/inet.h> 
#include<unistd.h> 
#include <pthread.h>

#define PORT 8080
#define MAXSZ 256
#define MAXCLIENTS 6


struct cl_details{
int name;
int ip;
struct cl_details *next;
};

struct new_thread{
int i;
struct cl_details **head_ref;
int sockfd;
};

int search(struct cl_details* head, char* x)
{

    struct cl_details* current = head;  
    while (current != NULL)
    {
        if (current->name == atoi(x))
           { 
            return (current->ip);
           }
        current = current->next;
    }
    return 0;
}

void delete_client(struct cl_details* head, int sockfd)
{
    struct cl_details* current = head; 
    while (current != NULL)
    {
        if (current->ip == sockfd)
           { 
            (current->ip)=0;
           }
        current = current->next;
    }
    
}


void *client( void *arguments){
char msg[MAXSZ];
struct new_thread *nt = arguments;
int sockfd= nt->sockfd;
int i = nt->i;
memset(msg, '0', 256);

printf("Hi client: %d!\n",i);
struct cl_details **head_ref = nt->head_ref;
struct cl_details *cld = (struct cl_details*)malloc (sizeof(struct cl_details));
  cld->ip = sockfd;
  cld->name =i;
  cld ->next = (*head_ref);
  (*head_ref) = cld;

 struct cl_details *print_var = (*head_ref);
 printf("The list of active clients\n");
     
       while (print_var != NULL)
    {
       printf("Client %d\n",print_var->name);
       print_var = print_var->next;
    }

  int newsockfd = sockfd;
 
 while(1){
  int n=recv(newsockfd,msg,MAXSZ,0);
   
         if(n>0){
         printf("Client %d says %s",newsockfd,msg);}
		   if(n<0) {
		   printf("Error receiving\n");
		   exit (0);
		    }           	    
        
                if(n==0)   // client logout, server receives null
			       {
			       printf("\n Client %d exited\n",newsockfd); 
			       //n1=0;
                               delete_client((*head_ref),newsockfd);
                               pthread_exit(NULL);
                               }
                    
                 int newsockfd1 = search((*head_ref),&msg[0]);
                      msg[0]='0'+i;
                int n1 = send(newsockfd1,msg,MAXSZ,0);       
               
		 if( n1 >0)
		  printf("Message sent\n");
		 if (n1<0)
		 printf("Error sending the message\nRequested client unavailable \n");
                

  }//while 
  
}



int main()
{
 int sockfd,newsockfd;
 struct sockaddr_in serverAddress,clientAddress;

 int n,clientAddressLength;
 pid_t pid;
 pthread_t tid1,tid2,threads[MAXCLIENTS];
int i=1;

struct cl_details *print_var,*head_ref = NULL;


 // Socket creation
 sockfd=socket(AF_INET,SOCK_STREAM,0);

 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(PORT);

 //bind
 bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

 //listen 
 listen(sockfd,5);
printf("\n Server ready to accept Connections------\nMax limit 6\n");

 while(1)
 {
  clientAddressLength = sizeof(clientAddress);
 
//accept 
  newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
  
 
// details of client to cl_details 
   struct new_thread *new_t = (struct new_thread*) malloc(sizeof(struct new_thread));
	new_t->sockfd=newsockfd;
	new_t->i=i;
	new_t->head_ref = (&head_ref);
	pthread_create(&threads[i],NULL,&client,(void*)new_t);
	i++;

   }//while

 int j;
		for (j=1;j<MAXCLIENTS;j++){
		pthread_join(threads[j],NULL);
                //close(new_t->sockfd);
		}// for 
 return 0;
}


