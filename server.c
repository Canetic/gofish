
/*
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include <stdio.h>
#include "csapp.h"
#include "deck.h"
#include "player.h"
#include <string.h>

typedef int bool;
#define true 1
#define false 0

void *thread(void *vargp);

// our gofish main
void echo(int connfd)
{
    char flag[MAXLINE] = "@$ \n";
    char flag2[MAXLINE] = "?$ \n";
    char endFlag[MAXLINE] = ")$ \n";
    char buf[MAXLINE];
    rio_t rio;
    int n;
    
    bool play_again = true;//see if they want to play again
    bool answered;//see if they answered question
    bool playerTurn = true; //determines which player goes true user false computer
    Rio_readinitb(&rio, connfd);
    
    char checkingBook;
    
    char userPick;
    int searchBool =1;
    char userCheck = 'a';
    
    printf("@Player1 joined the game \nReady to start!\n \n");
    while(play_again==true)
        //printf("%d \n",play_again);
    {
        //reset players before game
        reset_player(&user);
        reset_player(&computer);
        
        //shuffle cards
        shuffle();
        
        strcpy(buf, "Shuffling deck...\n");
        Rio_writen(connfd, buf, strlen(buf));
        
        //new line is crucial
        strcpy(buf, "\n");
        Rio_writen(connfd, buf, strlen(buf));
        
        //deal cards to players
        deal_player_cards(&user);
        deal_player_cards(&computer);
        
        //printf("asdasd %d \n", user.book_size);
        
    end:while(game_over(&user)!=1 && game_over(&computer)!=1)
    {
        if(playerTurn)
        {
            searchBool =1;
            while(searchBool==1)
            {
                print_hand_user(buf);
                Rio_writen(connfd, buf, strlen(buf));
                /*
                 print_hand_computer(buf);
                 Rio_writen(connfd, buf, strlen(buf));
                 */
                print_book_user(buf);
                Rio_writen(connfd, buf, strlen(buf));
                
                print_book_computer(buf);
                Rio_writen(connfd, buf, strlen(buf));
                
                
            reverse:
                Rio_writen(connfd, flag, strlen(flag));
                Rio_readlineb(&rio, buf, strlen(buf));
                userPick = buf[0];
                
                //store in variable to use later
                userCheck = user_play_check1(&user, &userPick, buf);
                
                
                if(userCheck=='R'){
                    Rio_writen(connfd, buf, strlen(buf));
                    goto reverse;
                }
                if(userCheck=='|')
                    searchBool=0;
                
                searchBool = search(&computer,userPick);
                
                if(searchBool==1){
                    transfer_cards(&computer, &user, userPick);
                }
                
                if(searchBool==1)
                {
                    //printf("    - Player 2 has ");
                    strcpy(buf, "    - Player 2 has ");
                    print_hand_rank1(&computer, userPick, &buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "    - Player 1 has ");
                    //printf("\n    - Player 1 has ");
                    print_hand_rank1(&user, userPick, buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    
                    strcpy(buf, "    - Player 1 books ");
                    // printf("\n    - Player 1 books ");
                    print_book1(&user, buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "    - Player 1 gets another turn \n");
                    // printf("    - Player 1 gets another turn");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "...\n");
                    //  printf("\n\n...\n\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                }
            }
            checkingBook=check_add_book(&user);
            if(game_over(&user)==1)
            {
                //game over
                //display results
                strcpy(buf, "\n");
                Rio_writen(connfd, buf, strlen(buf));
                strcpy(buf, "    -Player 1 books ");
                print_book1(&user, buf);
                Rio_writen(connfd, buf, strlen(buf));
                strcpy(buf, "    -Player 2 books ");
                print_book1(&computer, buf);
                Rio_writen(connfd, buf, strlen(buf));
                sprintf(buf, "    Player 1 Wins! %d-%d \n",user.book_size,computer.book_size);
                Rio_writen(connfd, buf, strlen(buf));
                
                goto end;
                
            }
            
            if(searchBool==0)
            {//has no
                
                //    printf("    - Player 1 has no %c's \n", userPick);
                strcpy(buf, "    - Player 1 has no ");
                strcat(buf, &userPick);
                strcat(buf, "'s \n");
                Rio_writen(connfd, buf, strlen(buf));
                
                //    printf("    - Go Fish, Player 1 draws %c%c\n    - Player 2's turn \n \n",(user.card_list)->top.rank,(user.card_list)->top.suit);
                strcpy(buf, "    - Go Fish, Player 1 draws ");
                //strncat(buf, &((user.card_list)->top.rank),1);
                //strncat(buf, &((user.card_list)->top.suit),1);
                strcat(buf, "\n");
                Rio_writen(connfd, buf, strlen(buf));
                
                
                strcpy(buf,"    - Player 2's turn \n ");
                Rio_writen(connfd, buf, strlen(buf));
                
                strcpy(buf,"\n");
                Rio_writen(connfd, buf,strlen(buf));
            }
            
            if(deck_instance.top_card<=51)
                add_card(&user, next_card());
            playerTurn = false;
        }
        else
        {
            searchBool=1;
            while(searchBool==1)
            {
                print_hand_user(buf);
                Rio_writen(connfd, buf, strlen(buf));
                
                /*
                 print_hand_computer(buf);
                 Rio_writen(connfd, buf, strlen(buf));
                 */
                print_book_user(buf);
                Rio_writen(connfd, buf, strlen(buf));
                
                print_book_computer(buf);
                Rio_writen(connfd, buf, strlen(buf));
                
                //computer playing
                userPick=computer_play(&computer);
                
                userCheck = user_play_check1(&computer, &userPick, buf);
                
                
                if(userCheck=='|' || userPick == 'R'){
                    searchBool=0;
                    goto jump;
                }
                //store search return to use later
                searchBool = search(&user,userPick);
                
                //printf("Player 2's turn, enter a Rank: %c \n", userPick);
                strcpy(buf,"Player 2's turn, enter a Rank: ");
                strncat(buf, &userPick, 1);
                strcat(buf, "\n");
                Rio_writen(connfd, buf, strlen(buf));
                
                if(searchBool==1){
                    //now transfer cards
                    transfer_cards(&user, &computer, userPick);
                }
                if(searchBool==1)//has
                {
                    strcpy(buf, "    - Player 1 has ");
                    //printf("    - Player 1 has ");
                    print_hand_rank1(&user, userPick, buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "    - Player 2 has ");
                    // printf("    - Player 2 has ");
                    print_hand_rank1(&computer, userPick, &buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "    - Player 2 books ");
                    //  printf("    - Player 2 books ");
                    print_book1(&computer, buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "    - Player 2 gets another turn \n");
                    // printf("    - Player 2 gets another turn");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "...\n");
                    //  printf("\n\n...\n\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    strcpy(buf, "\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                }
                
                //store in variable for later use
                checkingBook=check_add_book(&computer);
                if(game_over(&computer)==1)
                {
                    //display results
                    //game over
                    strcpy(buf, "\n");
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    //    printf("    - Player 2 books ");
                    strcpy(buf, "    -Player 2 books ");
                    print_book1(&computer, buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    //    printf("    - Player 1 books ");
                    strcpy(buf, "    -Player 1 books ");
                    print_book1(&user, buf);
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    sprintf(buf,"    Player 2 Wins! %d-%d \n",computer.book_size,user.book_size);
                    
                    Rio_writen(connfd, buf, strlen(buf));
                    
                    goto end;
                }
            }
            //printf("%d \n", searchBool);
        jump:
            if(searchBool==0)
            {//has no
                //printf("    - Player 1 has no %c's \n", userPick);
                strcpy(buf, "    - Player 1 has no ");
                strncat(buf, &userPick,1);
                strcat(buf, "'s \n");
                Rio_writen(connfd, buf, strlen(buf));
                
                //printf("    - Go Fish, Player 2 draws %c%c\n    - Player 1's turn \n \n",(computer.card_list)->top.rank,(computer.card_list)->top.suit);
                strcpy(buf, "    - Go Fish, Player 2 draws ");
                //strncat(buf, &((computer.card_list)->top.rank),1);
                //strncat(buf, &((computer.card_list)->top.suit),1);
                strcat(buf, "\n");
                Rio_writen(connfd, buf, strlen(buf));
                
                //then its player 1's turn
                strcpy(buf,"    - Player 1's turn \n");
                Rio_writen(connfd, buf, strlen(buf));
                
                strcpy(buf,"\n");
                Rio_writen(connfd, buf, strlen(buf));
            }
            
            //check if need to add card
            if(deck_instance.top_card<=51)
                add_card(&computer, next_card());
            playerTurn = true;
            
            searchBool=1;
        }
    }
        answered=true;
        //determine if they want to play again
        while(answered==true)
        {
            char choice ='\0';
            strcpy(buf, "Do you want to play again [Y/N]: \n");
            Rio_writen(connfd, buf, strlen(buf));
            Rio_writen(connfd, flag2, strlen(flag2));
            
            Rio_readlineb(&rio, buf, strlen(buf));
            choice = buf[0];
            if(choice=='Y')
            {
                //need to set booleans so can play again
                play_again = true;
                answered = false;
                sprintf(buf, "ending \n");
                Rio_writen(connfd, buf, strlen(buf));
            }
            
            else if(choice=='N')
            {
                //need to set booleans to not play again
                play_again = false;
                answered = false;
                // printf("Exiting \n");
                sprintf(buf, "Exiting \n");
                Rio_writen(connfd, buf, strlen(buf));
                Rio_writen(connfd, endFlag, strlen(endFlag));
            }
            else
            {
                answered = true;
            }
            //printf("ending \n");
            
        }
        
        
    }
    
}

