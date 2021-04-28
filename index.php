<?php

    require("classes.inc");

    session_start();
    
    $myGame = new gameRunner("ogz", 10);
    
    $myGame->setRound();

    $_SESSION['myGame'] = $myGame;

?>

<!--<!DOCTYPE html>
<html>
<body>

    <form action="hit.php" method="get">
        <input type="submit" value="Hit">
    </form>
    <form action="stay.php" method="get">
        <input type="submit" value="Stay">
    </form>

</body>
</html>-->