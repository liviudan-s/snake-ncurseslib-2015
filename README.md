# snake-ncurseslib-2015
Joc Snake cu functionalitati de baza facut cu ajutorul bibliotecii ncurses. Linux only.

Pentru obtinerea bibliotecii ncurses:
  sudo apt-get install libncurses5-dev
  
Pentru compilare: make
Pentru rulare: make run

Improvement(Bonus) fata de enunt:
 - Din cauza functionalitatii bibliotecii ncurses si faptului ca foloseste terminalul din linux, sunt mult mai putine "pozitii" pe verticala decat pe orizontala. De aceea, in implementare am pus delay/scaling diferit al vitezei de miscare pe verticala decat pe orizontala, pentru ca snakeul sa aiba o viteza asemanatoare si pe verticala si pe orizontala.(needs more finetuning)
 - Highscores(not fully implemented)

Known caveats:
 -  after the highscores menu the game can only be closed with force close(Ctrl+C)
 -  game needs the highscores file to run
 -  if snake is moving right for example, if you press left game registers it as going into an obstacle(makes sense because you run into yourself, but maybe for more normal gameplay it should do nothing)
