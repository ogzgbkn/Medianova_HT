<?php

    require("classes.inc");

    session_start();

    $myGame = $_SESSION['myGame'];
    $myGame->playerStay();
    $_SESSION['myGame'] = $myGame;

?>