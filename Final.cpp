#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

class CARD
{
	private:
		string suiteName;
		string faceCard;
		int cardValue;
	public:
		CARD();
		~CARD();
		void setSuiteName(int);
		string getSuiteName();
		void setCardValue(int);
		int getCardValue();
		void setAceValue(int);
		string getFaceCard();
		void CreateCard(int, int);
		
};

CARD::CARD()
{
 	suiteName = "NULL";
 	faceCard = "NULL";
    cardValue = 0;
}

CARD::~CARD() {}

void CARD::CreateCard(int suite, int cardsInSuite)
{
	setSuiteName(suite);
	setCardValue(cardsInSuite);
}

void CARD::setSuiteName(int suit)
{
	switch(suit)
	{
		case 1: suiteName = "Hearts";
				break;
		case 2: suiteName = "Diamonds";
				break;
		case 3: suiteName = "Clubs";
				break;
		case 4: suiteName = "Spades";
				break;
	}
}

string CARD::getSuiteName()
{
	return suiteName;
}

void CARD::setCardValue(int cardValueArgument)
{
	if(cardValueArgument == 1)
	{
		faceCard = "Ace";
		cardValue = 11;
	}
	
	else if(cardValueArgument == 11 || cardValueArgument == 12 || cardValueArgument == 13 )  // JACKS QUEENS AND KINGS ALL HAVE A VALUE OF 10
	{
		if(cardValueArgument == 11)
		{
			faceCard = "Jack";
		}
		
		else if(cardValueArgument == 12)
		{
			faceCard = "Queen";
		}
		
		else if(cardValueArgument == 13)
		{
			faceCard = "King";
		}
		
		cardValue = 10;
	}
	
	else
	{
		cardValue = cardValueArgument;
	}
}

void CARD::setAceValue(int newAceValue)
{
	cardValue = newAceValue;
}

int CARD::getCardValue()
{
	return cardValue;
}

string CARD::getFaceCard()
{
	return faceCard;
}

class SCORES
{
	private:
		int score;
		CARD gamerDeck[10];   // Cards in current play
		int gamerDeckIndex;
        int wins;		
	public:
	    SCORES();
	    ~SCORES();
		void setScore(CARD*);
		int getScore();
		void hitScore(CARD*);
		void printGamerDeckDetails(int);
		void setWin(bool);
		int getWin();
		void resetScoreAndIndex(int, int);
};

SCORES::SCORES()
{
	score = 0;
	wins = 0;
}

SCORES::~SCORES() {}

void SCORES::setScore(CARD* card)
{
	gamerDeck[gamerDeckIndex] = *card;      // stored locally for ace check and output.
		
	score += (card->getCardValue());
	
	if(score>21)
	{
		if(gamerDeck[gamerDeckIndex].getCardValue() == 11)
		{
			gamerDeck[gamerDeckIndex].setAceValue(1);
			score -= 10;
		}
		
		else if(gamerDeck[gamerDeckIndex + 1].getCardValue() == 11)
		{
			gamerDeck[gamerDeckIndex + 1].setAceValue(1);
			score -= 10;
		}
		
	}
	
	gamerDeckIndex++;
}

void SCORES::hitScore(CARD* nextCard)
{
	gamerDeck[gamerDeckIndex] = *nextCard;
	score += (nextCard->getCardValue());
	bool doOnce = true;                   //Makes sure only one ace is adjusted at a time and not all.

	if(score > 21)
	{
		for(int x = 0; x <= gamerDeckIndex; x++)
		{
			if(gamerDeck[x].getCardValue() == 11 && doOnce==true)             //ADJUST ACES
			{
				gamerDeck[x].setAceValue(1);
				score -= 10;
				doOnce == false;
			}
		}
	}
													
	gamerDeckIndex++;
}

int SCORES::getScore()
{
	return score;
}

void SCORES::printGamerDeckDetails(int index)
{
	if(gamerDeck[index].getFaceCard() == "NULL")
	{
		cout << gamerDeck[index].getCardValue() << " of " << gamerDeck[index].getSuiteName();
	}
	
	else if(gamerDeck[index].getFaceCard() != "NULL" || gamerDeck[index].getFaceCard() == "Ace")
	{
		cout <<  gamerDeck[index].getFaceCard() << " of " << gamerDeck[index].getSuiteName();
	}
}

void SCORES::setWin(bool winOrNot)
{
    if(winOrNot)
    {
        wins++;
    }
}

int SCORES::getWin()
{
    return wins;
}

void SCORES::resetScoreAndIndex(int newScore, int newIndex)
{
	score = newScore;
	gamerDeckIndex = newIndex;
}

CARD* Shuffle(CARD deckArgument[], CARD discardArgument[], int cardsInDiscard)
{
	random_shuffle(&discardArgument[0], &discardArgument[cardsInDiscard]);  //SHUFFLE THE DISCARDS
	
	for(int i= 0; i <= cardsInDiscard; i++)
	{
		deckArgument[i] = discardArgument[i];    //ASSIGN SHUFFLED DISCARD TO BE THE MAIN DECK
	}
		
	return deckArgument;
}

