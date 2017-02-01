#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include "port.h"
#include "gameState.h"

using namespace std;

webSocket server;
GameState gameState;
vector<int> clientIDs;
string player1;
string player2;
int turn = 1;
bool isAlreadyConnected = false;


void openHandler(int clientID);
void closeHandler(int clientID);
void messageHandler(int clientID, string message);
void periodicHandler();



int main(int argc, char *argv[])
{
	int port = PORT_NUMBER;

	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);

	server.startServer(port);

	return 1;
}

void openHandler(int clientID)
{
	if (isAlreadyConnected)
	{
		server.wsClose(clientID);
	}
	isAlreadyConnected = true;
}

void closeHandler(int clientID)
{
	isAlreadyConnected = false;
	printf("Client has disconnected.");
}

void messageHandler(int clientID, string message)
{
	if (message.find("score1:") != string::npos)
	{
		gameState.setP1Score(message.substr(7));
		gameState.printScore();
	}
	else if (message.find("score2:") != string::npos)
	{
		gameState.setP2Score(message.substr(7));
		gameState.printScore();
	}

	else if (message.find("Player1:") != string::npos)
	{
		gameState.setP1Name(message.substr(8));
	}

	else if (message.find("Player2:") != string::npos)
	{
		gameState.setP2Name(message.substr(8));
	}
}