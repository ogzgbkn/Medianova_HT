<?php

    class cardInfo{                                                                             // CardInfo class holds card type and card value

        public $cardNum;                                                                        // Number of the card (1 to 13)
        public $cardType;                                                                       // Type of the card (Clubs, Diamonds, Hearts, Spades)

        function __construct(string $newCardType, int $newCardNum){                             // A constructor to set the attributes

            $this->cardNum = $newCardNum;
            $this->cardType = $newCardType;
        }
    }

    class gameRunner{

        public $playerName;                                                                     // Player name
        public $delay;                                                                          // Delay for the next round    
        public $gameResult;                                                                     // A variable to hold game result(Player Win, Dealer Win, Draw)

        public $cards;                                                                          // 2 Dimensional array to hold all 6 decks
        public $playerHand;                                                                     // An array to hold player's hand
        public $playerHandValue;                                                                // A variable to hold total value of player's hand
        public $playerHandStatus;                                                               // A variable to hold player hand status (In progress, Stay, Blackjack, Busted)
                                                   
        public $dealerHand;                                                                     // An array to hold dealer's hand
        public $dealerHandValue;                                                                // A variable to hold total value of dealer's hand
        public $dealerHandStatus;                                                               // A variable to hold dealer hand status (In progress, Stay, Blackjack, Busted)

        function __construct(string $newPlayerName, int $newDelay){                             // Constructor for gameRunner class

            $this->playerName = $newPlayerName;                                                 // Setting the player name and delay
            $this->delay = $newDelay;

            $this->playerHand = array();                                                        // Creating the player hand
            $this->playerHandValue = 0;                                                         // Setting player's hand value to 0
            $this->playerHandStatus = "IP";                                                     // Setting player hand status as In Progress

            $this->dealerHand = array();                                                        // Creating the dealer hand
            $this->dealerHandValue = 0;                                                         // Setting dealer's hand value to 0
            $this->dealerHandStatus = "IP";                                                     // Setting dealer hand status as In Progress

            $this->cards = array();                                                             // Creating the array to hold 6 decks

            for($i = 0; $i < 6; $i++){                                                          // At each iteration for 6 iterations
                
                array_push($this->cards, array());                                              // Pushing an empty deck

                for($j = 0; $j < 4; $j++){                                                      // At each iteration for 4 iterations

                    $cardType = ($j+1)*100;                                                     // Defining card type (100: Clubs, 200: Diamonds, 300: Hearts, 400: Spades)

                    for($k = 0; $k < 13; $k++){                                                 // At each iteration for 13 iterations 

                        array_push(($this->cards)[$i], $cardType + ($k+1));                     // Filling the deck with that specific card type (Ex: 301 is Ace of Hearts)
                    }
                }
            }
        }

        function decodeCard(int $cardNum){                                                      // A function to decode cardum (Ex: 210) to class cardInfo

            $resultInfo = new cardInfo("", 0);
            $cardTypeInt = (int) ($cardNum/100);                                                // Card type is cardNum modulus 100
            $cardVal = $cardNum - $cardTypeInt*100;                                             // Card value is cardNum minus hundreds part (Ex: 406-400=6)
            $resultInfo->cardNum = $cardVal;

            switch($cardTypeInt){                                                               // Determining card type according to hundreds part (Ex: 304 is a card of Hearts)

                case 1:
                    $resultInfo->cardType = "Clubs";
                    break;
                case 2:
                    $resultInfo->cardType = "Diamonds";
                    break;
                case 3:
                    $resultInfo->cardType = "Hearts";
                    break;
                default:
                    $resultInfo->cardType = "Spades";
            }

            return $resultInfo;                                                                 // Returning the decoded info as cardInfo class
        }
        
        function takeCard(){                                                                    // A function to take 1 card from 6 decks

            while(true){                                                                        // A for loop to choose the deck again if the chosen deck is empty 

                $deckIdx = rand(0,5);                                                           // Randomly choosing the deck
                $deckCardCount = count(($this->cards)[$deckIdx]);                               // Determining the number of card in that particular deck

                if($deckCardCount == 0){                                                        // If the number of cards is 0 (The deck is empty)

                    continue;                                                                   // Choose another deck
                }
            
                $deckCardIdx = rand(0,$deckCardCount-1);                                        // Randomly choosing a card from the deck

                $removedCard = ($this->cards)[$deckIdx][$deckCardIdx];                          
                array_splice(($this->cards)[$deckIdx], $deckCardIdx, 1);                        // Removing the card from the deck

                return $removedCard;                                                            // Returning the taken card
            }    
        }

        function dealerHandCheck(){                                                             // A function to check dealer's current hand and determine its status

            if($this->dealerHandStatus == "IP"){                                                // If the dealer hand is In Progress (<17)

                $haveAce = false;                                                               // A variable to hold whether the hand has an ace
                $sumWithoutAces = 0;                                                            // A variable to hold the sum of the hand without the aces
                $aceIndices = array();                                                          // An array to hold ace indices if the hand has ace(s)                                                                 
                
                for($i = 0; $i < count($this->dealerHand); $i++){                               // Iterating through whole hand

                    if(($this->dealerHand[$i] % 100) == 1){                                     // If the particular card is an ace

                        $haveAce = true;                                                        // Mark the hand as having at least one ace
                        array_push($aceIndices, $i);                                            // Saving the index of the ace
                        continue;                                                               // Continue to look other cards in the hand
                    }

                    $sumWithoutAces += $this->decodeValue($this->dealerHand[$i] % 100);         // If the particular card is not an ace, add the value of the card to sum without aces
                }

                if($haveAce == false){                                                          // If the deck has no ace, determine hand status using $sumWithoutAces

                    if($sumWithoutAces < 17){                                                   // If the sum without aces is smaller than 17

                        return "IP";                                                            // The hand is still In Progress
                    }

                    else if(($sumWithoutAces >= 17) && ($sumWithoutAces < 21)){                 // If the sum without aces is bigger than 17 and smaller than 21

                        $this->dealerHandValue = $sumWithoutAces;                               // The hand check is over. Saving the hand value
                        return "S";                                                             // The hand is a Stay
                    }

                    else if($sumWithoutAces == 21){                                             // If the sum without aces is 21

                        $this->dealerHandValue = 21;                                            // The hand check is over. Saving the hand value
                        return "BL";                                                            // The hand is a Blackjack
                    }

                    else{                                                                       // If the sum without aces is bigger than 21

                        $this->dealerHandValue = $sumWithoutAces;                               // The hand check is over. Saving the hand value
                        return "BU";                                                            // The hand is a Busted
                    }
                }

                else{                                                                           // If the deck has ace(s)

                    $firstCaseVal = 11 + (count($aceIndices) - 1);                              // First case is one ace being 11, rest being 1
                    $secondCaseVal = count($aceIndices);                                        // Second case is all aces being 1
                    $firstCaseSum = $sumWithoutAces + $firstCaseVal;
                    $secondCaseSum = $sumWithoutAces + $secondCaseVal;

                    if($firstCaseSum < 17){                                                     // If first case is smaller than 17

                        return "IP";                                                            // No need to look at second case. Return In Progress
                    }

                    else if(($firstCaseSum >= 17) && ($firstCaseSum < 21)){                     // If first case is bigger than 17 and smaller than 21

                        if($firstCaseSum >= $this->playerHandValue){                            // If dealer's hand is bigger than or equal to player's hand

                            $this->dealerHandValue = $firstCaseSum;                             // Save the hand value
                            return "S";                                                         // The hand is a Stay
                        }
                        
                        else{                                                                   // If the dealer hand is smaller

                            return "IP";                                                        // There is no point of staying if the dealer will lose. Thinking of second case of aces and returnin In Progress
                        }
                    }

                    else if($firstCaseSum == 21){                                               // If first case is 21 

                        $this->dealerHandValue = 21;                                            // Save the hand value
                        return "BL";                                                            // The hand is a Blackjack
                    }

                    else{                                                                       // If the first case is bigger than 21

                        if($secondCaseSum < 17){                                                // If the second case is smaller than 17 

                            return "IP";                                                        // The hand is In Progress
                        }

                        else if(($secondCaseSum >= 17) && ($secondCaseSum < 21)){               // If the second case is bigger than 17 and smaller than 21 

                            $this->dealerHandValue = $secondCaseSum;                            // Saving the hand value
                            return "S";                                                         // The hand is a Stay
                        }

                        else if($secondCaseSum == 21){                                          // If the second case is 21

                            $this->dealerHandValue = 21;                                        // Saving the hand value
                            return "BL";                                                        // The hand is a Blackjack
                        }

                        else{                                                                   // If the second case is bigger than 21    

                            $this->dealerHandValue = $secondCaseSum;                            // Saving the hand value
                            return "BU";                                                        // The hand is a Busted
                        }
                    }
                }
            }
        }
        
        function playerHandCheck(){                                                             // A function to check player's current hand and determine its status

            if($this->playerHandStatus == "IP"){                                                // If the player hand is In Progress

                $haveAce = false;                                                               // A variable to hold whether the hand has ace(s)
                $sumWithoutAces = 0;                                                            // A variable to hold sum without aces
                $aceIndices = array();                                                          // A variable to hold ace indices                                                            
                
                for($i = 0; $i < count($this->playerHand); $i++){                               // Iterating through player hand

                    if(($this->playerHand[$i] % 100) == 1){                                     // If the particular card is an ace

                        $haveAce = true;                                                        // Marking that the hand has at least one ace
                        array_push($aceIndices, $i);                                            // Saving the index of the ace
                        continue;                                                               // Continue with other cards in the hand
                    }

                    $sumWithoutAces += $this->decodeValue($this->playerHand[$i] % 100);         // If the particular card is not an ace, adding its value to sum without aces
                }

                if($haveAce == false){                                                          // If the deck has no ace, determine hand status using $sumWithoutAces

                    if($sumWithoutAces < 21){                                                   // If the sum without aces is smaller than 21

                        return "IP";                                                            // The hand is In Progress
                    }

                    else if($sumWithoutAces == 21){                                             // If the sum without aces is 21     

                        $this->playerHandValue = 21;                                            // Saving the hand value
                        return "BL";                                                            // The hand is a Blackjack
                    }

                    else{                                                                       // If the sum without aces is bigger than 21

                        $this->playerHandValue = $sumWithoutAces;                               // Saving the hand value
                        return "BU";                                                            // The hand is a Busted
                    }
                }

                else{                                                                           // If the hand has ace(s)

                    $firstCaseVal = 11 + (count($aceIndices) - 1);                              // First case is one ace being 11, rest being 1
                    $secondCaseVal = count($aceIndices);                                        // Second case is all aces being 1
                    $firstCaseSum = $sumWithoutAces + $firstCaseVal;
                    $secondCaseSum = $sumWithoutAces + $secondCaseVal;

                    if($firstCaseSum < 21){                                                     // If the first case is smaller than 21

                        return "IP";                                                            // The hand is In Progress
                    }

                    else if($firstCaseSum == 21){                                               // If the first case is 21

                        $this->playerHandValue = 21;                                            // Saving the hand value
                        return "BL";                                                            // The hand is a Blackjack
                    }

                    else{                                                                       // If the first case is bigger than 21

                        if($secondCaseSum < 21){                                                // If the second case is smaller than 21

                            return "IP";                                                        // The hand is In Progress
                        }

                        else if($secondCaseSum == 21){                                          // If the second case is 21

                            $this->playerHandValue = 21;                                        // Saving the hand value
                            return "BL";                                                        // The hand is a Blackjack
                        }

                        else{                                                                   // If the second case is bigger than 21

                            $this->playerHandValue = 21;                                        // Saving the hand value
                            return "BU";                                                        // The hand is a Busted
                        }
                    }
                }
            }    
        }

        function setRound(){                                                                    // A function to give 2 cards to both player and dealer to make game ready

            $this->dealerHand = array();                                                        // Re-initializing (clearing) arrays in case they are full of junk from last round
            $this->playerHand = array();
            
            $this->playerHandValue = 0;                                                         // Setting player's hand value to 0 in case it is non zero from last round
            $this->playerHandStatus = "IP";                                                     // Setting player hand status as In Progress in case if it is not from the last round

            $this->dealerHandValue = 0;                                                         // Setting dealer's hand value to 0 in case it is non zero from last round
            $this->dealerHandStatus = "IP";                                                     // Setting dealer hand status as In Progress in case if it is not from the last round

            array_push($this->dealerHand, $this->takeCard());                                   // Giving 2 players 2 cards each. The order is important (Dealer, Player, Dealer, Player)
            array_push($this->playerHand, $this->takeCard());
            array_push($this->dealerHand, $this->takeCard());
            array_push($this->playerHand, $this->takeCard());

            $this->playerHandStatus = $this->playerHandCheck();
            $this->dealerHandStatus = $this->dealerHandCheck();

            $this->printHands(0);

            if($this->playerHandStatus == "BL") $this->contDealerHand();
        }

        function decodeValue(int $cardNum){

            $cardVal = $cardNum % 100;

            if($cardVal < 11){

                return $cardVal;
            }

            else{

                return 10;
            }
        }

        function decodeName(int $cardVal){                                                      // A function to decode card names using their values

            if($cardVal == 1){
                return "Ace";
            }
            else if(($cardVal > 1) && ($cardVal < 11)){
                return strval($cardVal);
            }
            else if($cardVal == 11){
                return "Jack";
            }
            else if($cardVal == 12){
                return "Queen";
            }
            else{
                return "King";
            }
        }

        function contDealerHand(){

            if($this->dealerHandStatus == "IP"){

                while(true){

                    $takenCard =  $this->takeCard();
                    array_push($this->dealerHand, $takenCard);

                    $this->dealerHandStatus = $this->dealerHandCheck();

                    $this->printHands(1);

                    if($this->dealerHandStatus != "IP"){

                        break;
                    }
                }
            }

            if($this->playerHandStatus == "BL"){

                if($this->dealerHandStatus == "BL"){

                    $this->gameResult = "Draw";
                }

                else{

                    $this->gameResult = "Player";
                }
            }
            
            else{                                                                               // Player hand status has to be 'Stay' (Only choice left)

                if($this->dealerHandStatus == "S"){

                    if($this->dealerHandValue > $this->playerHandValue){

                        $this->gameResult = "Dealer";
                    }

                    else{

                        $this->gameResult = "Player";
                    }
                }

                else if($this->dealerHandStatus == "BL"){

                    $this->gameResult = "Dealer";
                }

                else{

                    $this->gameResult = "Player";
                }
            }

            $this->printWinner();
            return 0;
        }
        
        function playerStay(){                                                                  // A function to mark player as staying and continue with dealer hand

            if($this->playerHandStatus == "IP"){

                $this->playerHandStatus = "S";

                $haveAce = false;
                $sumWithoutAces = 0;
                $aceIndices = array();                                                                   
                
                for($i = 0; $i < count($this->playerHand); $i++){

                    if(($this->playerHand[$i] % 100) == 1){

                        $haveAce = true;
                        array_push($aceIndices, $i);
                        continue;
                    }

                    $sumWithoutAces += $this->decodeValue($this->playerHand[$i] % 100);
                }

                if($haveAce == false){

                    $this->playerHandValue = $sumWithoutAces;
                }

                else{

                    $firstCaseVal = 11 + (count($aceIndices) - 1);
                    $secondCaseVal = count($aceIndices);
                    $firstCaseSum = $sumWithoutAces + $firstCaseVal;
                    $secondCaseSum = $sumWithoutAces + $secondCaseVal;

                    if(($firstCaseSum > $secondCaseSum) && ($firstCaseSum < 21)){

                        $this->playerHandValue = $firstCaseSum;
                    }

                    else{

                        $this->playerHandValue = $secondCaseSum;
                    }
                }

                $this->contDealerHand();
            }    
        }
        
        function playerHit(){                                                                   // A function to hit player with a card

            if($this->playerHandStatus == "IP"){
                
                $takenCard =  $this->takeCard();
                array_push($this->playerHand, $takenCard);

                $this->playerHandStatus = $this->playerHandCheck();

                $this->printHands(0);

                if($this->playerHandStatus == "IP"){

                    return 0;
                }

                else if($this->playerHandStatus == "BU"){

                    $this->gameResult = "Dealer";
                    $this->printWinner();
                    return 0;
                }
                
                $this->contDealerHand();
            }

            else{
                
                return -1;
            }    
        }

        function printWinner(){

            $resultString = "<br><br> THE RESULT (WINNER) IS: " . $this->gameResult . "<br>";
            $winnerHand;
            
            if($this->gameResult == "Dealer"){

                $winnerHand = $this->dealerHand; 
            }

            else{

                $winnerHand = $this->playerHand;
            }
            
            
            for($i = 0; $i < count($winnerHand); $i++){

                $currentCardInfo = $this->decodeCard($winnerHand[$i]);
                $resultString .= $currentCardInfo->cardType . " - " . $this->decodeName($currentCardInfo->cardNum);
                
                if($i != count($winnerHand)-1){
                    $resultString .= ', ';
                }
            }

            $resultString .= "<br> The user needs to wait " . $this->delay . " seconds to start the next round..<br><br>";
            echo $resultString;
            sleep($this->delay);
            $this->setRound();
        }

        function printHands(int $mode){                                                         // Printing hands (if mode == 0, first card of dealer is unknown; else it is known)

            $resultString = '<div width="50%">Dealer: ';
            $i = 0;
            
            if($mode == 0){

                $resultString .= 'Unknown, ';
                $i = 1; 
            }

            for(; $i < count($this->dealerHand); $i++){

                $currentCardInfo = $this->decodeCard($this->dealerHand[$i]);
                $resultString .= $currentCardInfo->cardType . " - " . $this->decodeName($currentCardInfo->cardNum);
                
                if($i != count($this->dealerHand)-1){
                    $resultString .= ', ';
                }
            }

            $resultString .= '<br>';
            $resultString .= 'Player: ';

            for($i = 0; $i < count($this->playerHand); $i++){

                $currentCardInfo = $this->decodeCard($this->playerHand[$i]);
                $resultString .= $currentCardInfo->cardType . " - " . $this->decodeName($currentCardInfo->cardNum);
                
                if($i != count($this->playerHand)-1){
                    $resultString .= ', ';
                }
            }

            $resultString .= '<br>';
            $resultString .= 'Player Status: ' . $this->playerHandStatus . '  Dealer Status: ' . $this->dealerHandStatus;
            $resultString .= '<br><br></div>';
            
            if($mode == 0){

                $resultString .= '  <form action="hit.php" method="get">
                                    <input type="submit" value="Hit">
                                </form>
                                <form action="stay.php" method="get">
                                    <input type="submit" value="Stay">
                                </form>';
            }    

            echo $resultString;
        }
    }

?>