//Adam Buerger
//CSC 362 Programming Assignment 4
//A Chutes-and-Laders style game where two players (represented by char pointers) take turns rolling a 6-sided die to move along a board represented by a string 
/*
		Board Spaces
an empty space (' ') does not move the player after they have landed on it
a 'B' moves the player backwards to the nearest 'H' or the beginning of the board if there is no 'H' behind the player
an 'F' moves the player forward to the nearest 'H' or not at all if there is no 'H' in front of the player
	-an 'H' is a destination that can be reached by either 'B' or 'F' and is replaced with an empty space (' ') if it is landed on by a player
'a' through 'm' moves the player backwards x spaces where x is the difference between the letter landed on and 'n'
'o' through 'z' moves the player forwards x spaces where x is the different between the letter landed on and 'n'
	-at no point will 'n' be used in the board
	-if a any lowercase letter is landed on, it becomes a '-'
a '*' moves the player to the beginning of the board
*/

#include <stdlib.h>
#include <stdio.h>
#define SIZE 100

//The move function takes in the both players, the board, and an immediate datum (int) to determine whose turn it is.
//It rolls a number 1-6 and moves the turn player that many spaces forward on the board then determines whether or not the player needs to be moved again based on what they land on
char* move(char*, char*, char*, int);

//The findHaven function takes in the active player (the player whose turn it is) and the board if that player lands on a B or F and moves it again
//if the player lands on a B they move back to the nearest H or the start of the board if no H can be found
//if the player lands on a F they move forward to the nearest H or not at all if there is no H between them and the end of the board
char* findHaven(char*, char*);

//the chuteLadder function takes in the active player and the board to determine if the player needs to move again
//If a player lands on any lowercase letter a-z, they are moved x spaces where x is the difference between the number landed on and n.
//The letter n does not appear in the board since n-n is 0
char* chuteLadder(char*, char*);

//Prints the state of the board and where the players are on it into the file pointed to by the file variable.
//Player1 is represented with a 1 and Player2 is represented with a 2.
//If both players are on the same space, they are both represented with a &.
void output(char*, char*, char*, FILE*);

void main() {
	char board[] = "  mHk tH l B He Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB  ";
	char* Player1 = board;										//Declare the pointer for Player 1 and start them at the first space on the board
	char* Player2 = board;										//Declare the pointer for Player 2 and start them at the first space on the board
	FILE* file;													//Declare the file pointer to send the board output.
	file = fopen("Output.txt", "w");							//associate the file pointer with a text file to output to
	srand(time(NULL));											//seed the random number generator
	while (Player1 < board+SIZE && Player2 < board+SIZE) {		//run the game until one or both players has passed the end point of the board
		Player1 = move(Player1, Player2, board, 1);				//Player 1 moves
		output(board, Player1, Player2, file);					//output the board now that Player 1 has moved
		Player2 = move(Player2, Player1, board, 2);				//Player 2 moves
		output(board, Player1, Player2, file);					//output the board now that Player 2 has moves
	}
	if (Player1 >= board + SIZE)								//if only one player has reached the end of the board, check which player it is
		printf("Player 1 wins!");
	else
		printf("Player 2 wins!");
}

//The move function takes in the both players, the board, and an immediate datum (int) to determine whose turn it is.
//It rolls a number 1-6 and moves the turn player that many spaces forward on the board then determines whether or not the player needs to be moved again based on what they land on
void output(char* board, char* p1, char* p2, FILE* file) {
	for (; *board != '\0'; board++) {							//run the loop until the entire board has been printed
		if (board == p1)										//check if Player1 is on the current space
			fprintf(file, "1");									//if so, print a 1 to the file
		else if (board == p2)									//check if Player2 is on the current spcae
			fprintf(file, "2");									//if so, print a 2 to the file
		else													//if neither Player1 nor Player2 are on the current space
			fprintf(file, "%c", *board);						//print the normal board space to the file
	}
	fprintf(file, "\n");										//create a new line so the next boardstate can be read clearly when printed
}

