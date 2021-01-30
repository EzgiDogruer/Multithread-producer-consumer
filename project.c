//Ezgi Doğruer 150117042 - İsra Nur Alperen 150117061 - Elif Gökpınar 150117510 - PROJECT 3
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//Book Struct
typedef struct books
{ char *bookName;
  int id;
  struct books *next;
}book;

//Buffer struct
typedef struct buffer
{
   int type;
   int number;
   int size;
   int index;
   book * bookBuffer;
}bf;

//Publisher struct
typedef struct publisher
{
   int thread_id;
   pthread_t ptId;
   int	type;
   int numOfBook;
   char *book;
   int isGone; 
}pb;

//Packager struct
typedef struct packager
{
   int pk_id;
   int size;
   int count;
   pthread_t ptId;
   char * bookBuffer;
}pk;


//Global variables
//semaphores
sem_t *queue; 
sem_t *queue2;
sem_t queue3;
bf *buffers; //stores all publisher buffers and their information
pk * packagers; //stores all packagers and their information
pb * pb_arr; //stores all publishers and their information
int numOfType;//stores number of publisher type
int numOfPubTh;//stores number of all publishers threads m*n
int totalIsGone;//stores how many thread left system
int threadInOnePub;//stores number of threads for each publishers
int isAllBufferEmpty;//stores how many buffer empty

//FUNCTION PROTOTYPES
void createLinkedlistBuffer(int length, int type);//creates or resize linkedlist according to given length for given type buffer
void addLinkedlist(int type, char *book_name, int thread_id);//adds given bookname to bookBuffer linkedlist in buffers array for given type
void *Publisher(void *threadPublisher);//publisher func. to send when create publisher thread
void *Package(void *threadPackager);//packager func. to send when create packager thread
void replaceBuffer(int type);//after package takes books we not delete any node only shift node information