int main() 
{
    srand(time(0));
    int numOfGames = 0;
    
    bool continueGame = true;
    bool continueSession = true;
    char keepPlaying;
    
    CARD deck[52];
    CARD discardDeck[52];
    
	int indexCounterForDeck = 51;
	int cardsInDeck = 51;
    int cardsInDiscard = 0;
    
	int* pindexCounterForDeck = &indexCounterForDeck;		
    int* pcardsInDeck = &cardsInDeck;                       	//POINTERS TO RESPECTIVE VARIABLES TO CHANGE VARIABLES WHEN THEY GO OUT OF SCOPE       
    int* pcardsInDiscard = &cardsInDiscard;  

    CARD* pdeck = & deck[52];     	                               
    
    SCORES playerScore;
    SCORES dealerScore;
    	
	int deckCount = 0;
    for(int suites = 1; suites <= 4; suites++)
    {
        for(int cardValue = 1; cardValue <= 13; cardValue++)
        {
            deck[deckCount].CreateCard(suites, cardValue);
            deckCount++;
    	}
    }
		    
    random_shuffle(&deck[0], &deck[51]);                      //Shuffles Deck
    
  
  						//---------------------------------//GAME STARTS HERE//-------------------------------------//
        
    do
    {   //START POINT OF A ROUND 
        cout << "\n<================================BLACKJACK====================================>\n\n";
        
		//DEAL PLAYER (BEGINNING OF EVERY ROUND)
        { 
			playerScore.resetScoreAndIndex(0,0);
			 
			for(int dealPlayer = 0; dealPlayer < 2; dealPlayer++)
			{
				if(cardsInDeck == 0)  //IF THERE ARE NO CARDS TO DEAL AND EVERY CARD IS IN DISCARD PILE
            	{
            		pdeck = Shuffle(deck, discardDeck, cardsInDiscard);  //NEW RE-SHUFFLED DECK
            		(*pindexCounterForDeck) = (*pcardsInDeck) = (*pcardsInDiscard); //UPDATE NEW DECK CARD COUNT
					(*pcardsInDiscard) = 0;  //NO MORE CARDS IN DISCARD
				}
				
				playerScore.setScore(&deck[indexCounterForDeck-cardsInDeck]);
            	discardDeck[cardsInDiscard] = deck[indexCounterForDeck-cardsInDeck];
            	
				(*pcardsInDeck)--;
            	(*pcardsInDiscard)++;
    		}
            
		    cout <<"<Player>" << endl;                      
            for(int i = 0; i<2; i++)
            {
            	cout << "Card " << (i+1) << ": ";
                playerScore.printGamerDeckDetails(i);
                cout << endl;
            }
            cout << "\t\t\t\t\t\tThe players score is: " << playerScore.getScore() << endl;
        }
    
		//DEAL DEALER (BEGINNING OF EVERY ROUND)
        {
           dealerScore.resetScoreAndIndex(0,0);
           
           for(int dealDealer = 0; dealDealer < 2; dealDealer++)
			{
				if(cardsInDeck == 0)  // IF THERE ARE NO CARDS TO DEAL AND EVERY CARD IS IN DISCARD PILE
            	{
            		pdeck = Shuffle(deck, discardDeck, cardsInDiscard);
            		(*pindexCounterForDeck) = (*pcardsInDeck) = (*pcardsInDiscard);
					(*pcardsInDiscard) = 0;
				}
				
				dealerScore.setScore(&deck[indexCounterForDeck-cardsInDeck]);
            	discardDeck[cardsInDiscard] = deck[indexCounterForDeck-cardsInDeck];
            	
				(*pcardsInDeck)--;
            	(*pcardsInDiscard)++;
        	}
            
			cout <<"\n<Dealer>" << endl;
			for(int i = 0; i<2; i++)
            {
            	cout << "Card " << (i+1) << ": ";
            	dealerScore.printGamerDeckDetails(i);
            	cout << endl;
            }
           
            cout << "\t\t\t\t\t\tThe dealers score is: " << dealerScore.getScore() << endl;
        }
        
        //HIT OR STAND PHASE (A.K.A : GAMEPLAY)
        {	bool continueLoop = true;
             
            if(playerScore.getScore() == 21 || dealerScore.getScore() == 21) 
            {
                continueLoop = false;
            }
            
            while(continueLoop == true)
            {
                char hitOrStandResponse;
                
				do
                {
                    cout << "\n\nWould you like to hit or stand?(H/h or S/s): ";
                    cin >> hitOrStandResponse;
                    
                }while(hitOrStandResponse != 'H' && hitOrStandResponse != 'h' && hitOrStandResponse != 'S' && hitOrStandResponse != 's');
                
                
                if(hitOrStandResponse == 'H' || hitOrStandResponse == 'h')
                {
                	if(cardsInDeck == 0)
	            	{
	            		pdeck = Shuffle(deck, discardDeck, cardsInDiscard);
	            		(*pindexCounterForDeck) = (*pcardsInDeck) = (*pcardsInDiscard);
						(*pcardsInDiscard) = 0;
					}
                	
                    playerScore.hitScore(&deck[indexCounterForDeck-cardsInDeck]);
                    cout << "\nThe new card for the player is: " << (deck[indexCounterForDeck - cardsInDeck].getCardValue()) << " of "<< (deck[indexCounterForDeck - cardsInDeck]).getSuiteName() << endl;
                    discardDeck[cardsInDiscard] = deck[indexCounterForDeck-cardsInDeck];
                    (*pcardsInDiscard)++;
                    (*pcardsInDeck)--;
                    cout << "\t\t\t\t\t\tThe players score is: " << playerScore.getScore() << endl;
                    cout << "\t\t\t\t\t\tThe dealers score is: " << dealerScore.getScore() << endl;
                    if(playerScore.getScore() >= 21) 
                    {
                        continueLoop = false;
                    }
                }
                
                else if(hitOrStandResponse == 'S' || hitOrStandResponse == 's')
                {
                    if(cardsInDeck == 0)
	            	{
	            		pdeck = Shuffle(deck, discardDeck, cardsInDiscard);
	            		(*pindexCounterForDeck) = (*pcardsInDeck) = (*pcardsInDiscard);
						(*pcardsInDiscard) = 0;
					}
					
					if(dealerScore.getScore() > 16 || dealerScore.getScore() >= 21 )  //CONDITIONS IN FAVOR OF PLAYER
                    {
                    	cout << "\t\t\t\t\t\tThe players score is: " << playerScore.getScore() << endl;
                        cout << "\t\t\t\t\t\tThe dealers score is: " << dealerScore.getScore() << endl;
                        continueLoop = false;
                    }
					
					while(dealerScore.getScore() <= 16)
                    {
                    	if(cardsInDeck == 0)
		            	{
		            		pdeck = Shuffle(deck, discardDeck, cardsInDiscard);
		            		(*pindexCounterForDeck) = (*pcardsInDeck) = (*pcardsInDiscard);
							(*pcardsInDiscard) = 0;
						}
                    	
                        dealerScore.hitScore(&deck[indexCounterForDeck-cardsInDeck]);
                        cout << "\nThe new card for the dealer is: " << (deck[indexCounterForDeck - cardsInDeck].getCardValue()) << " of "<< (deck[indexCounterForDeck - cardsInDeck]).getSuiteName() <<endl;
                        discardDeck[cardsInDiscard] = deck[indexCounterForDeck-cardsInDeck];
                        (*pcardsInDiscard)++;
    					(*pcardsInDeck)--;
    					cout << "\t\t\t\t\t\tThe players score is: " << playerScore.getScore() << endl;
                        cout << "\t\t\t\t\t\tThe dealers score is: " << dealerScore.getScore() << endl;
                    }
                    
					if(dealerScore.getScore() > 16 || dealerScore.getScore() >= 21 )
                    {
                    	continueLoop = false;
					}
                }
            }
            		        
            //END OF A SINGLE SESSION
            
            if(playerScore.getScore() == dealerScore.getScore())
            {
                cout << "\n\n                    <----------The dealer wins-----------> ";
                dealerScore.setWin(true);
                playerScore.setWin(false);
            }
            
            else if( ( ( playerScore.getScore() > dealerScore.getScore() ) && playerScore.getScore() <= 21 ) || dealerScore.getScore() > 21)
            {
                cout << "\n\n                    <----------The player wins-----------> ";
                dealerScore.setWin(false);
                playerScore.setWin(true);
            }
            
            else if( ( (playerScore.getScore() < dealerScore.getScore() ) && dealerScore.getScore() <= 21 ) || playerScore.getScore() > 21)
            {
                cout << "\n\n                   <----------The dealer wins-----------> ";
                dealerScore.setWin(true);
                playerScore.setWin(false);
            } 
        
        	numOfGames++;
        	cout << "\n\n <== Dealer Wins: " << dealerScore.getWin() << " ==> " << " <== Player Wins: " << playerScore.getWin() << " ==> "<< " <== Number of Games: "<< numOfGames << "  ==>" << endl;
    	}
		
     	//CONTINUE GAME?
        do
        {
            cout <<"\n\nWould you like to play again?(Y/y or N/n): ";
            cin >> keepPlaying;
            
            if( keepPlaying != 'Y' &&  keepPlaying != 'y' && keepPlaying != 'N' && keepPlaying != 'n')
            {
                cout <<"\nError. Try Again." << endl;
            }
            
            if(keepPlaying == 'Y' || keepPlaying == 'y')
            {
                continueGame = true;
            }
            
            else if(keepPlaying == 'N' || keepPlaying == 'n')
            {
                continueGame = false;                                              					//TERMINATES ENTIRE PROGRAM IF N/n
            }
                
        }
        while(keepPlaying != 'Y' && keepPlaying != 'y' && keepPlaying != 'N' && keepPlaying != 'n');           
            
    }
    while(continueGame == true);

	return 0;
}

