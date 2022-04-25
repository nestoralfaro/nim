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
	std::string boardConfig = "";
	char newline;

	if (player == Serv_PLAYER) {
		std::cout << "Playing as Host" << std::endl;
		std::cout << "Please enter the number of piles you'd like to play with, 3-9" << std::endl;
		std::string PileNum = "";
		std::cin >> PileNum;
		while ((PileNum[0] - '0') > 9 || (PileNum[0] - '0') < 3 || PileNum[1] != NULL) {
			PileNum = "";
			std::cout << "The number you've input is invalid, please try again with a number 3-9" << std::endl;
			std::cin >> PileNum;
		}
		boardConfig += PileNum;
		for (int i = 0; i < PileNum[0] - '0'; i++) {
			std::cout << "Please choose how many rocks, 1-20 you would like in pile " << i + 1 << std::endl;
			std::string RockNum = "";
			std::cin >> RockNum;
			if (RockNum[1] == NULL) {
				RockNum = '0' + RockNum;
			}
			while (((RockNum[0] - '0') * 10 + (RockNum[1] - '0')) > 20 || ((RockNum[0] - '0') * 10 + (RockNum[1] - '0')) <= 0 || RockNum[2] != NULL) {
				RockNum = "";
				std::cout << "The number you've input is invalid, please try again with a number 1-20" << std::endl;
				std::cin >> RockNum;
				if (RockNum[1] == NULL) {
					RockNum = '0' + RockNum;
				}
			}
			boardConfig += RockNum;
			
		}
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
				if (move.moveString[0] == 'F') {
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
					char movecstr[80];
					UDP_recv(s, movecstr, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
					if (movecstr[0] == 'C') {
						std::cout << "Comment from your opponent: ";

						//declare iterator to point at the beginning of the array
						const char* it = std::begin(movecstr);
						//skip first character
						++it;

						//iterate while (*it) is truthy
						for (; *it; ++it) std::cout << *it;
						std::cout << std::endl;

						//not my turn yet, as it was just a comment what we received.
						myMove = !myMove;
					}
					else if (movecstr[0] == 'F' || movecstr[0] == 'f') {
						std::cout << "Your opponent forfeited!" << std::endl;
						winner = player;
					}
					else {
						board.updateBoardOp(movecstr);
						std::cout << "Board after your opponent's move" << std::endl;
						board.displayBoard();
					}
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
			if (winner == noWinner) winner = board.check4win();
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
