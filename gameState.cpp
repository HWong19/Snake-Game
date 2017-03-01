/*
Snake Game for ICS 167 Milestone 3

Notes: There are pre-entered information in the IP, Port, and PlayerName boxes but this can be changed by just clicking the box and editting it. It's this way to make testing quicker.

Milestone Notes: We add an random artificial latency from 1 to 150ms to sent and received packets. We calculate the latency with the NTP equations from class ((B-A)-(Y-X))/2. The average latencies are between 1 and 300ms. Sometimes we see negative latencies, however, this is when a rollback occurs due to out of order packets. The out of order packets also make the game a little wonky. For example, when the game ends (player dies) the snakes will move backwards to a position closer to the starting position. We do a timestamp request for a latency estimation every second. You can see these estimations on screen and each client's log.

Team Members

Andrew Chen, andretc1@uci.edu, 28676301
Contributions: 

David Kang, dhkang2@uci.edu, 49958769
Contributions: 

Harry Wong, cheukhw@uci.edu, 66209666
Contributions: 

Joshua Sosa, jhsosa@uci.edu, 84232577
Contributions: Modified Periodic Handler to a millisecond count from a count of calls. Setup timestamp priority queues for packets. Added timestamp response functionality on server. Added request timestamp (time_loop) functionality for client. Added artificial latency in milliseconds to sent and received packets with a random uniform distribution integer generator. Implemented latency calculation and display on client. Modified client logging. 

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
//Timing Code for milliseconds for use in periodic handler taken from http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
*/



#include "gameState.h"
#include <numeric>      // std::iota
#include <sstream>


GameState::GameState()
{
	p1Score = 0;
	p2Score = 0;
	screenWidth = 450;
	screenHeight = 450;
	cellWidth = 10;
	cellHeight = 10;
	snake1Length = 5;
	snake2Length = 5;
	player1Direction = "right";
	player2Direction = "down";
	playerIsDead = false;
}

string GameState::getP1Score()
{
	ostringstream os;
	os << p1Score;
	return os.str();
}

string GameState::getP1Name()
{
	return p1Name;
}

string GameState::getP2Score()
{
	ostringstream os;
	os << p2Score;
	return os.str();
}

string GameState::getP2Name()
{
	return p2Name;
}

string GameState::getP1Direction()
{
	return player1Direction;
}

string GameState::getP2Direction()
{
	return player2Direction;
}

string GameState::getDirections()
{
	return "Directions," + player1Direction + "," + player2Direction;
}

string GameState::getFood()
{
	stringstream ss;//create a stringstream
	ss << "Food," << food.first << "," << food.second;//add numbers to the stream
	return ss.str();//return a string with the contents of the stream
}

int GameState::getSnake1Length()
{
	return snake1.size();
}

int GameState::getSnake2Length()
{
	return snake2.size();
}

pair<string, string> GameState::getSnakes() 
{
	setSnake1Length(snake1.size());
	setSnake2Length(snake2.size());

	stringstream s1;//create snake1 stringstream
	stringstream s2;//create snake2 stringstream

	s1 << "Snake1,"<< snake1Length << ",";
	s2 << "Snake2," << snake2Length << ",";
	for(int i = 0; i < snake1Length; ++i){
		s1 << snake1[i].first << "," << snake1[i].second;//add snake1 coordinate to the stream
		if(i < snake1Length-1)
			s1 << ",";
	}
	for(int i = 0; i < snake2Length; ++i){
		s2 << snake2[i].first << "," << snake2[i].second;//add snake2 coordinate to the stream
		if(i < snake2Length-1)
			s2 << ",";
	}
	pair<string,string> str = make_pair(s1.str(), s2.str());
	return str;//return a pair of strings of the snakes coordinates
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
	p1Score = stoi(score);
}

void GameState::setP2Score(string score)
{
	p2Score = stoi(score);
}

void GameState::setP1Direction(string dir)
{
	player1Direction = dir;
}

void GameState::setP2Direction(string dir)
{
	player2Direction = dir;
}

void GameState::setSnake1Length(int length)
{
	snake1Length = length;
}

void GameState::setSnake2Length(int length)
{
	snake2Length = length;
}

void GameState::printScore()
{
	cout << "Player 1 (" << p1Name << "): " << p1Score << endl << "Player 2 (" << p2Name << "): " << p2Score << endl << endl;
}

void GameState::createFood()
{
	food.first = (rand()%(screenWidth/cellWidth));
	food.second = (rand()%(screenHeight/cellHeight));
	//This will create a random cell with x/y between 0-44
	//Because there are 45(450/10) positions accross the rows and columns
}

