#include "gameState.h"


GameState::GameState()
{
	p1Score = "0";
	p2Score = "0";
}

string GameState::getP1Name()
{
	return p1Name;
}

string GameState::getP2Name()
{
	return p2Name;
}

string GameState::getP1Score()
{
	return p1Score;
}

string GameState::getP2Score()
{
	return p2Score;
}

void GameState::setP1Name(string name)
{
	p1Name = name;
}

void GameState::setP2Name(string name)
{
	p2Name = name;
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
	cout << "Player 1 (" << p1Name << "): " << p1Score << endl << "Player 2 (" << p2Name << "): " << p2Score << endl << endl;
}