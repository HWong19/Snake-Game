#include <stdlib.h>
#include <iostream>
#include <ostream>
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
map<int, string> clientNames;
int turn = 0; //periodicHandler calls about 1 per msec
int msec = 40; //Speed of updates
bool clientsAlreadyConnected = false;


void openHandler(int clientID);
void closeHandler(int clientID);
void messageHandler(int clientID, string message);
void updateClients(string msg);
void updateClientSnakes(pair<string, string> snakes);
void periodicHandler();
void printMessage(string message);
void startGame();



int main(int argc, char *argv[])
{
	int port; // for static port: = PORT_NUMBER;
	// If we want to set server port
    cout << "Please set server port: ";
    cin >> port;

    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
    //server.setPeriodicHandler(periodicHandler);

    /* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
    server.startServer(port);

	return 1;
}

void openHandler(int clientID)
{
	if(!clientsAlreadyConnected) {
		ostringstream os;
		os << "Player " << clientID+1 << " has joined.";
		printMessage(os.str());
		updateClients(os.str());
		os.str("");
		os.clear();
		os << "PlayerNumber," << clientID+1;
		//Send player number
		server.wsSend(clientID, os.str());
	
		startGame();
	}
	else{
		server.wsClose(clientID);
	}
}

void closeHandler(int clientID)
{
    ostringstream os;
    os << "Player " << clientID+1 << ": " << clientNames[clientID] << " has left.";
	printMessage(os.str());

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID)
            server.wsSend(clientIDs[i], os.str());
    }
	server.setPeriodicHandler(NULL);
	gameState.resetGame();
	clientsAlreadyConnected = false;
}

void messageHandler(int clientID, string message)
{
	if (message.find("Direction:") != string::npos) {
		if(clientID == 0){
			gameState.setP1Direction(message.substr(10));
			printMessage("Player1 Direction Changed to " + message.substr(10));
		} else if(clientID == 1){
			gameState.setP2Direction(message.substr(10));
			printMessage("Player2 Direction Changed to " + message.substr(10));
		}
	}else if (message.find("Name:") != string::npos)
	{
		clientNames[clientID] = message.substr(5);
		if(server.getClientIDs().size() == 1){
			gameState.setP1Name(message.substr(5));
		}else if(server.getClientIDs().size() == 2){
			gameState.setP2Name(message.substr(5));
		}
		updateClients("PlayerName,1," + gameState.getP1Name());
		updateClients("PlayerName,2," + gameState.getP2Name());
		cout << "Player 1 Name: " << gameState.getP1Name() << endl;
		cout << "Player 2 Name: " << gameState.getP2Name() << endl;
	}
	else if (gameState.getPlayerStatus() && message.find("Reset") != string::npos)
	{
		gameState.resetGame();
		startGame();
	}
}

void updateClients(string msg) 
{
	//updates all clients with new message
	vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++)
			server.wsSend(clientIDs[i], msg);
}

void updateClientSnakes(pair<string, string> snakes)
{
	updateClients(snakes.first);
	updateClients(snakes.second);
}

/* called once per select() loop */
void periodicHandler() {
	if(gameState.getPlayerStatus()){
		updateClients("PlayerDied,");
		server.setPeriodicHandler(NULL);
	}
	++turn;
	if (turn >= msec){
		turn = 0;
		gameState.moveSnakes();
		updateClientSnakes(gameState.getSnakes());
		updateClients("Scores," + gameState.getP1Score() + "," + gameState.getP2Score());
		updateClients(gameState.getFood());
	}
}

void printMessage(string message)
{
	cout << message << endl;
}

void startGame()
{
	if(server.getClientIDs().size() == 2) {
		clientsAlreadyConnected = true;
		//Setup Game on server
		gameState.createSnakes();
		//Send Snake Information
		updateClientSnakes(gameState.getSnakes());
		updateClients(gameState.getDirections());

		//Create and Send Food Information
		gameState.createFood();
		updateClients(gameState.getFood());

		ostringstream os;
		//Send Scores
		updateClients("Scores," + gameState.getP1Score() + "," + gameState.getP2Score());

		//Send initialize
		updateClients("Init");

		//Start game loop
		server.setPeriodicHandler(periodicHandler);
	}
}