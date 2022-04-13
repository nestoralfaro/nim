# nim

- John Aldrich
- Nestor Alfaro
- Brendan Burke
- Joel Hoppe
Order of Operations:

Client:
- Inputs name from user (<=80 chars) used to identify player to others on network
- Create UDP socket - Send out a Datagram "Who?" [Null Terminator] to port #29333
- Enter Loop, Wait for and receive reply datagram from ALL Nim servers on network and hosting games. Reply will contain sting server name. Client stores names of all server names, their IP addresses and Port numbers
- Loop is maintained 1-2 seconds
- User may choose what server to connect to AFTER loop ends
- Send Datagram to Server's IP and port #29333 containing "Player=[User's name]"
- Wait for reply, decide a sufficient time to wait, don't wait forever.
- Server will reply "YES" or "NO" 
- If reply is "NO" allow user to challenge another host ((Seems like we need to have steps 11-16 in a loop exiting when a challenge is accepted to the user chooses to exit))
- If reply is "YES" send a Datagram containing string "GREAT"
- If no reply, assume answer is "NO" and allow user to choose another opponent or quit
- Note: adding the following lines to client_main immediately after the connectsock function is called should resolve the "Problem with sendto function call within UDP_send(). Error Code = 10013."
- BOOL bOptVal = TRUE;
  setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, sizeof(BOOL));

Server:
