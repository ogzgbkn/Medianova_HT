<?php

    require("classes.inc");

    session_start();
    
    $myGame = new gameRunner("ogz", 10);
    
    $myGame->setRound();

    $_SESSION['myGame'] = $myGame;

?>
