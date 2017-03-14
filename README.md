# Snake-Game
Snake Game for ICS 167 Milestone 1

Our port number is currently hard-coded to 12345 so please use this when trying to connect to our server.

/* Team Members
Andrew Chen, 28676301, andretc1@uci.edu
Contributions: Frontend: The base for the client, setting up connections between server and client.

David Kang, dhkang2@uci.edu, 49958769
Contributions: None

Harry Wong, cheukhw@uci.edu, 66209666
Contributions: BackEnd: The client-server interactions: server side responses, recording player IDs, tracking scores, reporting new scores, and rejecting new connections.

Joshua Sosa, jhsosa@uci.edu, 84232577
Contributions: Frontend: Modified Chatroom example with text fields to specify PlayerIDs and inserted base code for Snake Game using HTML Canvas. Also, added support for multiple players, body collisions, sending scores to server, pausing game on death

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery

*/

/* Team Members
Snake Game for ICS 167 Milestone 2

Notes: Please note that there are pre-entered information in the IP, Port, and PlayerName boxes but this can be changed by just clicking the box and editting it. It's this way to make testing quicker.

Andrew Chen, 28676301, andretc1@uci.edu
Contributions: Pair Programming setting up initial server game logic sending and receiving information for game

David Kang, dhkang2@uci.edu, 49958769
Contributions: None

Harry Wong, cheukhw@uci.edu, 66209666
Contributions: Pair Programming setting up initial server game logic sending and receiving information for game

Joshua Sosa, jhsosa@uci.edu, 84232577
Contributions: Pair Programming setting up initial server game logic sending and receiving information for game. Setup server to accept two clients, Record player namees based on clientID, Take an inputted port number. Translated the game logic into C++ and implemented it within the server. Updated game logic to report winner server side. Implemented the appropriate sending and receiving for game information from the game logic. This includes sending scores, player numbers, sending player names, sending new coordinates, and sending reset information to all clients. Also, set up the periodic handler to handle the game loop and sending updates to make gameplay smooth. Also updated client to show player names infront of scores.

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
*/

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
