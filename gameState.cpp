#include "gameState.h"


GameState::GameState()
{
	p1Score = 0;
	p2Score = 0;
}

int GameState::getP1Score()
{
	return p1Score;
}

int GameState::getP2Score()
{
	return p2Score;
}

void GameState::incrP1Score()
{
	++p1Score;
}

void GameState::incrP2Score()
{
	++p2Score;
}

void GameState::setP1Score(int score)
{
	p1Score = score;
}

void GameState::setP2Score(int score)
{
	p2Score = score;
}