int main(int argc, char **argv)
{
    //MAIN VARIABLES
    
    //file descriptors
    int listenfd, *connfdp;
    socklen_t clientlen;
    
    //struct
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    
    //check # of args
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
    printf("$./server %s \n", argv[1]);
    
    //loop to keep server running until quit
    while (1) {
        clientlen=sizeof(struct sockaddr_storage);
        connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
        *connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
        Pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* Thread routine */
void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    echo(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */


//HELPER FUNCTIONS
int print_turn(char *buf)
{
    print_hand_user(buf);
    print_hand_computer(buf);
    print_book_user(buf);
    print_book_computer(buf);
}

int print_hand_user(char *buf){
    // printf("Player 1's Hand - ");
    strcpy(buf, "Player 1's Hand - ");
    print_hand1(&user, buf);
    return 0;
}

int print_hand_computer(char *buf)
{
    // printf("Player 2's Hand - ");
    strcpy(buf, "Player 2's Hand - ");
    print_hand1(&computer, buf);
    return 0;
}

int print_book_user(char *buf)
{
    // printf("Player 1's Book - ");
    strcpy(buf, "Player 1's Book - ");
    print_book1(&user, buf);
    return 0;
}

int print_book_computer(char *buf)
{
    // printf("Player 2's Book - ");
    strcpy(buf, "Player 2's Book - ");
    print_book1(&computer, buf);
    return 0;
}
© 2019 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About
