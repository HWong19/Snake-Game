#include "gameState.h"


GameState::GameState()
{
	p1Score = "0";
	p2Score = "0";
}

string GameState::getP1Score()
{
	return p1Score;
}

string GameState::getP2Score()
{
	return p2Score;
}


void GameState::setP1Score(string score)
{
	p1Score = score;
}

void GameState::setP2Score(string score)
{
	p2Score = score;
}

void GameState::printScore()
{
	cout << "Player 1: " << p1Score << endl << "Player 2: " << p2Score << endl << endl;
}