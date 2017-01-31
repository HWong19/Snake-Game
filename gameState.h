#pragma once


class GameState
{


private:
	int p1Score;
	int p2Score;


public:
	GameState();
	int getP1Score();
	int getP2Score();
	void incrP1Score();
	void incrP2Score();
	void setP1Score(int score);
	void setP2Score(int score);

};