//The move function takes in the both players, the board, and an immediate datum (int) to determine whose turn it is.
//It rolls a number 1-6 and moves the turn player that many spaces forward on the board then determines whether or not the player needs to be moved again based on what they land on
char* move(char* activePlayer, char* otherPlayer, char* board, int turnPlayer) {
	int roll;													//create an int variable to hold the roll
	roll = rand() % 6 + 1;										//roll a number 0 through 5 and add 1 to it
	activePlayer += roll;										//move the active player forward
	if (activePlayer >= board + SIZE) {							//check if the active player has reached the end of the board
		printf("Player %d rolled a %d and reached the end of the board!\n", turnPlayer, roll);
		return activePlayer;
	}
	//since the active player has not reached the end of the board, say how many spaced they moved. The sentence will be completed based on where activePlayer lands
	printf("Player %d rolled a %d, and ", turnPlayer, roll);	
	if (*activePlayer == 'B' || *activePlayer == 'F') {			//check if the player has landed on a B or F
		activePlayer = findHaven(activePlayer, board);			//move the player again
	}
	else if (*activePlayer == '*') {							//check if the player landed on a bomb
		*activePlayer = '\ ';									//the bomb gets used up when it is landed on
		printf("landed on a BOMB! They move back to the start!\n");
		activePlayer =  board;											//if the player landed on a bomb, send them back to the start of the board
	}
	else if (*activePlayer >= 'a' && *activePlayer <= 'z') {	//check if the player landed on a chute or ladder. n does not appear on the board so there is no conflict.
		activePlayer =  chuteLadder(activePlayer, board);
	}
	else
		printf("moved to square %d.\n", activePlayer - board);	//if any player has not landed on any letter a through z, a B or F, or a *, they do not move again
	if (activePlayer == otherPlayer && activePlayer != board) {
		activePlayer--;
		printf("Player %d got bumped back a space!\n", turnPlayer);
	}
	return activePlayer;
}

//The findHaven fuvtion takes in the active player (the player whose turn it is) and the board if that player lands on a B or F and moves it again
//if the player lands on a B they move back to the nearest H or the start of the board if no H can be found
//if the player lands on a F they move forward to the nearest H or not at all if there is no H between them and the end of the board
char* findHaven(char* Player, char* board) {
	char* searcher = Player;									//create a copy of Player that can move through the board in case 
	if (*Player == 'B') {										//check if the player is on a B
		while (*searcher != 'H' && searcher > board)			//move B back to either the beginning of the board or the nearest H behind it
			searcher--;
		printf("landed on a B so they slide back to square %d.\n", searcher - board);
		*searcher = '+';										//the H gets used up when it is landed on
		return searcher;
	}
	else if (*Player == 'F') {									//check if the player landed on an F
		while (*searcher != 'H' && searcher < board+SIZE)		//attempt to move the searcher to the nearest H in front of it
			searcher++;
		if (*searcher == '\0') {								//check if the searcher pointer is at the end of the board
			printf("landed on an F but stays on square %d.\n", Player - board);
			return Player;										//if so, don't move the player
		}
		else {
			printf("landed on an F, moving forward to square %d.\n", searcher - board);
			*searcher = '+';									//if the searcher lands on an H, it gets used up
			return searcher;									//then return the place where searcher is
		}
	}
}

