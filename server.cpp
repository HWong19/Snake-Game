/*
Snake Game for ICS 167 Milestone 3

Notes: There are pre-entered information in the IP, Port, and PlayerName boxes but this can be changed by just clicking the box and editting it. It's this way to make testing quicker.

Andrew Chen, 28676301, andretc1@uci.edu
Contributions: 

David Kang, dhkang2@uci.edu, 49958769
Contributions: 

Harry Wong, cheukhw@uci.edu, 66209666
Contributions: 

Joshua Sosa, jhsosa@uci.edu, 84232577
Contributions: Modified Periodic Handler to a millisecond count from a count of calls. Setup timestamp priority queues for packets. Added timestamp response functionality on server. Added request timestamp functionality for client. Added Artificial latency with a random normal distribution generator to sent messages. Set up time_loop to request timeStamps every 500ms on client.

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
//Timing Code for milliseconds for use in periodic handler taken from http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
*/


#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>
#include <time.h>
#include <queue>
#include <vector>
#include <cstdlib>
#include <sys/timeb.h>
#include <random>
#include "websocket.h"
#include "port.h"
#include "gameState.h"

typedef pair<int, string> Packet;
using namespace std;

struct order{bool operator()(Packet const& a, Packet const& b) const{return a.first < b.first;}};

webSocket server;
GameState gameState;
vector<int> clientIDs;
map<int, string> clientNames;
bool clientsAlreadyConnected = false;
bool playerDeadPacketSent = false;
priority_queue<Packet, deque<Packet>, order> packetsRecv;
priority_queue<Packet, deque<Packet>, order> packetsSend;
default_random_engine generator;
uniform_int_distribution<int> distribution(1,150); //Between 1 and 150 because game_loop time is 150

void openHandler(int clientID);
void closeHandler(int clientID);
void messageHandler(int clientID, string message);
void updateClients(string msg);
void updateClientSnakes(pair<string, string> snakes);
void periodicHandler();
void printMessage(string message);
void startGame();
int getMilliCount();
int getMilliSpan(int nTimeStart);
void analyzeMessage(int clientID, string message);
void checkQueues();
void clearQueues();

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
	clearQueues();
	gameState.resetGame();
	clientsAlreadyConnected = false;
}

void messageHandler(int clientID, string message)
{
	//Make a timestamp in milliseconds
	//Add this <timestamp,msg> to packetsRecv queue
	packetsRecv.push(make_pair(getMilliCount(),message + ":ClientID:" + to_string(clientID))); 
	//check in periodic handler for timestamps and activating message
	//analyzeMessage(clientID, message);
}

void analyzeMessage(int clientID, string message) {

	if (message.find("Timestamp:") != string::npos) {
		//Send back timestamp packet
		updateClients("Timestamp,");
	}else if (message.find("Direction:") != string::npos) {
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
		if(clientID == 0){
			gameState.setP1Name(message.substr(5));
		}else if(clientID == 1){
			gameState.setP2Name(message.substr(5));
		}
		cout << "Player 1 Name: " << gameState.getP1Name() << endl;
		cout << "Player 2 Name: " << gameState.getP2Name() << endl;
		updateClients("PlayerName,1," + gameState.getP1Name());
		updateClients("PlayerName,2," + gameState.getP2Name());
	}
	else if (gameState.getPlayerStatus() && message.find("Reset") != string::npos)
	{
		clearQueues();
		gameState.resetGame();
		startGame();
	}
}

void updateClients(string msg) 
{
	//Make a timestamp in milliseconds
	//Add this <timestamp,msg> to packetsSend queue
	int randInt;
	if(clientsAlreadyConnected) 
		randInt = distribution(generator);
	else 
		randInt = 0;
	packetsSend.push(make_pair(getMilliCount()+randInt, msg));
}

void sendMessage(string msg){
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
	static int next = getMilliCount() + 150;
	int current = getMilliCount();
	if(current >= next) {
		next = getMilliCount() + 150;
		if(gameState.getPlayerStatus()){
			if(!playerDeadPacketSent){
				updateClients("PlayerDied,");
				playerDeadPacketSent = true;
			}
		} else {
			//Step forward a tick in the gameloop
			gameState.moveSnakes();
			updateClientSnakes(gameState.getSnakes());
			updateClients("Scores," + gameState.getP1Score() + "," + gameState.getP2Score());
			updateClients(gameState.getFood());
			//printf("next = %u", next); 
		}
	}
	//Check queues for messages to be sent/recv
	//This makes it send/recv packets as soon as possible
	checkQueues();
}

void printMessage(string message)
{
	cout << message << endl;
}

void startGame()
{
	if(server.getClientIDs().size() == 2) {
		clientsAlreadyConnected = true;
		playerDeadPacketSent = false;
		//Setup Game on server
		gameState.createSnakes();
		//Send Snake Information
		pair<string, string> snakes = gameState.getSnakes();
		sendMessage(snakes.first);
		sendMessage(snakes.second);
		sendMessage(gameState.getDirections());

		//Create and Send Food Information
		gameState.createFood();
		sendMessage(gameState.getFood());

		ostringstream os;
		//Send Scores
		sendMessage("Scores," + gameState.getP1Score() + "," + gameState.getP2Score());

		//Send initialize
		sendMessage("Init");

		//Start game loop
		server.setPeriodicHandler(NULL);
		server.setPeriodicHandler(periodicHandler);
	}
}

int getMilliCount() 
{
		timeb tb;
		ftime(&tb);
		int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
		return nCount;
}

int getMilliSpan(int nTimeStart)
{
		int nSpan = getMilliCount() - nTimeStart;
		if(nSpan < 0)
			nSpan += 0x100000 * 1000;
		return nSpan;
}

void clearQueues(){
	/*
	priority_queue<Packet, vector<Packet>, order> emptyPQ1;
	priority_queue<Packet, vector<Packet>, order> emptyPQ2;
	swap(packetsRecv, emptyPQ1);
	swap(packetsSend, emptyPQ2);
	*/
	while(packetsRecv.size() > 1)
		packetsRecv.pop();
	while(packetsSend.size() > 1)
		packetsSend.pop();
	printMessage("Cleared Queues...");
}

void checkQueues(){
	//check front of each queue for timestamp <= msec time
	if(!packetsSend.empty()){
		if(packetsSend.top().first <= getMilliCount()){
			Packet packet(packetsSend.top().first,packetsSend.top().second);
			if(packet.second.find("Timestamp,") != string::npos)
				sendMessage(packet.second + to_string(packet.first));
			else
				sendMessage(packet.second);
			packetsSend.pop();
		}
	}
	if(!packetsRecv.empty()){
		if(packetsRecv.top().first <= getMilliCount()){
			Packet packet(packetsRecv.top().first, packetsRecv.top().second);
			int clientID = stoi(packet.second.substr(packet.second.find(":ClientID:") + 10));
			string msg = packet.second.substr(0, packet.second.find(":ClientID:"));
			analyzeMessage(clientID, msg);
			packetsRecv.pop();
		}
	}
}