void GameState::createSnakes()
{
	//This will create a horizontal snake starting from the top left
	for(int i = snake1Length-1; i>=0; --i)
		snake1.push_back(make_pair(i+1, 0));
	//This will create a horizontal snake starting from the top left
	for(int i = snake2Length-1; i>=0; --i)
		snake2.push_back(make_pair(0, i+1));
}

void GameState::moveSnakes()
{
		//The movement code for the snake to come here.
		//Logic: Pop out the tail cell and place it infront of the head cell
		int nx1 = snake1[0].first;
		int ny1 = snake1[0].second;
        int nx2 = snake2[0].first;
		int ny2 = snake2[0].second;
		//These were the coordinates of the head cell.
		//We will increment it to get the new head position

		//This adds proper direction based movement now
		if(!playerIsDead) {
			if(player1Direction.find("right") != string::npos) nx1++;
			else if(player1Direction.find("left") != string::npos) nx1--;
			else if(player1Direction.find("up") != string::npos) ny1--;
			else if(player1Direction.find("down") != string::npos) ny1++;
			if(player2Direction.find("right") != string::npos) nx2++;
			else if(player2Direction.find("left") != string::npos) nx2--;
			else if(player2Direction.find("up") != string::npos) ny2--;
			else if(player2Direction.find("down") != string::npos) ny2++;
		}
		
		//This will restart the game
		//If the snake hits the wall
		//Or if the snake has a body collision
		//Or if the snake hits the other snake
		if(nx1 == -1 || nx1 == screenWidth/cellWidth || ny1 == -1 || ny1 == screenHeight/cellHeight 
			|| checkCollision(nx1, ny1, snake1, snake2))
		{
			//pause game
			playerIsDead = true;
			cout << "Player 1 Died." << endl;
			if(p1Score > p2Score)
				cout << "Player 1 Wins!" << endl;
			else if (p2Score > p1Score)
				cout << "Player 2 Wins!" << endl;
			else cout << "Draw!" << endl;
		}
        if(nx2 == -1 || nx2 == screenWidth/cellWidth || ny2 == -1 || ny2 == screenHeight/cellHeight 
			|| checkCollision(nx2, ny2, snake2, snake1))
		{
			//pause game
			playerIsDead = true;
			cout << "Player 2 Died." << endl;
			if(p1Score > p2Score)
				cout << "Player 1 Wins!" << endl;
			else if (p2Score > p1Score)
				cout << "Player 2 Wins!" << endl;
			else cout << "Draw!" << endl;
		}

		//This code is to make the snakes eat the food
		//Logic: If the new head position matches with that of the food,
		//Create a new head instead of moving the tail
		//Otherwise, move tail to front as head <-- this is the moving step
		pair<int, int> tail;
        if(nx1 == food.first && ny1 == food.second){
			tail = make_pair(nx1, ny1);
			p1Score++;
			printScore();
			setSnake1Length(snake1.size()+1);
			//Create new food
			createFood();
		}else{
			tail = snake1.back();
			snake1.pop_back();
			tail.first = nx1; 
			tail.second = ny1;
		}
		snake1.insert(snake1.begin(),tail);

		if(nx2 == food.first && ny2 == food.second){
			tail = make_pair(nx2, ny2);
			p2Score++;
			printScore();
			setSnake2Length(snake2.size()+1);
			//Create new food
			createFood();
		}else{
			tail = snake2.back();
			snake2.pop_back();
			tail.first = nx2; 
			tail.second = ny2;
		}
		snake2.insert(snake2.begin(),tail);

}

bool GameState::checkCollision(int x, int y, vector<pair<int,int>> v1, vector<pair<int,int>> v2)
{
		//This loop will check if the provided x/y coordinates exist
		//in the vector of cells
		for(int i = 0; i < v1.size(); ++i)
			if(v1[i].first == x && v1[i].second == y)
               return true;
		
		//This loop will check if the provided vectors contain a matching x/y coordinate
        for (int i = 0; i < v1.size(); ++i) 
            for(int j = 0; j < v2.size(); ++j)
                if (v1[i].first == v2[j].first && v1[i].second == v2[j].second)
                    return true;
		return false;
}

bool GameState::getPlayerStatus()
{
	return playerIsDead;
}

void GameState::resetGame()
{
	snake1.clear();
	snake2.clear();
	p1Score = 0;
	p2Score = 0;
	screenWidth = 450;
	screenHeight = 450;
	cellWidth = 10;
	cellHeight = 10;
	snake1Length = 5;
	snake2Length = 5;
	player1Direction = "right";
	player2Direction = "down";
	playerIsDead = false;
}