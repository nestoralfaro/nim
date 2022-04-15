// playNIM.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playNIM() which is defined below
#include "NIM.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

//Convert two chars to a single int 
int convertToNum(char Uno, char Dos) {
	int integ = (Uno - '0') * 10 + (Dos - '0');
	return integ;
}

//Validate Request Data coming in
int ValidateData(amountToRemove) {
	if (NimBoard1.rows[Pile] < amountToRemove) {
		amountToRemove = NimBoard1.rows[Pile];
	}
	if (NimBoard1.rows[Pile] < 1) {
		amountToRemove = 1;
	}

	return amountToRemove;
}

//String that comes thru
void initializeBoard(char boardDetails)
{
	cout << str.at(5);
	int board [];
}

//Update with the values of the board
void updateBoard(int Pile, int amountToRemove)
{
	amountToRemove = ValidateData(amountToRemove);
	NimBoard1.rows[Pile] = NimBoard1.rows[Pile] - 1;
}

void NumberOfSticksInRowI(int i) {
	return sticks[HOWEVER YOU GET IT];
}

void displayBoard(int NumberOfRows)
{
	std::cout << std::endl;
	//Load the number of rows out
	for (int i = 0; i <= NumberOfRows; i++) {
  		//Load row
		std::cout << "Row " << i << ": ";
		
		for (int i = 0; i <= NumberOfSticksInRowI(i) ; i++) {
			//Number of sticks in the row
			cout << " | ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}



int check4Win(char board[10])
{
	int winner = noWinner;

	// Check for vertical winners
	int i = 1;
	while (winner == noWinner && i < 4) {
		if (board[i] == board[i + 3] && board[i] == board[i + 6]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i++;
	}

	// Check for horizontal winners
	i = 1;
	while (winner == noWinner && i < 8) {
		if (board[i] == board[i + 1] && board[i] == board[i + 2]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i += 3;
	}

	// Check for diagonal winners
	if (winner == noWinner) {
		if ((board[1] == board[5] && board[1] == board[9]) ||
			(board[3] == board[5] && board[3] == board[7])) {
			winner = (board[5] == 'X') ? xWinner : oWinner;
		}
	}

	// Check for tie
	i = 1;
	int numMoves = 0;
	while (i < 10) {
		if ((board[i] == 'X' || board[i] == 'O')) {
			numMoves++;
		}
		i++;
	}
	if (winner == noWinner && numMoves == 9)
		winner = TIE;


	return winner;
}

int getMove(char board[10], int player)
{
	int move;
	char move_str[80];

	std::cout << "Where do you want to place your ";
	char mark = (player == X_PLAYER) ? 'X' : 'O';
	std::cout << mark << "? " << std::endl;

	do {
		std::cout << "Your move? ";
		std::cin >> move_str;
		move = atoi(move_str);
		if (board[move] == 'X' || board[move] == 'O') move = 0;
	} while (move < 1 || move > 9);

	return move;
}

int playNIM(SOCKET s, std::string serverName, std::string host, std::string port, int player)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	char boardNIM[10];
	int opponent;
	int move;
	bool myMove;

	if (player == Serv_PLAYER) {
		std::cout << "Playing as Host" << std::endl;
		opponent = Client_PLAYER;
		myMove = true;
	}
	else {
		std::cout << "Playing as Client" << std::endl;
		opponent = Serv_PLAYER;
		myMove = false;
	}

	initializeBoard(board);
	displayBoard(board);

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board
			move = getMove(board, player);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(board, move, player);
			displayBoard(board);

			// Send move to opponent
/*****
	Task 1:	move is an integer that was assigned a value (from 1 to 9) in the
			previous code segment. Add code here to convert move to a null-terminated
			C-string and send it to your opponent.
*****/

			char movecstr[4] = "";
			_itoa_s(move, movecstr, 10);

			UDP_send(s, movecstr, strlen(movecstr) + 1, (char*)host.c_str(), (char*)port.c_str());

		}
		else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			int status = wait(s, 20, 0);
			if (status > 0) {
				
				char movecstr[4] = "";
				UDP_recv(s, movecstr, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				move = atoi(movecstr);

				int pileNum;
				int rockNum;
				rockNum = move % 100;
				pileNum = move / 100;
				

				updateBoard(pileNum, rockNum);

				/*****
							(iii) call a function that will display the updated board on your screen
				*****/
				displayBoard(board);


			}
			else {
				winner = ABORT;
			}
		}
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << "No response from opponent.  Aborting the game..." << std::endl;
		}
		else {
			winner = check4Win(board);
		}

		if (winner == player)
			std::cout << "You WIN!" << std::endl;
		else if (winner == TIE)
			std::cout << "It's a tie." << std::endl;
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}
