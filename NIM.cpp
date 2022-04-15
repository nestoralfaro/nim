// NIM.cpp

// This program plays NIM with other computers across the network using the UDP protocol.

#include "NIM.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WORD wVersion = 0x0202;
	WSADATA wsaData;
	int iResult = WSAStartup(wVersion, &wsaData);
	if (iResult != 0) {
		std::cout << "Unable to initialize Windows Socket library." << std::endl;
		return 0;
	}
	std::string playerName;
	std::cout << "What is your name? ";
	std::getline(std::cin, playerName);
	
	while (playerName.length() > 80) {
		std::cout << "Name must be 80 characters max" << std::endl;
		std::cout << "What is your name? ";
		std::getline(std::cin, playerName);
	}

	std::string choice_str;
	char newline;
	int choice = 0;
	const int QUIT = 3;
	do {
		std::cout << std::endl << std::endl << std::endl;
		std::cout << "Choose an option:" << std::endl;
		std::cout << "   1 - Host a game of Nim" << std::endl;
		std::cout << "   2 - Challenge someone else" << std::endl;
		std::cout << "   3 - Quit" << std::endl;
		std::cout << "Enter 1, 2 or 3: ";
		std::cin >> choice_str; std::cin.get(newline);
		choice = atoi(choice_str.c_str());

		switch (choice) {
		case 1:	server_main(argc, argv, playerName); break;
		case 2:	client_main(argc, argv, playerName); break;
		case 3: break;
		default: std::cout << std::endl << "Please enter a digit between 1 and 3." << std::endl;
		}
	} while (choice != QUIT);
/*INITIALIZING GAME BOARD, NEEDS EDITS TO FIT WITH SYNTAX
Int PileNumber = 0;
Cout << "Please input how many piles you'd like to play from 3 to 9."
While (PileNumber > 9 || PileNumber < 3){
CIN >> PileNumber;
If (PileNumber > 9 || PileNumber < 3){
Cout << "Invalid input, please input a number from 3 to 9";
}
}
For (I=0, I<PileNumber, I++){
Int 
String Suffix = "";
If (PileNumber == 1){
Suffix = "st";
}
Elseif (PileNumber == 2){
Suffix = "nd";
}
Elseif (PileNumber == 3) {
Suffix = "rd";
}
Else {
Suffix = "th"
}
While (pile[I] < 0 || Pile[I] > 20){
COUT << "How many rocks will be in the " << I + 1 << Suffix << " pile?";
CIN << pile[I]
}
}*/
	WSACleanup();
}

