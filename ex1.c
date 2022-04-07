#include <stdio.h>
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h> 
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/wait.h>


#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "127.0.0.1"
int main()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
    char input[256];
    char address[256];
    getcwd(address, 256);
    int boolean=0;
    int st= dup(STDOUT_FILENO);
    while (1)
    {
        if (boolean){
          dup2(st, STDOUT_FILENO);
       }
        printf("yes master?\n");
         if (boolean){
         dup2(sock, STDOUT_FILENO);
        }
        fgets(input, sizeof(input), stdin);  // read string
        int index1=0;
        while (input[index1]!='\n'){
          index1=index1+1;
       }
       //To exit the program enter EXIT
        input[index1]='\0';
        if (strncmp(input, "EXIT ", 4) == 0)
        {
            return 0;
        }
        //2. To get the current address enter getcwd.
        else if (strncmp(input, "getcwd ", 6) == 0){
        getcwd(address, 256);
        printf("%s\n", address);
        }
        //To connect to server you must run ./tcp_server and enter TCP PORT
        else if (strncmp(input, "TCP PORT ", 8) == 0)
        {
           boolean=1;
            if (sock == -1)
            {
                printf("Could not create socket : %d", errno);
            }
            struct sockaddr_in serverAddress;
            memset(&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(SERVER_PORT);
            int rval = inet_pton(AF_INET, (const char *)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
            if (rval <= 0)
            {
                printf("inet_pton() failed");
                return -1;
            }
            if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
            {
                printf("connect() failed with error code : %d", errno);
            }

            printf("connected to server\n");
            dup2(sock, STDOUT_FILENO);

        }
        //To print to the screen enter ECHO and the required
        else if (strncmp(input, "ECHO", 4) == 0) {
             
            int index=5;
            while (input[index]!='\0')
            {
                printf("%c",input[index]);
                index=index+1;
            }
            
            printf("\n");
            }
            //To disconnect from the server enter LOCAL(the socket server also close).
            else if (strncmp(input, "LOCAL", 5) == 0)
            {      
                    send(sock , input, strlen(input) , 0);
                    close(sock);
                    dup2(st, STDOUT_FILENO);         
                    boolean=0;
                    
                    
            }
            //To view the list of files from the current directory enter DIR
            else if (strncmp(input, "DIR", 3) == 0) {
                DIR *d;
             struct dirent *s;
               d  = opendir(address);
                while ((s = readdir(d)) != NULL)
                {
                     printf("%s  ", s->d_name);
                   
                }
                printf("\n");
                closedir(d);
                
            }
           /*To move to a directory that is in the current directory enter CD and the name of the directory.
             The chdir function is a system function used to modify a current work directory*/
            else if (strncmp(input, "CD", 2) == 0) {
               char dir[256];
             int k = 0;
              dir[k++] = '/';
            int index=3;
             while (input[index]!='\0')
              {
                  dir[k]=input[index];
                 index++;
                 k++;
               }
               dir[k]='\0';
                char dir1[256];
               strcpy(dir1, dir);
                strcat(address, dir1);
                if (chdir(address))
                {

                    printf("dir not exist\n");
                    
                }

            }
             /*To copy the file that appears in src to dst you have to enter COPY and the file src , file dest.
             fread, fwrite, fopen Are library functions*/
            else if (strncmp(input, "COPY", 4) == 0){
                char src [256];
                char dest [256];
                int k = 0;
                int k1 = 0;
                int i=5;
                while (input[i]!=' ')
                {
                  src [k]=input[i];  
                  i++;
                  k++; 
                }
                i++;
              while(input[i] != '\0'){
                 dest [k1]=input[i];  
                  i++;
                  k1++;   
              } 
                dest[k1] = '\0';
                char destcopy[256];
                src[k] = '\0';
                char srccopy[256];
                strcpy(destcopy, dest);
                strcpy(srccopy, src);
                printf("%s",destcopy);
                printf("%s",srccopy);
               FILE *s = fopen(srccopy, "r+");
               if(s==NULL){
                   printf("%s\n",strerror(errno));
                   continue;
               }
               FILE *d  = fopen(destcopy,"w+");
                  if(d==NULL){
                   printf("%s\n",strerror(errno));
                   continue;
               }
               char cTemp;
              while(fread(&cTemp, 1, 1, s) == 1){
                fwrite(&cTemp, 1, 1, d);
               }

                fclose(s);
                fclose(d);
            
            }
            /*To delete the file enter DELETE and the name of file
              unlink a system function is fundeletes the link named by pathname and decrements the link count for the file itself.*/
            else if (strncmp(input, "DELETE", 6) == 0)
            {
                char filename[256];
                int index = 0;
                filename[index] = '/';
                index++;
                int index2=7;
                while (input[index2]!='\0')
                {
                filename[index] = input[index2];
                index2++;
                index++;
                }
                filename[index] = '\0';
                char filename1[256];
                strcpy(filename1, filename);
                strcat(address, filename1);
                if (unlink(address))
                {
       
                    printf("file not exist\n");
                }
                else
                {

                    printf("file delete\n");
                
                }
            }
            else
            {
               /*
               system is a library function,
               The system function is part of the standard command transfer directory that can be executed in the command processor
               or in the operating system terminal,and finally returns the command after its completion.*/
             // system(input);
           char token2[]="/bin/";

           strcat(token2,input);
           pid_t  pid = fork();
            if (pid < 0) {
            /* error occurred */
            return 1;
            } else if (pid == 0) {
            /* child process */
           execlp(token2, input, NULL); // A new program(ls executable is loaded into memory and executed


           } else {
            /* parent process */
            /* parent will wait for the child to complete */
            wait(NULL);
           }
               }

    }
      return 0; 
    }    
