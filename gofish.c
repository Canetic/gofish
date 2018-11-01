#include <stdio.h>
#include "deck.h"
#include "player.h"

typedef int bool;
#define true 1
#define false 0

int print_hand_user();
int print_hand_computer();
int print_book_user();
int print_book_computer();
int print_turn();

int main()
{
    bool play_again = true;//see if they want to play again
    bool answered;//see if they answered question
    bool playerTurn = true; //determines which player goes true user false computer
    
    while(play_again==true)
    {
        char checkingBook;
        reset_player(&user);
        reset_player(&computer);
        shuffle();
        deal_player_cards(&user);
        deal_player_cards(&computer);
        
    end:while(game_over(&user)!=1 && game_over(&computer)!=1)
    {//this loop is for one game
        
        int searchBool =1;
        char userPick;
        if(playerTurn){
            while(searchBool==1){
                print_turn();
                userPick=user_play(&user);
                if(userPick=='R')
                    break;
                searchBool = search(&computer,userPick);
                if(searchBool==1){
                    transfer_cards(&computer, &user, userPick);
                }
                if(searchBool==1)//has
                {
                    printf("    - Player 2 has ");
                    print_hand_rank(&computer, userPick);
                    printf("    - Player 1 has ");
                    print_hand_rank(&user, userPick);
                    printf("    - Player 1 books ");
                    print_book(&user);
                    printf("    - Player 1 gets another turn");
                    printf("\n\n...\n\n");
                }
                checkingBook=check_add_book(&user);
                if(game_over(&user)==1)
                {
                    printf("Player 2 Wins! %c-%c\n",user.book_size,computer.book_size);
                    goto end;
                }
            }
 
            if(searchBool==0)
            {//has no
                printf("    - Player 2 has no %c's \n", userPick);
                printf("    - Go Fish, Player 1 draws %c%c\n    - Player's 2 turn \n \n",(user.card_list)->top.rank,(user.card_list)->top.suit);
            }
            
            if(deck_instance.top_card<=51)
                add_card(&user, next_card());
            playerTurn = false;
        }
        
        else{
            while(searchBool==1){
                print_turn();
                userPick=computer_play(&computer);
                if(userPick=='R')
                    break;
                searchBool = search(&user,userPick);
                printf("Player 2's turn enter a Rank: %c \n", userPick);
                if(searchBool==1){
                    transfer_cards(&user, &computer, userPick);
                }
                if(searchBool==1)//has
                {
                    printf("    - Player 1 has ");
                    print_hand_rank(&user, userPick);
                    printf("    - Player 2 has ");
                    print_hand_rank(&computer, userPick);
                    printf("    - Player 2 books ");
                    print_book(&computer);
                    printf("    - Player 2 gets another turn");
                    printf("\n\n...\n\n");
                }
                checkingBook=check_add_book(&computer);
                if(game_over(&computer)==1)
                {
                    printf("Player 2 Wins! %c-%c \n",computer.book_size,user.book_size);
                    goto end;
                }
            }
            if(searchBool==0)
            {//has no
                printf("    - Player 1 has no %c's \n", userPick);
                printf("    - Go Fish, Player 2 draws %c%c\n    - Player's 1 turn \n \n",(computer.card_list)->top.rank,(computer.card_list)->top.suit);
            }
            if(deck_instance.top_card<=51)
                add_card(&computer, next_card());
            playerTurn = true;
        }
        searchBool=1;
        
    }
        answered=true;
        //determine if they want to play again
        while(answered==true)
        {
            char choice ='\0';
            printf("Do you want to play again [Y/N]: \n");
            scanf(" %c", &choice);
            if(choice=='Y')
            {
                play_again = true;
                answered = false;
            }
            
            else if(choice=='N')
            {
                play_again = false;
                answered = false;
                printf("Exiting \n");
            }
            else
            {
                answered = true;
            }
            
            printf("ending \n");
            
        }
        
        
        
    }//end of play_again while loop
    
}//end of main

int print_turn()
{
    print_hand_user();
    //print_hand_computer();
    print_book_user();
    print_book_computer();
}

int print_hand_user(){
    printf("Player 1's Hand - ");
    print_hand(&user);
    return 0;
}

int print_hand_computer()
{
    printf("Player 2's Hand - ");
    print_hand(&computer);
    return 0;
}

int print_book_user()
{
    printf("Player 1's Book - ");
    print_book(&user);
    return 0;
}

int print_book_computer()
{
    printf("Player 2's Book - ");
    print_book(&computer);
    return 0;
}
