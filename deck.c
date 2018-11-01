#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Function: shuffle
 * --------------------
 *  Initializes deck_instance and shuffles it.
 *  Resets the deck if a prior game has been played.
 *
 *  returns: 0 if no error, and non-zero on error
 */
int shuffle() {
    deck_instance.top_card=0;//reset
    char arraySuit[4] = {'C','D','H','S'};//create array of suits
    char arrayCardNumber[13] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};//chose T for ten
    //create an array of ranks
    int index=0;//initialize index
    while(index<52){//while loop to step thru full deck
        for(int i=0;i<13;i++)//arrayCardNumber
        {
            for(int j=0;j<4;j++)//arraySuit
            {
                struct card a = { .rank = arrayCardNumber[i] , .suit = arraySuit[j] };//make each card
                deck_instance.list[index] = a;//place each card in deck
                index++;//increment index
            }
        }
    }
    //randomized deck
    srand(time(NULL));//seeds the randomizers
    int randomIndex;//will store index to use to hold random index when shuffling deck
    struct card temp;//temp to use when swapping
    for(int i=0;i<20;i++)//shuffles the deck 20 times so that is is truly shuffled
    {
        for(int j=0;j<52;j++)//shuffles the whole deck once
        {
            randomIndex = rand() % 52;
            temp = deck_instance.list[randomIndex];//swapping
            deck_instance.list[randomIndex] = deck_instance.list[j];
            deck_instance.list[j] = temp;//swapping
        }
    }
    printf("Shuffling deck...\n\n");//required print statement for desired gameplay
    return 0;//expected return
}

/*
 * Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck.
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */
int deal_player_cards(struct player* target) {
    for(int i =0; i<7;i++){
        add_card(target, next_card());
    }
    return 0;
}

/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck.
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card( ) {
    int pointer_top_card = deck_instance.top_card;//indexing
    deck_instance.top_card = deck_instance.top_card + 1;//increment index of top card
    return &deck_instance.list[pointer_top_card];//return pointer to top card
}

/*
 * Function: size
 * --------------
 *  Return the number of cards left in the current deck.
 *
 *  returns: number of cards left in the deck.
 */
size_t deck_size( ) {
    return 52 - deck_instance.top_card;//returns number of cards left
    
}
