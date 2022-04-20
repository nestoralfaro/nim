// playNIM.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playNIM() which is defined below
#include "NIM.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

//Convert two chars to a single int 
// int convertToNum(char Uno, char Dos) {
// 	int integ = (Uno - '0') * 10 + (Dos - '0');
// 	return integ;
// }

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
//Now as a method of NimBoard
// int updateBoard(NimBoard board, Move move, int player)
// {
// 	int winner = noWinner;
// 	winner = ValidateData(board, move.amountToRemove, move.pile, player);
// 	board.getBoard()[move.pile] = board.getBoard()[move.pile] - move.amountToRemove;
// 	return winner;
// }

//void NumberOfSticksInRowI(int i) {
//	return sticks[HOWEVER YOU GET IT];
//}

//Now as a method of NimBoard
// void displayBoard(NimBoard board)
// {
// 	int row = 1;
// 	std::cout << std::endl;
// 	for (const auto& pile : board.getBoard()) {
// 		std::cout << "Row " << row++ << ": ";

// 		for (int rock = 0; rock < pile; ++rock) {
// 			std::cout << " | ";
// 		}
// 	}
// 	////Load the number of rows out
// 	//for (int i = 0; i <= NumberOfRows; i++) {
// 	//	//Load row
// 	//	std::cout << "Row " << i << ": ";

// 	//	for (int i = 0; i <= NumberOfSticksInRowI(i); i++) {
// 	//		//Number of sticks in the row
// 	//		cout << " | ";
// 	//	}

// 	//	std::cout << std::endl;
// 	//}

// 	std::cout << std::endl;
// }


// Now as a method of the NimBoard object
// int check4Win(std::vector<int> board)
// {
// 	int winner = noWinner;

// 	// Check for vertical winners
// 	int i = 1;
// 	while (winner == noWinner && i < 4) {
// 		if (board[i] == board[i + 3] && board[i] == board[i + 6]) {
// 			winner = (board[i] == 'X') ? ServWinner : ClientWinner;
// 		}
// 		i++;
// 	}

// 	// Check for horizontal winners
// 	i = 1;
// 	while (winner == noWinner && i < 8) {
// 		if (board[i] == board[i + 1] && board[i] == board[i + 2]) {
// 			winner = (board[i] == 'X') ? ServWinner : ClientWinner;
// 		}
// 		i += 3;
// 	}

// 	// Check for diagonal winners
// 	if (winner == noWinner) {
// 		if ((board[1] == board[5] && board[1] == board[9]) ||
// 			(board[3] == board[5] && board[3] == board[7])) {
// 			winner = (board[5] == 'X') ? ServWinner : ClientWinner;
// 		}
// 	}

// 	// Check for tie
// 	i = 1;
// 	int numMoves = 0;
// 	while (i < 10) {
// 		if ((board[i] == 'X' || board[i] == 'O')) {
// 			numMoves++;
// 		}
// 		i++;
// 	}
// 	if (winner == noWinner && numMoves == 9)
// 		winner = TIE;


// 	return winner;
// }


// Now as a method of the NimBoard object
// Move getMove(NimBoard board, int player)
// {
// 	int move;
// 	std::string move_str;
// 	int pile;
// 	int amountToRemove;
// 	//std::string rocksToRemove;
// 	std::cout << "You are playing as the";
// 	//	X		O
// 	std::string mark = (player == Client_PLAYER) ? "client" : "server";
// 	std::cout << mark << std::endl;

// 	do {
// 		std::cout << "Your move? ";
// 		std::cin >> move_str;
// 		pile = (move_str[0] - '0');
// 		amountToRemove = (move_str[1] - '0') * 10 + (move_str[2] - '0');
// 		//if pile has no rocks
// 	} while (board.getBoard()[pile - 1] == 0);

// 	return {move_str, pile, amountToRemove};
// }

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
	bool boardIsConfigured;
	std::string boardConfig;
	char newline;

	if (player == Serv_PLAYER) {
		std::cout << "Playing as Host" << std::endl;
		std::cout << "Please enter initial pile configuration (# of Piles (2 digits) # of rocks for each pile (2 digits each)" << std::endl;
		std::cin >> boardConfig;
		std::cin.get(newline);
		board.setBoard(boardConfig);
		opponent = Client_PLAYER;
		myMove = false;
		boardIsConfigured = true;
		board.displayBoard();
		UDP_send(s, (char*)boardConfig.c_str(), strlen(boardConfig.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
	}
	else {
		std::cout << "Playing as Client" << std::endl;
		opponent = Serv_PLAYER;
		boardIsConfigured = false;
		myMove = true;
	}

	while (winner == noWinner) {

		if (boardIsConfigured) {
			if (myMove) {
				move = board.getMove(player);
				while (move.moveString[0] == 'C') {
					UDP_send(s, (char*)move.moveString.c_str(), strlen(move.moveString.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
					move = board.getMove(player);
				}
				if (move.moveString == "F") {
					UDP_send(s, (char*)move.moveString.c_str(), strlen(move.moveString.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
					winner = opponent;
					std::cout << "You forfeited!!" << std::endl;
				}
				else {
					std::cout << "Board after your move:" << std::endl;
					board.updateBoard(move.pile, move.amountToRemove);
					board.displayBoard();
					UDP_send(s, (char*)move.moveString.c_str(), strlen(move.moveString.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
				}
				
			}
			else {
				std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
				//Get opponent's move & display board
				int status = wait(s, 120, 0);
				if (status > 0) {
					char movecstr[4];
					UDP_recv(s, movecstr, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
					board.updateBoard(movecstr);
					std::cout << "Board after your opponent's move" << std::endl;
					board.displayBoard();
				}
				else {
					winner = ABORT;
				}
			}
		}
		else {
			std::cout << "Waiting for board configuration..." << std::endl << std::endl;
			int status = wait(s, 120, 0);
			if (status > 0) {
				myMove = false;
				boardIsConfigured = true;
				char boardConf[80];
				UDP_recv(s, boardConf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				board.setBoard(boardConf);
				board.displayBoard();
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
			winner = board.check4win();
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
