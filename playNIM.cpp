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
int ValidateData(NimBoard board, int amountToRemove, int pileNum, int player) {
	int winner = noWinner;

	if (board.getBoard()[pileNum] < amountToRemove) {
		winner = player;
	}
	if (board.getBoard()[pileNum] < 1) {
		winner = player;
	}

	return winner;
}

//String that comes thru
//void initializeBoard(char boardDetails)
//{
//	cout << str.at(5);
//	int board[];
//}

//Update with the values of the board
int updateBoard(NimBoard board, Move move, int player)
{
	int winner = noWinner;
	winner = ValidateData(board, move.amountToRemove, move.pile, player);
	board.getBoard()[move.pile] = board.getBoard()[move.pile] - move.amountToRemove;
	return winner;
}

//void NumberOfSticksInRowI(int i) {
//	return sticks[HOWEVER YOU GET IT];
//}

void displayBoard(NimBoard board)
{
	std::cout << std::endl;
	for (const auto& pile : board.getBoard()) {
		std::cout << "Row " << pile << ": ";

		for (int rock = 0; rock < pile; ++rock) {
			std::cout << " | ";
		}
	}
	////Load the number of rows out
	//for (int i = 0; i <= NumberOfRows; i++) {
	//	//Load row
	//	std::cout << "Row " << i << ": ";

	//	for (int i = 0; i <= NumberOfSticksInRowI(i); i++) {
	//		//Number of sticks in the row
	//		cout << " | ";
	//	}

	//	std::cout << std::endl;
	//}

	std::cout << std::endl;
}



int check4Win(std::vector<int> board)
{
	int winner = noWinner;

	// Check for vertical winners
	int i = 1;
	while (winner == noWinner && i < 4) {
		if (board[i] == board[i + 3] && board[i] == board[i + 6]) {
			winner = (board[i] == 'X') ? ServWinner : ClientWinner;
		}
		i++;
	}

	// Check for horizontal winners
	i = 1;
	while (winner == noWinner && i < 8) {
		if (board[i] == board[i + 1] && board[i] == board[i + 2]) {
			winner = (board[i] == 'X') ? ServWinner : ClientWinner;
		}
		i += 3;
	}

	// Check for diagonal winners
	if (winner == noWinner) {
		if ((board[1] == board[5] && board[1] == board[9]) ||
			(board[3] == board[5] && board[3] == board[7])) {
			winner = (board[5] == 'X') ? ServWinner : ClientWinner;
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



Move getMove(NimBoard board, int player)
{
	int move;
	std::string move_str;
	int pile;
	int amountToRemove;
	std::string rocksToRemove;
	std::cout << "You are playing as the";
	//	X		O
	std::string mark = (player == Client_PLAYER) ? "server" : "client";
	std::cout << mark << "? " << std::endl;

	do {
		std::cout << "Your move? ";
		std::cin >> move_str;
		rocksToRemove = move_str[1] + move_str[2];
		pile = (move_str[0] - '0');
		amountToRemove = atoi(rocksToRemove.c_str());
		//if pile has no rocks
	} while (board.getBoard()[pile] == 0);

	return {move_str, pile, amountToRemove};
}

int playNIM(SOCKET s, std::string serverName, std::string host, std::string port, int player)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	//NimBoard board;
	int opponent;
	NimBoard board;
	Move move;
	bool myMove;
	std::string boardConfig;
	char newline;

	if (player == Serv_PLAYER) {
		std::cout << "Playing as Host" << std::endl;
		std::cout << "Please enter initial pile configuration (# of Piles (2 digits) # of rocks for each pile (2 digits each)" << std::endl;
		std::cin >> boardConfig;
		std::cin.get(newline);
		board.setBoard(boardConfig);
		opponent = Client_PLAYER;
		myMove = true;
	}
	else {
		std::cout << "Playing as Client" << std::endl;
		opponent = Serv_PLAYER;
		myMove = false;
	}

	//initializeBoard(board);
	displayBoard(board);

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board
			move = getMove(board, player);
			std::cout << "Board after your move:" << std::endl;
			winner = updateBoard(board, move, player); // Not yet compatible with the updateBoard funciton
			displayBoard(board);

			// Send move to opponent
/*****
	Task 1:	move is an integer that was assigned a value (from 1 to 9) in the
			previous code segment. Add code here to convert move to a null-terminated
			C-string and send it to your opponent.
*****/

			//char movecstr[4] = "";
			//_itoa_s(move, movecstr, 10);

			//UDP_send(s, movecstr, strlen(movecstr) + 1, (char*)host.c_str(), (char*)port.c_str());
			UDP_send(s, (char*)move.move.c_str(), strlen(move.move.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());

		}
		else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			int status = wait(s, 120, 0);
			if (status > 0) {

				char movecstr[4] = "";
				UDP_recv(s, movecstr, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				//move = atoi(movecstr);

				//int pileNum;
				//int rockNum;
				//rockNum = move % 100;
				//pileNum = move / 100;


				//winner = updateBoard(board, pileNum, rockNum, player);
				move = getMove(board, player);
				winner = updateBoard(board, move, player);

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
			winner = check4Win(board.getBoard());
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