//MAIN FUNCTION
int main(int argc,char* argv[]) {
    
    
    int counter; 
    //ERROR CHECK FOR COMMAND LINE ARGUMENTS
    if(argc!=10 || strcmp(argv[1],"-n")!=0 || strcmp(argv[5],"-b")!=0 || strcmp(argv[7],"-s")!=0 || !atoi(argv[2])
    || !atoi(argv[3]) ||  !atoi(argv[4]) || !atoi(argv[6]) || !atoi(argv[8]) || !atoi(argv[9]))  {
        printf("Wrong input,try again\n"); 
        exit(0);
        }
        
     //Publisher array size
     
     int type=atoi(argv[2]);
     numOfType=atoi(argv[2]);
     int publisherThread=atoi(argv[3]);
     int pubBufferSize=atoi(argv[9]);
     numOfPubTh=type*publisherThread;
     threadInOnePub=atoi(argv[3]);
     isAllBufferEmpty = numOfType;
     int j=0;   
     int k=0;
     //Semaphore array assignment
     //If one type is busy, the other type can be run in the publisher.
     queue=(sem_t *)malloc(type* sizeof(sem_t));
     queue2=(sem_t *)malloc(type* sizeof(sem_t));
     for (j=0; j<type ; j++){
     sem_init (&queue[j],0,1);
     sem_init (&queue2[j],0,1);
     }   
     sem_init (&queue3,0,1);
     
     
     
     //---------------BUFFERS---------------------------
     buffers=(bf *)malloc(type * sizeof(bf));
     
     k=0;
     while (k<type){
     buffers[k].type=k+1;
     buffers[k].number=0;
     buffers[k].index=0;
     buffers[k].size=atoi(argv[9]);
     createLinkedlistBuffer(buffers[k].size, buffers[k].type);
     
     k++;
     }
     
     //-------------PACKAGERS-----------------------
     	packagers=(pk *)malloc((atoi(argv[4])) * sizeof(pk));
     	k=0;
   	while (k<atoi(argv[4])){
	     packagers[k].pk_id=k+1;
	     packagers[k].size=atoi(argv[8]);
	     packagers[k].bookBuffer = malloc(25* atoi(argv[8])* sizeof(char));
    	     k++;
     }
     
     //Publisher struct array
     pb_arr= (pb *)malloc((numOfPubTh)* sizeof(pb));
   
      int t=0; 
      int threadReturn=0;
      // Create threads for packagers
       for(k=0;k<atoi(argv[4]); k++){
          
	      threadReturn = pthread_create(&packagers[k].ptId, NULL, Package, (void *) 
	      &packagers[k]);
	      
	      //error check for threads      
	      if (threadReturn) {
	      printf("ERROR; return code from pthread_create() is %d\n", threadReturn);
	      exit(-1);
	      }
      }
     
      // Create threads for publishers
      for(t=0;t<type;t++) {   
      for(k=0;k<publisherThread; k++){
          
	      pb_arr[publisherThread*t+k].thread_id = k+1;
	      pb_arr[publisherThread*t+k].type = t+1;
	      pb_arr[publisherThread*t+k].numOfBook = atoi(argv[6]);
	      pb_arr[publisherThread*t+k].isGone = 0;
	      pb_arr[publisherThread*t+k].book=(char *)malloc(atoi(argv[6])* 25 * sizeof(char));
	      threadReturn = pthread_create(&pb_arr[publisherThread*t+k].ptId, NULL, Publisher, (void *) 
	      &pb_arr[publisherThread*t+k]);
	      //error check for threads      
	      if (threadReturn) {
	      printf("ERROR; return code from pthread_create() is %d\n", threadReturn);
	      exit(-1);
	      }
      }
     }
     //join threads for packagers
     for(j=0; j< atoi(argv[4]) ; j++){
		pthread_join(packagers[j].ptId,NULL);}
		
     //join threads for publishers
     for(j=0; j<numOfPubTh ; j++){
		pthread_join(pb_arr[j].ptId,NULL);}
		
      printf("\nEXITING THE SYSTEM...\n");  
      pthread_exit(NULL);
         
	
    return 0; 
} 
// create or resize linkedlist according to given length for given type buffer
void createLinkedlistBuffer(int length, int type){
	int k;
	for( k=0; k < length ;k++){
	
		if(buffers[type-1].bookBuffer == NULL){
		 buffers[type-1].bookBuffer = (book*)malloc(sizeof(book));
		 buffers[type-1].bookBuffer->bookName = (char*)malloc(sizeof(char)* 25 );
		 strcpy(buffers[type-1].bookBuffer->bookName, " ");
		 buffers[type-1].bookBuffer -> next = NULL;
		 }
		 
		 else{
		 book* last = buffers[type-1].bookBuffer;
    		 while(last->next != NULL){
        		last = last->next;
  		  }
  		  
		  //add to the end
		    book* new = (book*)malloc(sizeof(book));
		    new->bookName = (char*)malloc(sizeof(char)* 25 );
		    sprintf(new->bookName,"%s", " ");
		    new->next = NULL;
		    last->next = new;
		   }
	 }
	
    }
    
//adds given bookname to bookBuffer linkedlist in buffers array for given type 
void addLinkedlist(int type, char *book_name, int thread_id){
int k = 0;
char a[25]="";
	sprintf(a,"%d",buffers[type-1].number);
	strcat(book_name, a );
	sem_wait(&queue2[type -1]);
	book *temp = buffers[type-1].bookBuffer;
	//book_name stores book name like Book2_9
	if(strcmp(temp->bookName," ")==0) isAllBufferEmpty--;
	for(k=0; (strcmp(temp->bookName," ")!=0)  && temp->next != NULL; k++, temp= temp->next);
	//adds
	if(strcmp(temp->bookName, " ") ==0 && k < buffers[type-1].size){
	sprintf(temp->bookName,"%s" ,book_name);
	temp->id = thread_id;
	printf("\nPublisher %d type %d \t%s is published and put into the buffer %d.", thread_id, type,temp->bookName,type);
	}
	//if buffer size is fulled doubled buffer size
	else if(k+1 == buffers[type-1].size ){
	createLinkedlistBuffer(buffers[type-1].size, type);
	buffers[type-1].size *= 2;
	temp=temp->next;
	printf("\nPublisher %d type %d \tBuffer is full. Resizing the buffer.", thread_id, type);
	sprintf(temp->bookName,"%s" ,book_name);
	printf("\nPublisher %d type %d \t%s is published and put into the buffer %d.", thread_id, type,temp->bookName,type);
	temp->id = thread_id;
	
	}
	sem_post(&queue2[type -1]);
	
	
}

