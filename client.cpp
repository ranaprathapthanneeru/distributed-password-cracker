#include <stdio.h>
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
#include <ctime>
#include <netdb.h>
using namespace std;
#define PORT 8881

int main(int argc, char const *argv[])
{
    if(argc==6)
    {
        time_t initial=time(NULL);
        int sockfd ,no_clients=3,client_socket[3],valread;
        string password=argv[3],length=argv[4],bits=argv[5];
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
        char buffer[1025];
        buffer[0]='y';
        password = password + length + bits;
        int k=0;
		send(sockfd,buffer,1,0);
        send(sockfd,password.c_str(),17,0);
        while(true)
        {
            if ((valread = read( sockfd , buffer,atoi(length.c_str()) )) == 0)
            {
                getpeername(sockfd , (struct sockaddr*)&my_addr , (socklen_t*)&addrlen);
                printf("Server disconnected , ip %s , port %d \n" , inet_ntoa(my_addr.sin_addr) , ntohs(my_addr.sin_port));

                close( sockfd );
                break;
            }
            else
            {	
        time_t final=time(NULL);
        cout<<"Time : "<<(final-initial)<<" seconds"<<endl;
		        	for(int j=0;j<atoi(length.c_str());j++){
		        	cout<<buffer[j];}
		        	cout<<endl;
		        	close(sockfd);
					break; 
            }
        }
        return 0;
    }
    else
    {
        puts("correct arguments needed");
        return -1;
    }
}
