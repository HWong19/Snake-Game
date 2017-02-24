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
Contributions: Modified Periodic Handler to a millisecond count from a count of calls. Setup timestamp priority queues for packets. Added timestamp response functionality on server. Added request timestamp (time_loop) functionality for client. Added artificial latency in milliseconds to sent and received packets with a random uniform distribution integer generator.

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
//Timing Code for milliseconds for use in periodic handler taken from http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
*/
