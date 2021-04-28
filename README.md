# Medianova_HT
Blackjack game made with PHP for the company Medianova.

* <ins>**Class (gameRunner) Attributes**</ins>
  - **$playerName**: Name of the player (Hardcoded).
  - **$delay**: Delay (seconds) between each round (Hardcoded).
  - **$gameResult**: Game result as a string ("Draw","Player","Dealer").
  - **$cards**: 2-D array to hold all 6 decks which players can take cards randomly from it.
  - **$playerHand**: An array to hold the player's hand.
  - **$playerHandValue**: A variable to hold the player's hand's sum.
  - **$playerHandStatus**: A variable to hold the player's hand's status (In Progress: **"IP"**, Stay: **"S"**, Blackjack: **"BL"**, Busted: **"BU"**).
  - **$dealerHand**: An array to hold the dealer's hand.
  - **$dealerHandValue**: A variable to hold the dealer's hand's sum.
  - **$dealerHandStatus**: A variable to hold the dealer's hand's status (In Progress: **"IP"**, Stay: **"S"**, Blackjack: **"BL"**, Busted: **"BU"**).
  
* <ins>**Class (gameRunner) Methods**</ins>
  - **__construct**: Initializes the player hands and all 6 decks.
  - **decodeCard**: Cards are hold as 3 digit numbers in arrays (Ex: 301 is ace of Hearts, 201 is ace of Diamonds). This function decodes this info and returns it in a cardInfo class.
  - **takeCard**: This function randomly takes a card from all 6 decks (312 cards).
  - **dealerHandCheck**: This function determines the current status of the dealer's hand as on of these: In Progress: **"IP"** (<17), Stay: **"S"** (>=17, <21), Blackjack: **"BL"** (=21), Busted: **"BU"** (>21)
  - **playerHandCheck**: This function determines the current status of the player's hand as on of these: In Progress: **"IP"** (<21), Stay: **"S"** (<21), Blackjack: **"BL"** (=21), Busted: **"BU"** (>21)
  - **setRound**: This function gives 2 cards each to both hands at the start of the round. It also determines the hand values and automatically completes the dealer's hand if the player's first hand is a blackjack.
  - **decodeValue**: Decodes the card numbers 1 to 13 to card values (1 to 10 are themselves. 11, 12 and 13 (Jack, Queen and King) are decoded as 10 (Their values are 10)).
  - **decodeName**: Decodes the card numbers 1 to 13 to card names.
  - **contDealerHand**: When the player's hand is finished, this function aims to take cards into the dealer's hand until the dealer's hand gets bigger than 17. Later on, it decides which player is the winner.
  - **playerStay**: This function runs when the stay button is pressed by the user. It saves the player's hand status as **"S"** and saves the player's hand value.
  - **playerHit**: This function runs when the hit button is pressed by the user. It adds a card to the player's hand and updates the player's hand status.
  - **printWinner**: This function prints the winner hand.
  - **printHands**: This function prints both hands and also buttons.

## Possible Improvements
* The hit button works fast but after the player's hand is completed, the system responds quite slowly for completing the dealer's hand and determining the winner. **An optimization is required.**
* Front end design is poor. echo function prints results cumulatively. **Efficient ways of using PHP with HTML should be studied.**
* PHP's sleep() function is not working. **Delay between the rounds is not working.**
* No debugger is used and unit/integration tests applied. **Error handling can be improved.**
