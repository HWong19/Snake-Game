#pragma once
#include <string>
#include <iostream>

using namespace std;
class GameState
{


private:
	string p1Name;
	string p2Name;
	string p1Score;
	string p2Score;


public:
	GameState();
	string getP1Name();
	string getP2Name();
	string getP1Score();
	string getP2Score();
	void setP1Name(string name);
	void setP2Name(string name);
	void setP1Score(string score);
	void setP2Score(string score);
	void printScore();
};