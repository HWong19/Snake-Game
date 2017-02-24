/* 
Snake Game for ICS 167 Milestone 3

Notes: There are pre-entered information in the IP, Port, and PlayerName boxes but this can be changed by just clicking the box and editting it. It's this way to make testing quicker.

Andrew Chen, 28676301, andretc1@uci.edu
Contributions: 

David Kang, dhkang2@uci.edu, 49958769
Contributions: 

Harry Wong, cheukhw@uci.edu, 66209666
Contributions: 

Joshua Sosa, jhsosa@uci.edu, 84232577
Contributions: Modified Periodic Handler to a millisecond count from a count of calls. Setup timestamp priority queues for packets. Added timestamp response functionality on server. Added request timestamp functionality for client. Added Artificial latency with a random normal distribution generator to sent messages. Set up time_loop to request timeStamps every 500ms on client.

Please note that we are giving credit to the original sources of the Chatroom Example and Snake Game tutorial followed.
// Main code for Chatroom Demo taken from 
// http://www.ics.uci.edu/~rkwang/Winter_2017_ICS167/project.html
// Main code/tutorial for Snake game taken from
// http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
//Timing Code for milliseconds for use in periodic handler taken from http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
*/

	//Canvas stuff
	var canvas = $("#canvas")[0];
	var ctx = canvas.getContext("2d");
	var w = $("#canvas").width();
	var h = $("#canvas").height();
	
	//Lets save the cell width in a variable for easy control
	var cw = 10;
	var food;
	var score1;
    var score2;
	var snake1Color = "Blue";
	var snake2Color = "Red";
	var foodColor = "Green";
	var textColor = "Black";
	var playerIsDead = false;
	var game_loop;
	var time_loop;
	
	var playerNumber; //Int to indicate which player this client is controlling
	var player1Name = ""; //Name of the player so it can't be changed
	var player2Name = ""; //Name of the player so it can't be changed

	//Lets create the snake now
	var snake1; //an array of cells to make up the snake1
	var snake2; //an array of cells to make up the snake2
	var snake1Length;
	var snake2Length;
	var d;
  
	function init()
	{
		playerIsDead = false;
		//Lets move the snake now using a timer which will trigger the paint function every 60ms
		if(game_loop != "undefined") clearInterval(game_loop);
		game_loop = setInterval(paint, 60);
		if(time_loop != "undefined") clearInterval(time_loop);
		time_loop = setInterval(timeStamp, 500);
	}
	
	function setScore1(num)
	{
		score1 = num;
	}
	
	function setScore2(num)
	{
		score2 = num;
	}
	
	function setPlayerNumber(num)
	{
		playerNumber = num;
	}
	
	function setPlayer1Name(str)
	{
		player1Name = str;
	}
	
	function setPlayer2Name(str)
	{
		player2Name = str;
	}
  
  	function setSnakeDirection(str)
	{
		d = str;
	}
  
  	function setSnake1Length(num)
	{
		snake1Length = num;
	}
	
	function setSnake2Length(num)
	{
		snake2Length = num;
	}
	
	function setSnake1(res)
	{
		snake1 = [];
		//This will create a horizontal snake starting from the top left
		for(var i = snake1Length*2+1; i>=3; i-=2) //>=3 b/c res[0],res[1] are not coordinates
			snake1.push({x: res[i-1], y: res[i]});
	}
	
	function setSnake2(res)
	{
		snake2 = [];
		//This will create a horizontal snake starting from the top left
		for(var i = snake2Length*2+1; i>=3; i-=2) //>=3 b/c res[0],res[1] are not coordinates
			snake2.push({x: res[i-1], y: res[i]});
	}
	
	function setFood(res)
	{
		food = { x: res[1], y: res[2] };
	}
	
	function setPlayerDied()
	{
		playerIsDead = true;
	}
	
	function getPlayer1Name()
	{
		return player1Name;
	}
	function getPlayer2Name()
	{
		return player2Name;		
	}
	
	//Lets paint the snake now
	function paint()
	{
		//To avoid the snake trail we need to paint the BG on every frame
		//Lets paint the canvas now
		ctx.fillStyle = "white";
		ctx.fillRect(0, 0, w, h);
		ctx.strokeStyle = "black";
		ctx.strokeRect(0, 0, w, h);
		
		//Lets paint the snakes
        var cell;
		for(var i = 0; i < snake1.length; i++)
		{
			cell = snake1[i];
			//Lets paint 10px wide cells
			paint_cell(cell.x, cell.y, snake1Color);
		}
        for(i = 0; i < snake2.length; i++)
		{
			cell = snake2[i];
			//Lets paint 10px wide cells
			paint_cell(cell.x, cell.y, snake2Color);
		}
		
		//Lets paint the food
		paint_cell(food.x, food.y, foodColor);
		
		//Lets paint the score
		ctx.fillStyle = textColor;
		if(!playerIsDead) {
			ctx.font = "20px Arial";
			var score1_text = player2Name + "'s Score 2: " + score2;
			ctx.fillText(score1_text, 5, h-5);
			var score2_text = player1Name + "'s Score 1: " + score1;
			ctx.fillText(score2_text, 5, h-25);
		}else{
			ctx.font = "30px Arial";
			ctx.fillText("Player Died",5,h-35);
			ctx.fillText("Press the 'r' key to reset",5,h-5);
		}
	}

	function timeStamp(){
		send("Timestamp:")
	}
	
	//Lets first create a generic function to paint cells
	function paint_cell(x, y, color)
	{
		ctx.fillStyle = color;
		ctx.fillRect(x*cw, y*cw, cw, cw);
		ctx.strokeStyle = "white";
		ctx.strokeRect(x*cw, y*cw, cw, cw);
	}

	
	//Lets add the keyboard controls now
	$(document).keydown(function(e){
		var key = e.which;
		//We will add the d1 clauses to prevent reverse gear
		//These statements make the snake controllable via the arrow keys and wasd
		var oldD = d;
		if((key == "37" || key == "65")&& d != "right") d = "left";
		else if((key == "38" || key == "87") && d != "down") d = "up";
		else if((key == "39" || key == "68") && d != "left") d = "right";
		else if((key == "40" || key == "83") && d != "up") d = "down";
		if(oldD != d)
			send("Direction:" + d);
		
		//Statement to reset the game
		if(playerIsDead && key == "82"){
			playerIsDead = false;
			send("Reset");
		}
	});