//If a player lands on any lowercase letter a-z, they are moved x spaces where x is the difference between the number landed on and n.
//The letter n does not appear in the board since n-n is 0
char* chuteLadder(char* Player, char* board) {
	int mover;													//create a variable to hold how many spaces the player will move forward or backwards
	mover = (int)(*Player - 'n');								//calculate mover by type casting the subtraction of n from where player is.
	if (*Player >= 'a' && *Player <= 'm')
		printf("landed on a chute. They slide back %d spaces to square %d.\n", -1 * mover, Player + mover - board);
	else
		printf("landed on a ladder. They climb up %d spaces to square %d.\n", mover, Player + mover - board);
	*Player = '-';												//the chute/ladder gets used up when it is landed on
	Player += mover;											//since mover is either a positive or negative number, you can add it to Player without issue
	return Player;
}
/*
		Output Log
Player 1 rolled a 1, and moved to square 1.
Player 2 rolled a 1, and moved to square 1.
Player 2 got bumped back a space!															I guess this isn't technically super improbable especially given how many times I've ran this but I think it's neat that this happened on the second roll
Player 1 rolled a 5, and landed on a ladder. They climb up 6 spaces to square 12.			EXAMPLE of ladder
Player 2 rolled a 5, and moved to square 5.
Player 1 rolled a 3, and moved to square 15.
Player 2 rolled a 6, and landed on a B so they slide back to square 7.						EXAMPLE of B push back to H
Player 1 rolled a 4, and moved to square 19.
Player 2 rolled a 1, and moved to square 8.
Player 1 rolled a 1, and landed on a ladder. They climb up 11 spaces to square 31.
Player 2 rolled a 6, and landed on a chute. They slide back 9 spaces to square 5.			EXAMPLE of chute
Player 1 rolled a 4, and landed on a B so they slide back to square 33.
Player 2 rolled a 5, and moved to square 10.
Player 1 rolled a 4, and landed on a BOMB! They move back to the start!						EXAMPLE of bomb
Player 2 rolled a 2, and moved to square 12.
Player 1 rolled a 1, and moved to square 1.
Player 2 rolled a 6, and landed on a ladder. They climb up 3 spaces to square 21.
Player 1 rolled a 2, and moved to square 3.
Player 2 rolled a 6, and landed on a ladder. They climb up 11 spaces to square 38.
Player 1 rolled a 6, and landed on a chute. They slide back 2 spaces to square 7.
Player 2 rolled a 6, and moved to square 44.
Player 1 rolled a 3, and moved to square 10.
Player 2 rolled a 1, and landed on a chute. They slide back 5 spaces to square 40.
Player 1 rolled a 6, and landed on an F, moving forward to square 22.						EXAMPLE of F push forward to H
Player 2 rolled a 3, and landed on a B so they slide back to square 41.
Player 1 rolled a 2, and moved to square 24.
Player 2 rolled a 2, and landed on a B so they slide back to square 28.
Player 1 rolled a 2, and landed on a B so they slide back to square 13.
Player 2 rolled a 1, and landed on a chute. They slide back 2 spaces to square 27.
Player 1 rolled a 2, and moved to square 15.
Player 2 rolled a 6, and moved to square 33.
Player 1 rolled a 5, and moved to square 20.
Player 2 rolled a 3, and moved to square 36.
Player 1 rolled a 5, and landed on a chute. They slide back 6 spaces to square 19.
Player 2 rolled a 5, and moved to square 41.
Player 1 rolled a 6, and moved to square 25.
Player 2 rolled a 1, and landed on an F, moving forward to square 50.
Player 1 rolled a 3, and moved to square 28.
Player 2 rolled a 1, and moved to square 51.
Player 1 rolled a 2, and landed on a chute. They slide back 6 spaces to square 24.
Player 2 rolled a 6, and landed on a BOMB! They move back to the start!
Player 1 rolled a 2, and landed on a B so they slide back to square 3.
Player 2 rolled a 3, and moved to square 3.
Player 2 got bumped back a space!
Player 1 rolled a 6, and moved to square 9.
Player 2 rolled a 3, and moved to square 5.
Player 1 rolled a 6, and moved to square 15.
Player 2 rolled a 4, and moved to square 9.
Player 1 rolled a 5, and moved to square 20.
Player 2 rolled a 1, and moved to square 10.
Player 1 rolled a 4, and moved to square 24.
Player 2 rolled a 4, and moved to square 14.
Player 1 rolled a 6, and moved to square 30.
Player 2 rolled a 3, and landed on a chute. They slide back 2 spaces to square 15.
Player 1 rolled a 5, and landed on a B so they slide back to square 0.						EXAMPLE of B pushing back to the start
Player 2 rolled a 5, and moved to square 20.
Player 1 rolled a 5, and moved to square 5.
Player 2 rolled a 3, and moved to square 23.
Player 1 rolled a 3, and moved to square 8.
Player 2 rolled a 6, and moved to square 29.
Player 1 rolled a 6, and moved to square 14.
Player 2 rolled a 5, and moved to square 34.
Player 1 rolled a 3, and moved to square 17.
Player 2 rolled a 1, and landed on a B so they slide back to square 0.
Player 1 rolled a 5, and moved to square 22.
Player 2 rolled a 1, and moved to square 1.
Player 1 rolled a 4, and landed on a B so they slide back to square 0.
Player 2 rolled a 5, and moved to square 6.
Player 1 rolled a 6, and moved to square 6.
Player 1 got bumped back a space!
Player 2 rolled a 5, and landed on a B so they slide back to square 0.
Player 1 rolled a 2, and moved to square 7.
Player 2 rolled a 6, and moved to square 6.
Player 1 rolled a 3, and moved to square 10.
Player 2 rolled a 3, and moved to square 9.
Player 1 rolled a 5, and moved to square 15.
Player 2 rolled a 3, and moved to square 12.
Player 1 rolled a 2, and moved to square 17.
Player 2 rolled a 1, and moved to square 13.
Player 1 rolled a 6, and moved to square 23.
Player 2 rolled a 2, and moved to square 15.
Player 1 rolled a 4, and moved to square 27.
Player 2 rolled a 1, and landed on an F, moving forward to square 62.
Player 1 rolled a 6, and moved to square 33.
Player 2 rolled a 1, and moved to square 63.
Player 1 rolled a 2, and landed on a B so they slide back to square 0.
Player 2 rolled a 2, and landed on a chute. They slide back 4 spaces to square 61.
Player 1 rolled a 4, and landed on a chute. They slide back 3 spaces to square 1.
Player 2 rolled a 6, and moved to square 67.
Player 1 rolled a 6, and moved to square 7.
Player 2 rolled a 3, and moved to square 70.
Player 1 rolled a 3, and moved to square 10.
Player 2 rolled a 6, and moved to square 76.
Player 1 rolled a 2, and moved to square 12.
Player 2 rolled a 3, and moved to square 79.
Player 1 rolled a 4, and landed on an F, moving forward to square 69.
Player 2 rolled a 4, and landed on a BOMB! They move back to the start!
Player 1 rolled a 3, and moved to square 72.
Player 2 rolled a 6, and moved to square 6.
Player 1 rolled a 2, and landed on a B so they slide back to square 0.
Player 2 rolled a 1, and moved to square 7.
Player 1 rolled a 2, and landed on a chute. They slide back 1 spaces to square 1.
Player 2 rolled a 5, and moved to square 12.
Player 1 rolled a 2, and moved to square 3.
Player 2 rolled a 1, and moved to square 13.
Player 1 rolled a 5, and moved to square 8.
Player 2 rolled a 6, and moved to square 19.
Player 1 rolled a 4, and moved to square 12.
Player 2 rolled a 2, and moved to square 21.
Player 1 rolled a 5, and moved to square 17.
Player 2 rolled a 5, and landed on a B so they slide back to square 0.
Player 1 rolled a 3, and moved to square 20.
Player 2 rolled a 1, and moved to square 1.
Player 1 rolled a 4, and moved to square 24.
Player 2 rolled a 4, and moved to square 5.
Player 1 rolled a 2, and landed on a B so they slide back to square 0.
Player 2 rolled a 4, and moved to square 9.
Player 1 rolled a 4, and moved to square 4.
Player 2 rolled a 6, and moved to square 15.
Player 1 rolled a 3, and moved to square 7.
Player 2 rolled a 2, and moved to square 17.
Player 1 rolled a 2, and moved to square 9.
Player 2 rolled a 3, and moved to square 20.
Player 1 rolled a 2, and landed on a B so they slide back to square 0.
Player 2 rolled a 1, and moved to square 21.
Player 1 rolled a 5, and moved to square 5.
Player 2 rolled a 6, and moved to square 27.
Player 1 rolled a 3, and moved to square 8.
Player 2 rolled a 2, and moved to square 29.
Player 1 rolled a 2, and moved to square 10.
Player 2 rolled a 6, and landed on a B so they slide back to square 0.
Player 1 rolled a 5, and moved to square 15.
Player 2 rolled a 4, and moved to square 4.
Player 1 rolled a 1, and landed on an F, moving forward to square 79.
Player 2 rolled a 1, and moved to square 5.
Player 1 rolled a 1, and moved to square 80.
Player 2 rolled a 6, and landed on a B so they slide back to square 0.
Player 1 rolled a 1, and moved to square 81.
Player 2 rolled a 2, and moved to square 2.
Player 1 rolled a 2, and moved to square 83.
Player 2 rolled a 5, and moved to square 7.
Player 1 rolled a 3, and moved to square 86.
Player 2 rolled a 4, and landed on a B so they slide back to square 0.
Player 1 rolled a 5, and landed on a B so they slide back to square 89.
Player 2 rolled a 5, and moved to square 5.
Player 1 rolled a 4, and landed on a ladder. They climb up 5 spaces to square 98.
Player 2 rolled a 4, and moved to square 9.
Player 1 rolled a 1, and moved to square 99.
Player 2 rolled a 1, and moved to square 10.
Player 1 rolled a 3 and reached the end of the board!
Player 2 rolled a 1, and landed on a B so they slide back to square 0.
Player 1 wins!

		Board Output
21mHk tH l B He Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
21mHk tH l B He Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 mHk -H l B1He Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk2-H l B1He Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk2-H l B He1Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -2 l B He1Flq y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -2 l B He Flq1y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk - 2l B He Flq1y H  hByHlh pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk - 2l B He Flq - H  hByHlh1pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk2-  l B H- Flq - H  hByHlh1pH B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk2-  l B H- Flq - H  hByHlh p1 B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  l2B H- Flq - H  hByHlh p1 B *js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
1 mHk -  l2B H- Flq - H  hByHlh p  B  js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
1 mHk -  l B2H- Flq - H  hByHlh p  B  js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
 1mHk -  l B2H- Flq - H  hByHlh p  B  js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
 1mHk -  l B H- Fl- -2H  hByHlh p  B  js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m1k -  l B H- Fl- -2H  hByHlh p  B  js HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m1k -  l B H- Fl- - H  hB-Hlh p  B  2s HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -1 - B H- Fl- - H  hB-Hlh p  B  2s HFB is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -1 - B H- Fl- - H  hB-Hlh p  B  js HFB2is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  -1B H- Fl- - H  hB-Hlh p  B  js HFB2is j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  -1B H- Fl- - H  hB-Hlh p  B  js2HFB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B H- Fl- - 1  hB-Hlh p  B  js2HFB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B H- Fl- - 1  hB-Hlh p  B  js 2FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B H- Fl- -   1hB-Hlh p  B  js 2FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B H- Fl- -   1hB-2lh p  B  js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B 1- Fl- -    hB-2lh p  B  js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B 1- Fl- -    hB2 -h p  B  js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  -1Fl- -    hB2 -h p  B  js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  -1Fl- -    hB- -h p2 B  js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- 1    hB- -h p2 B  js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- 1    hB- -h p  B2 js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl-1-    -B- -h p  B2 js  FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl-1-    -B- -h p  B  js 2FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- -    1B- -h p  B  js 2FB -s j H  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- -    1B- -h p  B  js  FB -s j 2  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- -    -B-1-h p  B  js  FB -s j 2  F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- -    -B-1-h p  B  js  FB -s j  2 F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  mHk -  - B  - Fl- -   1-B- -- p  B  js  FB -s j  2 F ku*gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 mHk -  - B  - Fl- -   1-B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 m1k -  - B  - Fl- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  21k -  - B  - Fl- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  2 k -  1 B  - Fl- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k2-  1 B  - Fl- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k2-  - B  -1Fl- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  2 B  -1Fl- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  2 B  - Fl- 1    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  -2B  - Fl- 1    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  -2B  - Fl- -   1-B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  2 Fl- -   1-B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  2 Fl- -    -B- -1 p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  -2F-- -    -B- -1 p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
1 m k -  - B  -2F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
1 m k -  - B  - F-- 2    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k1-  - B  - F-- 2    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k1-  - B  - F-- -  2 -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k - 1- B  - F-- -  2 -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k - 1- B  - F-- -    -B- 2- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  1 F-- -    -B- 2- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  1 F-- -    -B- -- p 2B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  - F1- -    -B- -- p 2B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 m k -  - B  - F1- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 m k -  - B  - F-- - 1  -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
 2m k -  - B  - F-- - 1  -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
12m k -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
1 m k 2  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k12  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 m k1-  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
2 m k -1 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k 21 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k 2  -1B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  21B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  2 B  -1F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B2 -1F-- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B2 - F1- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B 2- F1- -    -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B 2- F-- -  1 -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  -2F-- -  1 -B- -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  -2F-- -    -B1 -- p  B  js  FB -s j    F ku gd  H qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  - F-- -    -B1 -- p  B  js  FB -s j    F ku gd  2 qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  - F-- -    -B- -- p1 B  js  FB -s j    F ku gd  2 qjB mH x  BF i H  m*pB HlHFBhsH BB
  m k -  - B  - F-- -    -B- -- p1 B  js  FB -s j    F ku gd   2qjB mH x  BF i H  m*pB HlHFBhsH BB
1 m k -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd   2qjB mH x  BF i H  m*pB HlHFBhsH BB
1 m k -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd 2  q-B mH x  BF i H  m*pB HlHFBhsH BB
 1m - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd 2  q-B mH x  BF i H  m*pB HlHFBhsH BB
 1m - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B2mH x  BF i H  m*pB HlHFBhsH BB
  m - -1 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B2mH x  BF i H  m*pB HlHFBhsH BB
  m - -1 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B mH2x  BF i H  m*pB HlHFBhsH BB
  m - -  -1B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B mH2x  BF i H  m*pB HlHFBhsH BB
  m - -  -1B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B mH x  BF2i H  m*pB HlHFBhsH BB
  m - -  - B1 - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B mH x  BF2i H  m*pB HlHFBhsH BB
  m - -  - B1 - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B mH x  BF i 2  m*pB HlHFBhsH BB
  m - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m1 x  BF i 2  m*pB HlHFBhsH BB
2 m - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m1 x  BF i H  m pB HlHFBhsH BB
2 m - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x1 BF i H  m pB HlHFBhsH BB
  m - 2  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x1 BF i H  m pB HlHFBhsH BB
1 m - 2  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
1 m - -2 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
 1- - -2 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
 1- - -  - B2 - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  -1- -  - B2 - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  -1- -  - B 2- F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - - 1- B 2- F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - - 1- B  - F--2-    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -  - B1 - F--2-    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -  - B1 - F-- -2   -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -  - B  - F1- -2   -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
2 - - -  - B  - F1- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
2 - - -  - B  - F-- 1    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
 2- - -  - B  - F-- 1    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
 2- - -  - B  - F-- -   1-B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - -2-  - B  - F-- -   1-B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
1 - -2-  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
1 - - -  2 B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - 1 -  2 B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - 1 -  - B  -2F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -1 - B  -2F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -1 - B  - F2- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -  1 B  - F2- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -  1 B  - F-- 2    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
1 - - -  - B  - F-- 2    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
1 - - -  - B  - F-- -2   -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - -1-  - B  - F-- -2   -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - -1-  - B  - F-- -    -B2 -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - - 1- B  - F-- -    -B2 -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - - 1- B  - F-- -    -B- 2- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - - -  -1B  - F-- -    -B- 2- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
2 - - -  -1B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
2 - - -  - B  -1F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - 2 -  - B  -1F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i H  m pB HlHFBhsH BB
  - 2 -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i 1  m pB HlHFBhsH BB
  - -2-  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i 1  m pB HlHFBhsH BB
  - -2-  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i  1 m pB HlHFBhsH BB
2 - - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i  1 m pB HlHFBhsH BB
2 - - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i   1m pB HlHFBhsH BB
  2 - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i   1m pB HlHFBhsH BB
  2 - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m1pB HlHFBhsH BB
  - - -2 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m1pB HlHFBhsH BB
  - - -2 - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB1HlHFBhsH BB
2 - - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB1HlHFBhsH BB
2 - - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl1FBhsH BB
  - -2-  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl1FBhsH BB
  - -2-  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl FBh-H BB1
  - - -  2 B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl FBh-H BB1
  - - -  2 B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl FBh-H BB 1
  - - -  -2B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl FBh-H BB 1
  - - -  -2B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl FBh-H BB
2 - - -  - B  - F-- -    -B- -- p  B  js  FB -s j    F ku gd    q-B m  x  BF i    m pB Hl FBh-H BB
*/