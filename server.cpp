#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h> 
using namespace std;
#define TRUE   1
#define FALSE  0
#define PORT 8881

int main(int argc , char *argv[])
{
    int master_socket , addrlen , new_socket , client_socket[3],client_length[3],client_alloted[3] , max_clients = 3 , activity, i , valread , sd;
    int worker_socket[5] ,length1, max_workers = 5 ,worker_occupied[5],worker_client[5],free_worker;
    int max_sockfd;
    int free_workers,nth_worker,bit[3];
    int max_sd;
    string x;
    struct sockaddr_in address;
    char buffer[19];  //data buffer of 1K
    char client1[17],client2[17],client3[17];
    //set of socket descriptors
    fd_set readfds;
    //a message
    char *message;
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
        client_alloted[i]=-1;
    }
    //initialise all worker_socket[] to 0 so not checked
    //worker_client[] which client , the workers are serving
    for (i = 0; i < max_workers; i++)
    {
        worker_socket[i] = 0;
        worker_occupied[i] = 0;
        worker_client[i] = -1;
    }
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("10.105.12.55");
    address.sin_port = htons( atoi(argv[1]) );
	int k=0;

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %s \n", argv[1]);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 8) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //cout<<master_socket;
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //adding child sockets(of clients) to the set.
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0){
                FD_SET( sd , &readfds);
            }

            //highest file descriptor number, need it for the select function
            if(sd > max_sd){
                max_sd = sd;
            }
        }
        for ( i = 0 ; i < max_workers ; i++)
        {
            //socket descriptor
            sd = worker_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0){
                FD_SET( sd , &readfds);
				}
            //highest file descriptor number, need it for the select function
            if(sd > max_sd){
                max_sd = sd;
            }
        }


        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            sd = new_socket;

            //Check if it was for closing , and also read the incoming message
            if ((valread = read(sd,buffer,1)) == 0)
        	{
            //Somebody disconnected , get his details and print
            	getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                //Close the socket and mark as 0 in list for reuse
                close( sd );
                //client_socket[i] = 0;
            }

            //Add them to the users or workers based on message received
            else
            {   
                //if it is worker
                if(buffer[0]=='k')
                {
                    //add new socket to array of worker sockets
                    for (i = 0; i < max_workers; i++)
                    {
                        //if position for the workers is empty
                        if( worker_socket[i] == 0 )
                        {	
                            worker_socket[i] = sd;
                            //inform user of socket number - used in send and receive commands
                            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                            printf("Adding the worker to list of sockets as %d\n" , i);
                            break;
                        }
                        //exceeded the limit of workers
                        else if(i==max_workers-1)
                        {
                            getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                            printf("exceeded the limit,worker  disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                            //Close the socket and mark as 0 in list for reuse
                            close( sd );
                            worker_socket[i] = 0;
                        }
                    }

                }
                //if incoming connection was client
                else
                {
                    //add new socket to array of sockets
                    for (i = 0; i < max_clients; i++)
                    {
                        //if position for client is empty
                        if( client_socket[i] == 0 )
                        {
                            client_socket[i] = new_socket;
                            //inform user of socket number - used in send and receive commands
                            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                            printf("Adding to list of sockets as %d\n" , i);
                            read(sd,buffer,17);
                            if(i==0){
                                for(int k=0;k<17;k++){
                                client1[k]=buffer[k];}
                            }
                            else if(i==1){
                                for(int k=0;k<17;k++){
                                client2[k]=buffer[k];}
                            }
                            else{
                                for(int k=0;k<17;k++){
                                client3[k]=buffer[k];}
                            }
                            client_alloted[i]=0;
                            break;
                        }
                        //exceeded the limit of clients
                        else if(i==max_clients-1)
                        {
                            getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                            printf("exceeded the limit,client disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                            //Close the socket and mark as 0 in list for reuse
                            close( sd );
                            client_socket[i] = 0;
                        }
                    }
                }
            }
        }

        //else its some IO operation on some other socket :)
        for (int j = 0; j < max_clients; j++)
        {
            sd = client_socket[j];
            if(client_alloted[j]==0){
            	free_workers=0;
                for (int i = 0; i < max_workers; i++){
                                sd = worker_socket[i];                              
                                if ((worker_socket[i]!=0)&&worker_client[i]==-1){                	
                	free_workers++;}
                }
                nth_worker=0;
                for (int i = 0; i < max_workers; i++)
                            {   
                                sd = worker_socket[i];                              
                                if ((worker_socket[i]!=0))
                                {   
                                    //if worker is free send the message
                                    if(worker_client[i]==-1)
                                    {   
                                        int cal;//,length1;
                                        if(j==0){
                                for(int k=0;k<17;k++){
                                client1[k]=buffer[k];
                            	bit[0]=free_workers;}
                                        }
                                        else if(j==1){
                                for(int k=0;k<17;k++){
                                client2[k]=buffer[k];
                                bit[1]=free_workers;}
                                        }
                                        else{
                                for(int k=0;k<17;k++){
                                client3[k]=buffer[k];
                                bit[2]=free_workers;}
                                        }
                                        buffer[17]=nth_worker++;
                                        buffer[18]=free_workers;
                                        cal=send(sd,buffer,19,0);
                                        length1=buffer[13]-48;
                                        //cout<<cal;
                                        if(cal==0){cout<<"error";break;}
                                        worker_client[i]=j;
                                        client_alloted[j]=1;
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                            }
            }

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 17)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Client disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[j] = 0;
                }

                //allocating the decryption work for the workers
                else
                {
                }
            }
        }
        for (i = 0; i < max_workers; i++)
        {
            sd = worker_socket[i];
           // if(sd!=0){cout<<sd<<FD_ISSET( sd , &readfds);}

            if (FD_ISSET( sd , &readfds))
            {   
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 17)) == 0)
                {   
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Worker disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    worker_socket[i] = 0;
                    worker_client[i] = -1;
                }

                //Echo back the message that came in
                else
                {
                            //read(sd,buffer,length1);
                    sd = client_socket[worker_client[i]];
                    if(worker_client[i]==0){
                                        length1=client1[13]-48;
                                        bit[0]--;
                    }
                    else if(worker_client[i]==1){
                                        length1=client2[13]-48;
                    }
                    else{
                                        length1=client3[13]-48;
                    }
                    if(buffer[0]!='$'){
                    send(sd,buffer,length1,0);
                    client_alloted[worker_client[i]]=-1;}
                    worker_client[i]=-1;
                }
            }
        }
    }
    close(master_socket);
    return 0;
}
