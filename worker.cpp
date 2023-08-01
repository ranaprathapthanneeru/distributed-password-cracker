#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#define PORT 8881
//   char *password;
	int sent;
   string x;
   int sockfd,free_workers,nth_worker,ap,aln;
        char buffer[19];
int permute(char set[],int n,int size,char *str)
{
 	   	ap=((size*nth_worker)/free_workers);
   	aln=ap+(size/free_workers);
   if(n==0){
        string y=crypt(str,"aa");
    if(x==y){//cout<<y<<str<<endl;
		        	sent=1;
      return 0;}
      return -1;
   }
   else if(n==1){
   	for(int j=ap;j<aln;j++){
            str[n-1]=set[j];
            if(permute(set,n-1,size,str)==0){
              return 0;
            }   		
   	}
   	return -1;
   }
   else
   {
 
           for(int j=0;j<size;j++)
           {
            str[n-1]=set[j];
            if(permute(set,n-1,size,str)==0){
              return 0;
            }
           }
 
   }
 
}
 
int main(int argc, char const *argv[])
{
    if(argc==3)
    {
        int valread,k=0,bit;
    char * str;
          int n;

        //string password=argv[3],length=argv[4],bits=argv[5];
        struct sockaddr_in my_addr;
    	int addrlen = sizeof(my_addr);
        sockfd=socket(PF_INET,SOCK_STREAM,0);
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(atoi(argv[2]));
        my_addr.sin_addr.s_addr = inet_addr(argv[1]);

        if(sockfd==0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        if (connect(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        buffer[0]='k';

		send(sockfd,buffer,1,0);
        while(true)
        {
            if ((valread = read( sockfd , buffer,19)) == 0)
            {
                getpeername(sockfd , (struct sockaddr*)&my_addr , (socklen_t*)&addrlen);
                printf("Server disconnected , ip %s , port %d \n" , inet_ntoa(my_addr.sin_addr) , ntohs(my_addr.sin_port));

                close( sockfd );
                break;
            }
            else
            {	
            	sent=0;
		        	k=buffer[13];
		        	n=k-48;
		        	    str=(char *)malloc(sizeof(char)*n);
		        	free_workers=buffer[18];
		        	nth_worker=buffer[17];	
		        	bit=(buffer[14]-48)*100+(buffer[15]-48)*10+buffer[16]-48;
		        	string l(buffer);
		        	x=l.substr(0,13);
		        	char set1[]={'1','2','3','4','5','6','7','8','9','0'};
					char set2[]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
					char set3[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
					char set4[]={'1', '2','3','4','5','6','7','8','9','0','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    				char set5[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1', '2','3','4','5','6','7','8','9','0'};
    				char set6[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    				char set7[]={'1', '2','3','4','5','6','7','8','9','0','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  switch(bit){
    case 001 :
      permute(set1,n,(sizeof(set1)/sizeof(char)),str);
      break;
    case 010 :
    permute(set2,n,(sizeof(set2)/sizeof(char)),str);
      break;
    case 100 :
     permute(set3,n,(sizeof(set3)/sizeof(char)),str);
      break;
    case 011 :
    permute(set4,n,(sizeof(set4)/sizeof(char)),str);
    break;
    case 101 :
    permute(set5,n,(sizeof(set5)/sizeof(char)),str);
    break;
    case 110 :
    permute(set6,n,(sizeof(set6)/sizeof(char)),str);
    break;
    default :
    permute(set7,n,(sizeof(set7)/sizeof(char)),str);
  }

            }

    if(sent==0){
    	buffer[0]='$';
    }
    else{

    }
		        	x=string(str);
					for(int j=0;j<n;j++){
		        	buffer[j]=x[j];
		        	}
              	send(sockfd,buffer,n,0);

        }


        return 0;
    }
    else
    {
        puts("correct number of arguments needed");
        return -1;
    }
}