//***********************PUBLISHER - PRODUCER**************************
void *Publisher(void *threadPublisher){
  pb *my_data;
  my_data = (pb *) threadPublisher;
  int i=0;
  int j=0;
  int k=0;
  
  
  for (i=0 ; i<(my_data->numOfBook); i++){
  sem_wait(&queue[my_data->type -1]);
  //create name of book
  char name[25]="Book";
  char temp[25];
  sprintf(name, "Book%d_",my_data->type);
  //
  buffers[my_data->type -1].number++;
  //if publisher threads create given input
  addLinkedlist(my_data->type, name, my_data->thread_id);
  if(i+1==my_data->numOfBook) {
  	pb_arr[(my_data->type -1)*threadInOnePub + my_data->thread_id - 1].isGone=1;
  	totalIsGone++;
  	printf("\nPublisher %d type %d \tfinished publishing %d books.", my_data->thread_id, my_data->type, my_data->numOfBook);
  	pthread_join(my_data->ptId,NULL);
  	printf("Exiting the system..");
  	
  	}
  sem_post(&queue[my_data->type -1]);
  }    
  
  
}
//-------------------------------------PACKAGER
void *Package(void *threadPackager){
  pk *my_package;
  my_package = (pk *) threadPackager;
  //printf("\n %d ",my_data->thread_id);
  int i=0;
  int j=0;
  int random ;
  time_t timet;
  srand((unsigned)time(&timet));
  
  //takes input from buffers
  while ( (totalIsGone !=numOfPubTh && totalIsGone < numOfPubTh) || isAllBufferEmpty != numOfType ){
  //random value assign
  random=rand()%numOfType;
  sem_wait(&queue[random]);
 
  if(strcmp(buffers[random].bookBuffer->bookName," ") != 0){
  if(packagers[my_package->pk_id-1].count!=0){
  strcat(packagers[my_package->pk_id-1].bookBuffer,", ");}
  strcat(packagers[my_package->pk_id-1].bookBuffer,buffers[random].bookBuffer->bookName);
  printf("\nPackager %d\tPut %s into the package.",packagers[my_package->pk_id-1].pk_id, buffers[random].bookBuffer->bookName);
  replaceBuffer(buffers[random].type);
  packagers[my_package->pk_id-1].count++;
  
  }
  //if finished
  if(packagers[my_package->pk_id-1].count==my_package->size) {
  	printf("\nPackager %d\tFinished preparing one package. The package contains: %s.  ",packagers[my_package->pk_id-1].pk_id, packagers[my_package->pk_id-1].bookBuffer);
  	strcpy(packagers[my_package->pk_id-1].bookBuffer,"");
  	packagers[my_package->pk_id-1].count=0;
  	}
  sem_post(&queue[random]);
  }    
  //sem_wait(&queue3);
  
  if(totalIsGone ==numOfPubTh &&  isAllBufferEmpty == numOfType){
  
 
  printf("\nPackager %d\tThere are no publishers left in the system. Only %d of %d number of books could be package. The package contains: %s.  ",packagers[my_package->pk_id-1].pk_id, packagers[my_package->pk_id-1].count, packagers[my_package->pk_id-1].size, packagers[my_package->pk_id-1].bookBuffer);
  
  }
  //sem_post(&queue3);
  
}
//after package takes books we not delete any node only shift node information
void replaceBuffer(int type){
	book * temp=buffers[type-1].bookBuffer;
   int j;
  	if(strcmp(temp->next->bookName," ")==0 || strcmp(temp->bookName," ")==0){ isAllBufferEmpty++;}
  	
     for(j=0; j<buffers[type-1].size && temp->next!=NULL ; j++){
     
     if(strcmp(temp->next->bookName," ")!=0){
     strcpy(temp->bookName,temp->next->bookName);
     strcpy(temp->next->bookName," ");
     temp->id= temp->next->id;
     temp->next->id=0;
     buffers[type-1].index--;
     }
     
     else{
     strcpy(temp->bookName," ");
     temp->id=0;
     break;}
     temp = temp -> next;
     }

}

