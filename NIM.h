#pragma once
// NIM.h
//
// This header file establishes some constants that should be used
// in the NIM project for COMP 3110
#include <WinSock2.h>
#include <vector>
#include <string>

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

//struct NimBoard {
//	int rowCount;
//	//int boardRows[rowCount];
//	std::vector<int> boardRows(rowCount, 0);
//}

class NimBoard {
	std::vector<int> board;
	int piles;
public:
	NimBoard() : piles { 0 } {};
	void setBoard(std::string pilesConf) {
		this->piles = pilesConf[0] - '0';
		//board(this->piles, 0);
		//for (int pile = 0; pile < board.size(); ++pile) {
		//	board[pile] = atoi((const char*)(piles[pile + 1] + piles[pile + 2]));
		//}
		for (int c = 1; c <= pilesConf.size() - 2; c += 2) {
			//board.push_back(atoi((const char*)(piles[c] + piles[c + 1])));
			board.push_back((pilesConf[c] - '0') * 10 + (pilesConf[c + 1] - '0'));
		}
	}

	std::vector<int> getBoard() {
		return this->board;
	}

	int getPiles() {
		return this->piles;
	}
};

struct Move {
	std::string moveString;
	int pile;
	int amountToRemove;
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