#pragma once
// NIM.h
//
// This header file establishes some constants that should be used
// in the NIM project for COMP 3110
#include <WinSock2.h>
#include <vector>
#include <string>
#include <iostream>

static char NIM_UDPPORT[] = "29333";	// Port number used by NIM servers

#define NIM_QUERY  "Who?"
#define NIM_NAME	 "Name="
#define NIM_CHALLENGE "Player="

const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;
const int Serv_PLAYER = 1;
const int Client_PLAYER = 2;
const int ABORT = -1;
const int noWinner = 0;
const int ServWinner = 1;
const int ClientWinner = 2;
const int TIE = 3;

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

struct Move {
	std::string moveString;
	int pile;
	int amountToRemove;
};

class NimBoard {
	std::vector<int> board;
	int piles;
public:
	NimBoard() : piles { 0 } {};
	void setBoard(std::string pilesConf) {
		this->piles = pilesConf[0] - '0';
		for (int c = 1; c <= pilesConf.size() - 2; c += 2) {
			board.push_back((pilesConf[c] - '0') * 10 + (pilesConf[c + 1] - '0'));
		}
	}

	std::vector<int> getBoard() {
		return this->board;
	}

	void updateBoard(int pile, int amountToRemove) {
		if (this->board[pile - 1] != 0)
			this->board[pile - 1] -= amountToRemove;
		else std::cout << "Cannot remove from empty pile" << std::endl;
	}

	void displayBoard() {
		int row = 1;
		std::cout << std::endl;
		for (const auto& pile : this->board) {
			std::cout << "Row " << row++ << ": ";

			for (int rock = 0; rock < pile; ++rock) {
				std::cout << " | ";
			}
		}

		std::cout << std::endl;
	}

	Move getMove(const int player) {
		int move;
		std::string inputOne;
		std::string move_str;
		std::string comment = "";
		int pile;
		int amountToRemove;
		//std::string rocksToRemove;
		std::cout << "You are playing as the";
		//	X		O
		std::string mark = (player == Client_PLAYER) ? "client" : "server";
		std::cout << mark << std::endl;

		do {	
			inputOne = "invalid";
			while(inputOne == "C"||inputOne=="invalid"){
				std::cout << "Please input a number 1-" << /*Number of total piles*/ << " to select a pile to remove from, The letter 'C' to send a comment, or the letter 'F' to forfeit";
				std::cin >> inputOne;
				if (inputOne == "C"){
					cout << "Please type the comment you would like to send.";
					while(strlen(comment) > 80){
					std::getline(std::cin, comment);
					if(strlen(comment)>80){
						cout << "Your comment was too long, please send something shorter.";
					}
				return /*Variable*/{inputOne, comment);
				}
				else if (inputOne == "F"){
					pile = "0";
					amountToRemove = "00";
				}
				else if (/*logic to determine if its within the number limit*/){
					/*logic to parse and send*/
				}
				else{
					"Your input was invalid. Please try again, input a number 1-" << /*number of rocks in selected pile*/;
					inputOne = "invalid"
				}
			}
		/*	pile = (move_str[0] - '0');
			amountToRemove = (move_str[1] - '0') * 10 + (move_str[2] - '0');
			//if pile has no rocks
		} while (this->board[pile - 1] == 0);

		return Move{ move_str, pile, amountToRemove };*/
	}

	void updateBoard(std::string move) {
		int pile = (move[0] - '0');
		int amountToRemove = (move[1] - '0') * 10 + (move[2] - '0');
		this->updateBoard(pile, amountToRemove);
	}

	int getPiles() {
		return this->piles;
	}

	//This function needs to be ran when we get there move to know if they won and after our move to know if we won
	int check4win() {
		std::cout << "checking for win" << std::endl;
		/* Need to work here */
		int total = 0;
		for (int i = 0; i < this->piles.size(); i++) {
			total = total + this->piles[i];
		}

		if (total == 0) {
			std::cout << "We have a winner!!!" << std::endl;
			return 1;
		}

		return noWinner;
	}
};

SOCKET connectsock(char*, char*, char*);
SOCKET passivesock(char*, char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, char*, int, char*, char*);
int wait(SOCKET, int, int);
char* GetBroadcastAddress();

int server_main(int, char* [], std::string);
int client_main(int, char* [], std::string);
void getServers(SOCKET, char*, char*, ServerStruct[], int&);
int playNIM(SOCKET, std::string, std::string, std::string, int);
