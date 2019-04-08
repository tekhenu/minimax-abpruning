// MaxConnect-4 code

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include "gameStatus.hpp"

//function prototypes
void countScore(gameStatus &currentScoreGame);

//ai play minimax, minPlayer, maxPlayer, orderMoves, evalfunction  *****TODO********


// Output current game status to console
void printGameBoard(gameStatus &currentPrintGame)
{
  int i = 0;
  int j = 0;
  printf(" -----------------\n");
  for(i = 0; i < 6; i++)
  {	
    printf(" | ");
    for(j = 0; j < 7; j++)
    {
      printf("%li ", currentPrintGame.gameBoard[i][j]);
    }
    printf("| \n");
  }
  printf(" -----------------\n");
}

// Output current game status to file
void printGameBoardToFile(gameStatus &currentFileGame)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 6; i++)
	{	
		for(j = 0; j < 7; j++)
		{
			fprintf(currentFileGame.gameFile, "%li", currentFileGame.gameBoard[i][j]);
		}
		fprintf(currentFileGame.gameFile, "\r\n");
	}
	fprintf(currentFileGame.gameFile, "%ld\r\n", currentFileGame.currentTurn);
}

// Given a column and which player it is, 
// place that player's piece in the requested column
int playPiece(int column, gameStatus &currentPlayGame){
	// if column full, return 0
        if(column < 0) return 0;
	if(currentPlayGame.gameBoard[0][column] != 0){
		return 0;
        }
	int i;
	
        // starting at the bottom of the board, place the piece into the first empty spot
	for(i = 5; i >= 0; i--){
		if(currentPlayGame.gameBoard[i][column] == 0){
			currentPlayGame.gameBoard[i][column] = currentPlayGame.currentTurn;
			currentPlayGame.pieceCount++;
			return 1;
		}
	}
  return 0;
}

int checkPlay(int column, gameStatus &currentCheckPlay){
	// if column full, return 0
        if(column < 0) return 0;
	if(currentCheckPlay.gameBoard[0][column] != 0){
		return 0;
        }
	int i;
	
        // starting at the bottom of the board, check for the first empty spot
	for(i = 5; i >= 0; i--){
		if(currentCheckPlay.gameBoard[i][column] == 0){
			return 1;
		}
	}
  return 0;
}


// The AI section.  Currently plays randomly.
void aiPlayRand(gameStatus &currentGame)
{	
	int randColumn = (int) rand() % 7;
	int result = 0;
	result = playPiece(randColumn, currentGame);
	if(result == 0){
	    aiPlayRand(currentGame);
        } else {
           printf("\n\nmove %d: Player %li, column %d\n", 
           currentGame.pieceCount, currentGame.currentTurn, randColumn+1);
		if(currentGame.currentTurn == 1)
			currentGame.currentTurn = 2;
		else if (currentGame.currentTurn == 2)
			currentGame.currentTurn = 1;
  }
}



