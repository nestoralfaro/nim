#pragma once
// NIM.h
//
// This header file establishes some constants that should be used
// in the NIM project for COMP 3110
#include <WinSock2.h>
#include <string>

static char NIM_UDPPORT[] = "29333";	// Port number used by NIM servers

#define NIM_QUERY  "Who?"
#define NIM_NAME	 "Name="
#define NIM_CHALLENGE "Player="

const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;
const int X_PLAYER = 1;
const int O_PLAYER = 2;
const int ABORT = -1;
const int noWinner = 0;
const int xWinner = 1;
const int oWinner = 2;
const int TIE = 3;

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

struct NimBoard {
	int rowCount;
	int boardRows[rowCount];
}NimBoard1;

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

