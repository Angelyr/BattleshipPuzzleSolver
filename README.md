# Battleship Puzzle Solver:

Explination of battleship puzzle game: https://en.wikipedia.org/wiki/Battleship_(puzzle)

This code receives a battleship puzzle as input in this format:

board 10 10   
rows 2 2 4 2 1 2 1 4 2 0   
cols 3 3 1 2 1 1 3 1 1 4  
submarine  
submarine  
submarine  
submarine  
destroyer  
destroyer  
destroyer  
cruiser  
cruiser  
battleship  

This input includes the size of the board, the amount of ship tiles in every row and column,  
and a list of all of the ships that are on the board. Then the program solves the puzzle and   
returns all the ships it found and it their location. It also returns prints a ASCII picture   
of what the board looks like in this format:  

Solution:
battleship  2 0 horizontal  
cruiser     7 3 horizontal  
cruiser     3 6 vertical  
destroyer   0 0 horizontal  
destroyer   1 7 horizontal  
destroyer   7 9 vertical  
submarine   3 9  
submarine   5 9  
submarine   6 0  
submarine   8 1  
+----------+  
|<>        |2  
|       <> |2  
|<xx>      |4  
|      ^  o|2  
|      x   |1  
|      v  o|2  
|o         |1  
|   <x>   ^|4  
| o       v|2  
|          |0  
+----------+  
 3312113114  


In addition, you can pass in a second input in this format:  
board 10 10  
rows 2 2 4 2 1 2 1 4 2 0  
cols 3 3 1 2 1 1 3 1 1 4  
submarine  
submarine  
submarine  
submarine  
destroyer  
destroyer  
destroyer  
cruiser  
cruiser  
battleship  
constraint 2 4 X  
constraint 7 1 X  
constraint 7 7 X  

This is identical to the first argument except it contains a list of constraints.  
These constraints include the row and the column, and a character of what needs to be   
in that postition. Then the program will return a list of all the possible solutions  
to this puzzle with the constraints.  

Coordinates are in this format:  
+----------+  
|          |0  
|          |1  
|          |2  
|          |3  
|          |4   (y)  
|          |5   
|          |6  
|          |7  
|          |8  
|          |9  
+----------+  
 0123456789  

    (x)
