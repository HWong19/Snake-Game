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