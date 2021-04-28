<?php

    require("classes.inc");

    session_start();

    $myGame = $_SESSION['myGame'];
    $myGame->playerHit();
    $_SESSION['myGame'] = $myGame;

?>