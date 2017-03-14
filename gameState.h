/*
Snake Game for ICS 167 Milestone 4

Notes: There are pre-entered information in the IP, Port, and PlayerName boxes but this can be changed by just clicking the box and editting it. It's this way to make testing quicker.

Milestone Notes: We implemented a rollback and local prediction in an attempt to mitigate latency. This worked well in reducing jitter and lag. However, it is not perfect it does a good job at preserving user intentions, final outcome, and joint plausibility.

Team Members

Andrew Chen, andretc1@uci.edu, 28676301
Contributions: Brainstorming

David Kang, dhkang2@uci.edu, 49958769
Contributions: None

Harry Wong, cheukhw@uci.edu, 66209666
Contributions: Brainstorming

Joshua Sosa, jhsosa@uci.edu, 84232577
Contributions: Implemented game logic in javascript for local prediction. Handled server and client communication for rollbacks and updating clients of other clients local predictions. Overall, handled latency mitigation with local prediction and rollbacks.

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
//Timing Code for milliseconds for use in periodic handler taken from http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
*/




#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class GameState
{


private:
	string p1Name;
	string p2Name;
	int p1Score;
	int p2Score;
	string player1Direction;
	string player2Direction;
	bool playerIsDead;
	int screenWidth;
	int screenHeight; 
	int cellWidth; // Width of snake body cell
	int cellHeight; //Height of snake body cell
	pair<int,int> food; // Food coordinates
	int snake1Length;
	int snake2Length;
	vector<pair<int, int>> snake1; //Empty array of in pairs to start snake1 coordinates
	vector<pair<int, int>> snake2; //Empty array of in pairs to start snake2 coordinates

public:
	GameState();
	string getP1Name();
	string getP2Name();
	string getP1Score();
	string getP2Score();
	string getP1Direction();
	string getP2Direction();
	string getDirections();
	string getFood();
	pair<string, string> getSnakes();
	int getSnake1Length();
	int getSnake2Length();
	void setP1Name(string name);
	void setP2Name(string name);
	void setP1Score(string score);
	void setP2Score(string score);
	void setP1Direction(string dir);
	void setP2Direction(string dir);
	void printScore();
	void createFood();
	void createSnakes();
	void setSnake1Length(int length);
	void setSnake2Length(int length);
	void moveSnakes();
	bool checkCollision(int x, int y, vector<pair<int, int>> v1, vector<pair<int, int>> v2);
	bool getPlayerStatus();
	void resetGame();
};