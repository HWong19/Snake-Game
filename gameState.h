#pragma once
#include <string>

using namespace std;

class GameState
{


private:
	string p1Score;
	string p2Score;


public:
	GameState();
	string getP1Score();
	string getP2Score();
	void setP1Score(string score);
	void setP2Score(string score);
	void printScore();
};