/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include <stdio.h>
#include "csapp.h"
#include "deck.h"
#include "player.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    char flag[MAXLINE] = "@$ \n";
    char flag2[MAXLINE] = "?$ \n";
    char endFlag[MAXLINE] = ")$ \n";
    rio_t rio;
    char userInput = 'P';
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    //store host and port as entered on client
    host = argv[1];
    port = argv[2];
    
    
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    printf("$./client %s \n", host);
    while (1) {
        Rio_readlineb(&rio, buf, MAXLINE);
        
        if(strcmp(buf, &flag)==0){
            //print statement asking for Rank
            printf("Player 1's turn, enter a Rank: ");
            //scan for user input
            scanf(" %c", &userInput);
            strncpy(buf, &userInput, MAXLINE);
            strcat(buf, "\n");
            //use Rio function
            Rio_writen(clientfd, buf, strlen(buf));
        }else if(strcmp(buf, &flag2)==0){
            scanf(" %c", &userInput);
            strncpy(buf, &userInput, MAXLINE);
            strcat(buf, "\n");
            Rio_writen(clientfd, buf, strlen(buf));
        }
        else if(strcmp(buf, &endFlag)==0){
            goto end;
        }
        else if(strcmp(buf, &flag)!=0){
            printf("%s", buf);
        }
        
    }
    
end:
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
