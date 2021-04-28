<?php

    class cardInfo{                                                                             // CardInfo class holds card type and card value

        public $cardNum;
        public $cardType;

        function __construct(string $newCardType, int $newCardNum){

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

            for($i = 0; $i < 6; $i++){                                                         // At each iteration for 6 iterations
                
                array_push($this->cards, array());                                              // Pushing an empty deck

                for($j = 0; $j < 4; $j++){                                                     // At each iteration for 4 iterations

                    $cardType = ($j+1)*100;                                                     // Defining card type (100: Clubs, 200: Diamonds, 300: Hearts, 400: Spades)

                    for($k = 0; $k < 13; $k++){                                                // At each iteration for 13 iterations 

                        array_push(($this->cards)[$i], $cardType + ($k+1));                     // Filling the deck with that specific card type (Ex: 301 is Ace of Hearts)
                    }
                }
            }
        }

        function decodeCard(int $cardNum){                                                      // A function to decode cardum (Ex: 210) to class cardInfo

            $resultInfo = new cardInfo("", 0);
            $cardTypeInt = (int) ($cardNum/100);
            $cardVal = $cardNum - $cardTypeInt*100;
            $resultInfo->cardNum = $cardVal;

            switch($cardTypeInt){

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

            return $resultInfo;
        }
        
        function takeCard(){                                                                    // A function to take 1 card from 6 decks

            while(true){

                $deckIdx = rand(0,5);
                $deckCardCount = count(($this->cards)[$deckIdx]);

                if($deckCardCount == 0){

                    continue;
                }
            
                $deckCardIdx = rand(0,$deckCardCount-1);

                $removedCard = ($this->cards)[$deckIdx][$deckCardIdx];
                array_splice(($this->cards)[$deckIdx], $deckCardIdx, 1);

                return $removedCard;
            }    
        }

        function dealerHandCheck(){                                                             // A function to check dealer's current hand and determine its status

            if($this->dealerHandStatus == "IP"){

                $haveAce = false;
                $sumWithoutAces = 0;
                $aceIndices = array();                                                                   
                
                for($i = 0; $i < count($this->dealerHand); $i++){

                    if(($this->dealerHand[$i] % 100) == 1){

                        $haveAce = true;
                        array_push($aceIndices, $i);
                        continue;
                    }

                    $sumWithoutAces += $this->decodeValue($this->dealerHand[$i] % 100);
                }

                if($haveAce == false){                                                          // If the deck has no ace, determine hand status using $sumWithoutAces

                    if($sumWithoutAces < 17){

                        return "IP";
                    }

                    else if(($sumWithoutAces >= 17) && ($sumWithoutAces < 21)){

                        $this->dealerHandValue = $sumWithoutAces;
                        return "S";
                    }

                    else if($sumWithoutAces == 21){

                        $this->dealerHandValue = 21;
                        return "BL";
                    }

                    else{

                        $this->dealerHandValue = $sumWithoutAces;
                        return "BU";
                    }
                }

                else{

                    $firstCaseVal = 11 + (count($aceIndices) - 1);
                    $secondCaseVal = count($aceIndices);
                    $firstCaseSum = $sumWithoutAces + $firstCaseVal;
                    $secondCaseSum = $sumWithoutAces + $secondCaseVal;

                    if($firstCaseSum < 17){

                        return "IP";
                    }

                    else if(($firstCaseSum >= 17) && ($firstCaseSum < 21)){

                        if($firstCaseSum >= $this->playerHandValue){

                            $this->dealerHandValue = $firstCaseSum;
                            return "S";
                        }
                        
                        else{

                            return "IP";
                        }
                    }

                    else if($firstCaseSum == 21){

                        $this->dealerHandValue = 21;
                        return "BL";
                    }

                    else{

                        if($secondCaseSum < 17){

                            return "IP";
                        }

                        else if(($secondCaseSum >= 17) && ($secondCaseSum < 21)){

                            $this->dealerHandValue = $secondCaseSum;
                            return "S";
                        }

                        else if($secondCaseSum == 21){

                            $this->dealerHandValue = 21;
                            return "BL";
                        }

                        else{

                            $this->dealerHandValue = $secondCaseSum;
                            return "BU";
                        }
                    }
                }
            }
        }
        
        function playerHandCheck(){                                                             // A function to check player's current hand and determine its status

            if($this->playerHandStatus == "IP"){

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

                if($haveAce == false){                                                          // If the deck has no ace, determine hand status using $sumWithoutAces

                    if($sumWithoutAces < 21){

                        return "IP";
                    }

                    else if($sumWithoutAces == 21){

                        $this->playerHandValue = 21;
                        return "BL";
                    }

                    else{

                        $this->playerHandValue = $sumWithoutAces;
                        return "BU";
                    }
                }

                else{

                    $firstCaseVal = 11 + (count($aceIndices) - 1);
                    $secondCaseVal = count($aceIndices);
                    $firstCaseSum = $sumWithoutAces + $firstCaseVal;
                    $secondCaseSum = $sumWithoutAces + $secondCaseVal;

                    if($firstCaseSum < 21){

                        return "IP";
                    }

                    else if($firstCaseSum == 21){

                        $this->playerHandValue = 21;
                        return "BL"; 
                    }

                    else{

                        if($secondCaseSum < 21){

                            return "IP";
                        }

                        else if($secondCaseSum == 21){

                            $this->playerHandValue = 21;
                            return "BL";
                        }

                        else{

                            $this->playerHandValue = 21;
                            return "BU";
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