int main(int argc, char ** argv)
{
  char ** command_line = argv;

  if (argc != 5)
  {
    printf("Four command-line arguments are needed:\n");
    printf("Usage: %s interactive [input_file] [computer-next/human-next] [depth]\n", command_line [0]);
    printf("or:  %s one-move [input_file] [output_file] [depth]\n", command_line [0]);

    return 0;
  }

  char * game_mode = command_line [1];

  if (strcmp(game_mode, "interactive") == 0){
        //get command line inputs for next to move and file
        char * input = command_line[2]; 
        char * next_to_move = command_line [3];
        char  humanOut[10] = "human.txt";
        char  cpuOut[13] = "computer.txt";
        char  humanTurn[11] = "human-next";
        char  cpuTurn[14] = "computer-next";
        long human(0), cpu(0);
        gameStatus currentGame;	  
        currentGame.gameFile = fopen(input, "r");
          char current = 0;
          int i, j;
          if (currentGame.gameFile != 0){
            for(i = 0; i < 6; i++){
              for(j = 0; j < 7; j++){
                do {
                  current = getc(currentGame.gameFile);
                } while ((current == ' ') || (current == '\n') || (current == '\r'));

                currentGame.gameBoard[i][j] = current - 48;
                if(currentGame.gameBoard[i][j] > 0){
                    currentGame.pieceCount++;
                }
              }
            }
         
            do {
              current = getc(currentGame.gameFile);
            } while ((current == ' ') || (current == '\n') || (current == '\r'));
            
            currentGame.currentTurn = current - 48;
            fclose(currentGame.gameFile);
          }


        if(strcmp(next_to_move, humanTurn) == 0){ 
            human = currentGame.currentTurn;
            if(currentGame.currentTurn == 1) cpu = 2;
            else cpu = 1;
        } else if (strcmp(next_to_move, cpuTurn) == 0){ 
            cpu = currentGame.currentTurn;
            if(currentGame.currentTurn == 1) human = 2;
            else human = 1;
        }
       while(currentGame.pieceCount < 42){
           //print board before move and the score
            printf("\nMaxConnect-4 game\n");
            printf("Current Board:\n");
            printGameBoard(currentGame);
            countScore(currentGame);
            printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
    
            if(strcmp(next_to_move, "computer-next") == 0){
                currentGame.currentTurn = cpu;
                std::cout << "CPU plays:" << std::endl;    
                aiPlayRand(currentGame);
                currentGame.gameFile = fopen(cpuOut, "w");
                currentGame.currentTurn = human;
                if (currentGame.gameFile != 0){
                    printGameBoardToFile(currentGame);
                    fclose(currentGame.gameFile);
                } else{
                    printf("error: could not open output file %s\n", cpuOut);
                }
                next_to_move = humanTurn;
            } else if (strcmp(next_to_move, "human-next") == 0){
                //make sure that it wont break if input file has current turn set with one number but human/comp player set at prompt to be opposite
                currentGame.currentTurn = human;
                int move(-1);
                std::cout << "3n73r y0ur m0v3, hum4n...(column number 0 - 6)" << std::endl;
                std::cin >> move;
                while((move < 0 || move > 6) || (playPiece(move, currentGame) != 1)){
                    std::cout << "Invalid move, or column full...(column number 0 - 6)" << std::endl;
                    std::cin >> move;
                }
                currentGame.gameFile = fopen(humanOut, "w");
                currentGame.currentTurn = cpu;
                if (currentGame.gameFile != 0){
                    printGameBoardToFile(currentGame);
                    fclose(currentGame.gameFile);
                } else{
                    printf("error: could not open output file %s\n", humanOut);
                }
                next_to_move = cpuTurn;
            } else {
                std::cout << "please enter 'computer-next' or 'human-next' for this turn" << std::endl;
            }
        }
    printf("\nMaxConnect-4 game\n");
    printf("game state before move:\n");
    printGameBoard(currentGame);
    printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
     return 0;
  
    }  else if (strcmp (game_mode, "one-move") != 0) {
    printf("%s is an unrecognized game mode\n", game_mode);
    return 0;
  }


  char * input = command_line[2];
  char * output = command_line [3];

  gameStatus currentGame;	 // Declare current game
  printf("\nMaxConnect-4 game\n");

  currentGame.gameFile = fopen(input, "r");
  printf("game state before move:\n");
  
  //set currentTurn
  char current = 0;
  int i, j;
  if (currentGame.gameFile != 0){
    for(i = 0; i < 6; i++){
      for(j = 0; j < 7; j++){
        do {
          current = getc(currentGame.gameFile);
        } while ((current == ' ') || (current == '\n') || (current == '\r'));

        currentGame.gameBoard[i][j] = current - 48;
	if(currentGame.gameBoard[i][j] > 0){
	    currentGame.pieceCount++;
	}
      }
    }
 
    do {
      current = getc(currentGame.gameFile);
    } while ((current == ' ') || (current == '\n') || (current == '\r'));
    
    currentGame.currentTurn = current - 48;
    fclose(currentGame.gameFile);
  }

  printGameBoard(currentGame);
  countScore(currentGame);
  printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);

  // Seed random number generator
  int seed = time(NULL);
  srand(seed);

  if(currentGame.pieceCount == 42)
  {
    printf("\nBOARD FULL\n");
    printf("Game over!\n\n");

    return 1;
  }

  aiPlayRand(currentGame);
  printf("game state after move:\n");
  printGameBoard(currentGame);
  countScore(currentGame);
  printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
  
  currentGame.gameFile = fopen(output, "w");
  if (currentGame.gameFile != 0)
  {
    printGameBoardToFile(currentGame);
    fclose(currentGame.gameFile);
  }
  else
  {
    printf("error: could not open output file %s\n", output);
  }

  return 1;
}
void countScore(gameStatus &currentScoreGame)
{
  currentScoreGame.player1Score = 0; 
  currentScoreGame.player2Score = 0; 

  //check horizontally
  int i;
  for(i = 0; i < 6; i++)
  {
		//check player 1
		if(currentScoreGame.gameBoard[i][0] == 1 && currentScoreGame.gameBoard[i][1] == 1 
			&& currentScoreGame.gameBoard[i][2] == 1 && currentScoreGame.gameBoard[i][3] == 1)
			{currentScoreGame.player1Score++;}
		if(currentScoreGame.gameBoard[i][1] == 1 && currentScoreGame.gameBoard[i][2] == 1 
			&& currentScoreGame.gameBoard[i][3] == 1 && currentScoreGame.gameBoard[i][4] == 1)
			{currentScoreGame.player1Score++;}
		if(currentScoreGame.gameBoard[i][2] == 1 && currentScoreGame.gameBoard[i][3] == 1 
			&& currentScoreGame.gameBoard[i][4] == 1 && currentScoreGame.gameBoard[i][5] == 1)
			{currentScoreGame.player1Score++;}
		if(currentScoreGame.gameBoard[i][3] == 1 && currentScoreGame.gameBoard[i][4] == 1 
			&& currentScoreGame.gameBoard[i][5] == 1 && currentScoreGame.gameBoard[i][6] == 1)
			{currentScoreGame.player1Score++;}
		//check player 2
		if(currentScoreGame.gameBoard[i][0] == 2 && currentScoreGame.gameBoard[i][1] == 2 
			&& currentScoreGame.gameBoard[i][2] == 2 && currentScoreGame.gameBoard[i][3] == 2)
			{currentScoreGame.player2Score++;}
		if(currentScoreGame.gameBoard[i][1] == 2 && currentScoreGame.gameBoard[i][2] == 2 
			&& currentScoreGame.gameBoard[i][3] == 2 && currentScoreGame.gameBoard[i][4] == 2)
			{currentScoreGame.player2Score++;}
		if(currentScoreGame.gameBoard[i][2] == 2 && currentScoreGame.gameBoard[i][3] == 2 
			&& currentScoreGame.gameBoard[i][4] == 2 && currentScoreGame.gameBoard[i][5] == 2)
			{currentScoreGame.player2Score++;}
		if(currentScoreGame.gameBoard[i][3] == 2 && currentScoreGame.gameBoard[i][4] == 2 
			&& currentScoreGame.gameBoard[i][5] == 2 && currentScoreGame.gameBoard[i][6] == 2)
			{currentScoreGame.player2Score++;}
	}

	//check vertically
	int j;
	for(j = 0; j < 7; j++)
	{
		//check player 1
		if(currentScoreGame.gameBoard[0][j] == 1 && currentScoreGame.gameBoard[1][j] == 1 
			&& currentScoreGame.gameBoard[2][j] == 1 && currentScoreGame.gameBoard[3][j] == 1)
			{currentScoreGame.player1Score++;}
		if(currentScoreGame.gameBoard[1][j] == 1 && currentScoreGame.gameBoard[2][j] == 1 
			&& currentScoreGame.gameBoard[3][j] == 1 && currentScoreGame.gameBoard[4][j] == 1)
			{currentScoreGame.player1Score++;}
		if(currentScoreGame.gameBoard[2][j] == 1 && currentScoreGame.gameBoard[3][j] == 1 
			&& currentScoreGame.gameBoard[4][j] == 1 && currentScoreGame.gameBoard[5][j] == 1)
			{currentScoreGame.player1Score++;}
		//check player 2
		if(currentScoreGame.gameBoard[0][j] == 2 && currentScoreGame.gameBoard[1][j] == 2 
			&& currentScoreGame.gameBoard[2][j] == 2 && currentScoreGame.gameBoard[3][j] == 2)
			{currentScoreGame.player2Score++;}
		if(currentScoreGame.gameBoard[1][j] == 2 && currentScoreGame.gameBoard[2][j] == 2 
			&& currentScoreGame.gameBoard[3][j] == 2 && currentScoreGame.gameBoard[4][j] == 2)
			{currentScoreGame.player2Score++;}
		if(currentScoreGame.gameBoard[2][j] == 2 && currentScoreGame.gameBoard[3][j] == 2 
			&& currentScoreGame.gameBoard[4][j] == 2 && currentScoreGame.gameBoard[5][j] == 2)
			{currentScoreGame.player2Score++;}
	}

	//check diagonally
	
	//check player 1
	if(currentScoreGame.gameBoard[2][0] == 1 && currentScoreGame.gameBoard[3][1] == 1 
		&& currentScoreGame.gameBoard[4][2] == 1 && currentScoreGame.gameBoard[5][3] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][0] == 1 && currentScoreGame.gameBoard[2][1] == 1 
		&& currentScoreGame.gameBoard[3][2] == 1 && currentScoreGame.gameBoard[4][3] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][1] == 1 && currentScoreGame.gameBoard[3][2] == 1 
		&& currentScoreGame.gameBoard[4][3] == 1 && currentScoreGame.gameBoard[5][4] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][0] == 1 && currentScoreGame.gameBoard[1][1] == 1 
		&& currentScoreGame.gameBoard[2][2] == 1 && currentScoreGame.gameBoard[3][3] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][1] == 1 && currentScoreGame.gameBoard[2][2] == 1 
		&& currentScoreGame.gameBoard[3][3] == 1 && currentScoreGame.gameBoard[4][4] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][2] == 1 && currentScoreGame.gameBoard[3][3] == 1 
		&& currentScoreGame.gameBoard[4][4] == 1 && currentScoreGame.gameBoard[5][5] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][1] == 1 && currentScoreGame.gameBoard[1][2] == 1 
		&& currentScoreGame.gameBoard[2][3] == 1 && currentScoreGame.gameBoard[3][4] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][2] == 1 && currentScoreGame.gameBoard[2][3] == 1 
		&& currentScoreGame.gameBoard[3][4] == 1 && currentScoreGame.gameBoard[4][5] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][3] == 1 && currentScoreGame.gameBoard[3][4] == 1 
		&& currentScoreGame.gameBoard[4][5] == 1 && currentScoreGame.gameBoard[5][6] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][2] == 1 && currentScoreGame.gameBoard[1][3] == 1 
		&& currentScoreGame.gameBoard[2][4] == 1 && currentScoreGame.gameBoard[3][5] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][3] == 1 && currentScoreGame.gameBoard[2][4] == 1 
		&& currentScoreGame.gameBoard[3][5] == 1 && currentScoreGame.gameBoard[4][6] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][3] == 1 && currentScoreGame.gameBoard[1][4] == 1 
		&& currentScoreGame.gameBoard[2][5] == 1 && currentScoreGame.gameBoard[3][6] == 1)
			{currentScoreGame.player1Score++;}

	if(currentScoreGame.gameBoard[0][3] == 1 && currentScoreGame.gameBoard[1][2] == 1 
		&& currentScoreGame.gameBoard[2][1] == 1 && currentScoreGame.gameBoard[3][0] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][4] == 1 && currentScoreGame.gameBoard[1][3] == 1 
		&& currentScoreGame.gameBoard[2][2] == 1 && currentScoreGame.gameBoard[3][1] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][3] == 1 && currentScoreGame.gameBoard[2][2] == 1 
		&& currentScoreGame.gameBoard[3][1] == 1 && currentScoreGame.gameBoard[4][0] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][5] == 1 && currentScoreGame.gameBoard[1][4] == 1 
		&& currentScoreGame.gameBoard[2][3] == 1 && currentScoreGame.gameBoard[3][2] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][4] == 1 && currentScoreGame.gameBoard[2][3] == 1 
		&& currentScoreGame.gameBoard[3][2] == 1 && currentScoreGame.gameBoard[4][1] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][3] == 1 && currentScoreGame.gameBoard[3][2] == 1 
		&& currentScoreGame.gameBoard[4][1] == 1 && currentScoreGame.gameBoard[5][0] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[0][6] == 1 && currentScoreGame.gameBoard[1][5] == 1 
		&& currentScoreGame.gameBoard[2][4] == 1 && currentScoreGame.gameBoard[3][3] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][5] == 1 && currentScoreGame.gameBoard[2][4] == 1 
		&& currentScoreGame.gameBoard[3][3] == 1 && currentScoreGame.gameBoard[4][2] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][4] == 1 && currentScoreGame.gameBoard[3][3] == 1 
		&& currentScoreGame.gameBoard[4][2] == 1 && currentScoreGame.gameBoard[5][1] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[1][6] == 1 && currentScoreGame.gameBoard[2][5] == 1 
		&& currentScoreGame.gameBoard[3][4] == 1 && currentScoreGame.gameBoard[4][3] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][5] == 1 && currentScoreGame.gameBoard[3][4] == 1 
		&& currentScoreGame.gameBoard[4][3] == 1 && currentScoreGame.gameBoard[5][2] == 1)
			{currentScoreGame.player1Score++;}
	if(currentScoreGame.gameBoard[2][6] == 1 && currentScoreGame.gameBoard[3][5] == 1 
		&& currentScoreGame.gameBoard[4][4] == 1 && currentScoreGame.gameBoard[5][3] == 1)
			{currentScoreGame.player1Score++;}
		
	//check player 2
	if(currentScoreGame.gameBoard[2][0] == 2 && currentScoreGame.gameBoard[3][1] == 2 
		&& currentScoreGame.gameBoard[4][2] == 2 && currentScoreGame.gameBoard[5][3] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][0] == 2 && currentScoreGame.gameBoard[2][1] == 2 
		&& currentScoreGame.gameBoard[3][2] == 2 && currentScoreGame.gameBoard[4][3] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][1] == 2 && currentScoreGame.gameBoard[3][2] == 2 
		&& currentScoreGame.gameBoard[4][3] == 2 && currentScoreGame.gameBoard[5][4] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][0] == 2 && currentScoreGame.gameBoard[1][1] == 2 
		&& currentScoreGame.gameBoard[2][2] == 2 && currentScoreGame.gameBoard[3][3] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][1] == 2 && currentScoreGame.gameBoard[2][2] == 2 
		&& currentScoreGame.gameBoard[3][3] == 2 && currentScoreGame.gameBoard[4][4] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][2] == 2 && currentScoreGame.gameBoard[3][3] == 2 
		&& currentScoreGame.gameBoard[4][4] == 2 && currentScoreGame.gameBoard[5][5] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][1] == 2 && currentScoreGame.gameBoard[1][2] == 2 
		&& currentScoreGame.gameBoard[2][3] == 2 && currentScoreGame.gameBoard[3][4] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][2] == 2 && currentScoreGame.gameBoard[2][3] == 2 
		&& currentScoreGame.gameBoard[3][4] == 2 && currentScoreGame.gameBoard[4][5] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][3] == 2 && currentScoreGame.gameBoard[3][4] == 2 
		&& currentScoreGame.gameBoard[4][5] == 2 && currentScoreGame.gameBoard[5][6] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][2] == 2 && currentScoreGame.gameBoard[1][3] == 2 
		&& currentScoreGame.gameBoard[2][4] == 2 && currentScoreGame.gameBoard[3][5] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][3] == 2 && currentScoreGame.gameBoard[2][4] == 2 
		&& currentScoreGame.gameBoard[3][5] == 2 && currentScoreGame.gameBoard[4][6] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][3] == 2 && currentScoreGame.gameBoard[1][4] == 2 
		&& currentScoreGame.gameBoard[2][5] == 2 && currentScoreGame.gameBoard[3][6] == 2)
			{currentScoreGame.player2Score++;}

	if(currentScoreGame.gameBoard[0][3] == 2 && currentScoreGame.gameBoard[1][2] == 2 
		&& currentScoreGame.gameBoard[2][1] == 2 && currentScoreGame.gameBoard[3][0] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][4] == 2 && currentScoreGame.gameBoard[1][3] == 2 
		&& currentScoreGame.gameBoard[2][2] == 2 && currentScoreGame.gameBoard[3][1] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][3] == 2 && currentScoreGame.gameBoard[2][2] == 2
		&& currentScoreGame.gameBoard[3][1] == 2 && currentScoreGame.gameBoard[4][0] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][5] == 2 && currentScoreGame.gameBoard[1][4] == 2 
		&& currentScoreGame.gameBoard[2][3] == 2 && currentScoreGame.gameBoard[3][2] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][4] == 2 && currentScoreGame.gameBoard[2][3] == 2 
		&& currentScoreGame.gameBoard[3][2] == 2 && currentScoreGame.gameBoard[4][1] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][3] == 2 && currentScoreGame.gameBoard[3][2] == 2 
		&& currentScoreGame.gameBoard[4][1] == 2 && currentScoreGame.gameBoard[5][0] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[0][6] == 2 && currentScoreGame.gameBoard[1][5] == 2 
		&& currentScoreGame.gameBoard[2][4] == 2 && currentScoreGame.gameBoard[3][3] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][5] == 2 && currentScoreGame.gameBoard[2][4] == 2 
		&& currentScoreGame.gameBoard[3][3] == 2 && currentScoreGame.gameBoard[4][2] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][4] == 2 && currentScoreGame.gameBoard[3][3] == 2 
		&& currentScoreGame.gameBoard[4][2] == 2 && currentScoreGame.gameBoard[5][1] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[1][6] == 2 && currentScoreGame.gameBoard[2][5] == 2 
		&& currentScoreGame.gameBoard[3][4] == 2 && currentScoreGame.gameBoard[4][3] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][5] == 2 && currentScoreGame.gameBoard[3][4] == 2 
		&& currentScoreGame.gameBoard[4][3] == 2 && currentScoreGame.gameBoard[5][2] == 2)
			{currentScoreGame.player2Score++;}
	if(currentScoreGame.gameBoard[2][6] == 2 && currentScoreGame.gameBoard[3][5] == 2 
		&& currentScoreGame.gameBoard[4][4] == 2 && currentScoreGame.gameBoard[5][3] == 2)
			{currentScoreGame.player2Score++;}
}

