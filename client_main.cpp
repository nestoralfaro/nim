// client_main.cpp
//   This function serves as the "main" function for the client-side
#include "NIM.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int client_main(int argc, char* argv[], std::string playerName)
{
	std::string host;
	std::string port;
	ServerStruct server[MAX_SERVERS];
	char protocol[4] = "udp";
	char* _host = NULL;

	SOCKET s = connectsock(_host, NIM_UDPPORT, protocol);		// Create a socket  (host and port not used here for UDP)
	//Added to resolve "Problem with sendto function call within UDP_send(). Error Code = 10013."
	bool bOptVal = true;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, sizeof(bool));

	// Find all NIM servers
	std::cout << std::endl << "Looking for NIM servers ... " << std::endl;
	int numServers;
	getServers(s, GetBroadcastAddress(), NIM_UDPPORT, server, numServers);

	if (numServers == 0) {
		std::cout << std::endl << "Sorry.  No NIM servers were found.  Try again later." << std::endl << std::endl;
	}
	else {
		// Display the list of servers found
		std::cout << std::endl << "Found NIM server";
		if (numServers == 1) {
			std::cout << ":" << "  " << server[0].name << std::endl;
		}
		else {
			std::cout << "s:" << std::endl;
			for (int i = 0; i < numServers; i++) {
				std::cout << "  " << i + 1 << " - " << server[i].name << std::endl;
			}
			std::cout << std::endl << "  " << numServers + 1 << " - QUIT" << std::endl;
		}
		std::cout << std::endl;

		// Allow user to select someone to challenge
		int answer = 0;
		std::string answer_str;
		if (numServers == 1) {
			std::cout << "Do you want to challenge " << server[0].name << "? ";
			std::getline(std::cin, answer_str);
			if (answer_str[0] == 'y' || answer_str[0] == 'Y') answer = 1;
		}
		else if (numServers > 1) {
			std::cout << "Who would you like to challenge (1-" << numServers + 1 << ")? ";
			std::getline(std::cin, answer_str);
			answer = atoi(answer_str.c_str());
			if (answer > numServers) answer = 0;
		}

		if (answer >= 1 && answer <= numServers) {
			// Extract the opponent's info from the server[] array
			std::string serverName;
			serverName = server[answer - 1].name;		// Adjust for 0-based array
			host = server[answer - 1].host;
			port = server[answer - 1].port;

			// Append playerName to the NIM_CHALLENGE string & send a challenge to host:port
			char buf[MAX_SEND_BUF];
			strcpy_s(buf, NIM_CHALLENGE);
			strcat_s(buf, playerName.c_str());
			int len = UDP_send(s, buf, strlen(buf) + 1, (char*)host.c_str(), (char*)port.c_str());

			// Play the game.  You are the 'X' player
			int winner = playNIM(s, serverName, host, port, Client_PLAYER);
		}
	}

	closesocket(s);
	return 0;
}
