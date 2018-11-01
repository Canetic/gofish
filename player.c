#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//function to print one card
int print_card(struct card* card_to_print)
{
    if(card_to_print==NULL){return -1;}
    printf("%c%c ", card_to_print->rank, card_to_print->suit);
    return 0;
}

//function that prints hand
int print_hand(struct player* target)
{
    struct hand* iterator;
    iterator=target->card_list;
    if(iterator==NULL){return -1;}
    do
    {
        print_card(&iterator->top);
        iterator = iterator->next;
    }
    while(iterator != NULL);
    printf("\n");
    return 0;
}

//function that prints all of a desired rank from a player's hand
int print_hand_rank(struct player* target, char rankIn)
{
    struct hand* iterator;
    iterator=target->card_list;
    if(iterator==NULL){return -1;}
    do
    {
        if(iterator->top.rank==rankIn)
            print_card(&iterator->top);
        iterator = iterator->next;
    }
    while(iterator != NULL);
    printf("\n");
    return 0;
}

//prints a players books
int print_book(struct player* target)
{
    for(int i =0; i<7;i++){
        printf("%c ",target->book[i]);
    }
    printf("\n");
    return 0;
}

/*
 * Function: add_card
 * -------------------
 *  Add a new card to the player's hand.
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card) {
    // to test out the deal_cards function works
    //printf("%c /n", new_card->rank);
    struct hand* temp;
    temp = (struct hand*)malloc(sizeof(struct hand));
    
    temp->top.rank=new_card->rank;
    temp->top.suit=new_card->suit;
    
    temp->next = target->card_list;//modify temp variable
    
    target->card_list=temp;//assign temp back to player
    target->hand_size++;//increment hand size when adding
    return 0;
}

/*
 * Function: remove_card
 * ---------------------
 *  Remove a card from the player's hand.
 *
 *  target: the target player
 *  old_card: pointer to the old card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card) {
    struct hand* iterator;
    iterator=target->card_list;
    if(iterator==NULL){return -1;}//use iterator so can check null
    //TA said not to check this for add b.c. when you malloc, it would never be null so you don't need to check
    if((iterator->top.rank == old_card->rank) && (iterator->top.suit == old_card->suit))
    {//first card
        target -> card_list = target -> card_list -> next;
        target -> hand_size -= 1;
        return 0;
    }
    while(iterator->next != NULL)
    {//second card to last card
        if((iterator->next->top.rank == old_card->rank) && (iterator->next->top.suit == old_card->suit))
        {
            iterator->next = iterator->next->next;
            target -> hand_size -= 1;
            return 0;//desired outcome
        }
        iterator = iterator->next;
    }
    free(iterator);
    return -1;
}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 *
 *  target: pointer to the player to check
 *
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 */
char check_add_book(struct player* target) {
    char arrayCardNumber[13] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};//chose T for ten
    struct hand* temp;
    temp = (struct hand*)malloc(sizeof(struct hand));
    temp = target->card_list;
    struct card a;
    struct card b;
    struct card c;
    struct card d;
    for (int i=0; i <13; i++)
    {
        int counter = 0;
        char searchRank = arrayCardNumber[i];
        while(temp!=NULL)
        {
            if (temp->top.rank == searchRank){
                counter += 1;//need to look thru whole hand for each char and increment counter each time u see it
                if(counter==1)
                    a =temp->top;//one card
                if(counter==2)
                    b =temp->top;//two card
                if(counter==3)
                    c =temp->top;//three card
                if(counter==4)
                    d =temp->top;//four AYE
                //stupid easy
            }
            
            temp = temp -> next;
        }
        if (counter == 4)
        {// then a book exists
            //printf("remove card %c %c \n",a.rank, a.suit);
            remove_card(target,&a);
            //printf("remove card %c %c \n",b.rank, b.suit);
            remove_card(target,&b);
            //printf("remove card %c %c \n",c.rank, c.suit);
            remove_card(target,&c);
            //printf("remove card %c %c \n",d.rank, d.suit);
            remove_card(target,&d);
            
            target-> book[target -> book_size] = d.rank;
            target -> book_size +=1;
            return a.rank;//useful in driver file
        }
        temp = target->card_list;
        counter=0;//reset counter
    }
    return 0;
}

/*
 * Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 */
int search(struct player* target, char rank) {
    struct hand* current = target->card_list;
    while(current != NULL){
        //testing purpose
        //printf("%c \n", current->top.rank);
        if(current->top.rank == rank){
            return 1;//player has card of that rank
        }else{
            current = current->next;
        }
    }
    return 0;//player didnt have card of that rank
}

/*
 * Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise
 *   return value indicates number of cards transferred
 */
int transfer_cards(struct player* src, struct player* dest, char rank) {
    struct card a;
    struct hand* b = src->card_list;
    while(b!=NULL)
    {
        a=b->top;
        if(search(src,rank)==1 && a.rank==rank)
        {
            add_card(dest,&a);//needed these before hand
            remove_card(src,&a);//needed these before hand to make transfer easily
        }
        b  = b -> next;
    }
    return 0;
}

/*
 * Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *
 *   Return: 1 if game is over, 0 if game is not over
 */
int game_over(struct player* target) {
    if(target -> book_size >= 7)//excellent implementation
        return 1;
    return 0;
}

/*
 * Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 *
 *   target: player to reset
 *
 *   Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target) {
    for(int i =0; i<7;i++){
        target->book[i] = '\0';//blank it
    }//goal is to clear everything for player
    target->hand_size = 0;
    target->book_size = 0;
    struct hand* temp = NULL;
    while(target->card_list != NULL){
        temp = target->card_list;
        target->card_list = temp->next;
        free(temp);
    }
    return 0;
}


char computer_play(struct player* target) {
    if(target->hand_size==0)
        return 'R';
    srand(time(NULL));//seeds the randomizers
    struct hand* current = target->card_list;
    
    int r = rand() % target->hand_size;//int between 0 and hand_size
    for(int i =0; i<r;i++){ // loop through hand_size
        current=current->next;
    }
    //printf("Player 2's turn, enter a Rank: %c",current->top.rank);
    
    return current->top.rank;
    
}

char user_play(struct player* target) {
    if(target->hand_size==0)
        return 'R';
    int x= 0;
    char pickRank;
    while(x==0){
        printf("Player 1's turn, enter a Rank: ");
        scanf(" %c", &pickRank);
        x = search(target, pickRank);
        if(x==0)
            printf("Error - must have at least one card from rank to play\n");
//print statements match desired gameplay
    }
    return pickRank;
}
