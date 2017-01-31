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
int player1 = -1;
int player2 = -1;
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
	clientIDs = server.getClientIDs();
}

void closeHandler(int clientID)
{
	isAlreadyConnected = false;
	clientIDs = server.getClientIDs();
}

void messageHandler(int clientID, string message);
{
	if (player1 == -1)
	{
		player1 = (int)message;
	}
	else if (player2 == -1)
	{
		player2 = (int)message;
	}
	else
	{
	}
}