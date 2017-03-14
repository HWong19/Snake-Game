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
	var extrapolated_playerIsDead = false;
	var render_loop;
	var latency_loop;
	var text_loop;
	
	var playerNumber; //Int to indicate which player this client is controlling
	var player1Name = ""; //Name of the player so it can't be changed
	var player2Name = ""; //Name of the player so it can't be changed

	//Lets create the snake now
	var snake1; //an array of cells to make up the snake1
	var extrapolated_snake1;
	var snake2; //an array of cells to make up the snake2
	var extrapolated_snake2;
	var snake1Length;
	var snake2Length;
	var snake1Direction;
	var snake2Direction;
	var d;
	
	//Latency Variables
	var a = 0;
	var b = 0;
	var latency = 0;
  
	function init()
	{
	    playerIsDead = false;
	    extrapolated_playerIsDead = false;
	    snake1 = [];
	    snake2 = [];
	    extrapolated_snake1 = [];
	    extrapolated_snake2 = [];
	    snake1Direction = "right";
	    snake2Direction = "down";
		//Lets move the snake now using a timer which will trigger the paint function every 60ms
	    if (text_loop != "undefined") clearInterval(text_loop);
	    text_loop = setInterval(paint_text, 100);
		if (render_loop != "undefined") clearInterval(render_loop);
		render_loop = setInterval(paint_snake, 100);
		if (latency_loop != "undefined") clearInterval(latency_loop);
		latency_loop = setInterval(timeStamp, 500);
	}
	
    //This function checks to see if the extrapolated snake1 is >= the newly received snake1
	function checkExtrapolatedSnake1(newSnake)
	{
        //Variables for finding distance
	    var a = newSnake[0].x - snake1[0].x;
	    var b = newSnake[0].y - snake1[0].y;
	    var c = Math.sqrt(a * a + b * b);
	    //Checks to see if snake goes into itself
	    //If snake is going to bite itself then extrapolated position = actual position
	    //Otherwise this is an old packet
	    if (c < 1 && snake1.indexOf(newSnake[0]) > -1)
	        snake1 = newSnake;
	        //Checks to see if new position is >= 3 steps away
	        //This implies an odd packet has been received.
	        //Either we have diverged greatly from the authoritative state
	        //Or the packet was a fluke and out of order.	    
	}

    //This function checks to see if the extrapolated snake2 is >= the newly received snake2
	function checkExtrapolatedSnake2(newSnake) {
	    //Variables for finding distance
	    var a = newSnake[0].x - snake2[0].x;
	    var b = newSnake[0].y - snake2[0].y;
	    var c = Math.sqrt(a * a + b * b);
	    //Checks to see if snake goes into itself
	    //If snake is going to bite itself then extrapolated position = actual position
	    //Otherwise this is an old packet
	    if (c < 1 && snake2.indexOf(newSnake[0]) > -1)
	        snake2 = newSnake;
	}

	function extrapolateSnake1()
	{
	    extrapolated_snake1 = snake1;
	    //The movement code for the snake to come here.
	    //The logic is simple
	    //Pop out the tail cell and place it infront of the head cell
	    var nx1 = extrapolated_snake1[0].x;
	    var ny1 = extrapolated_snake1[0].y;

	    //These were the position of the head cell.
	    //We will increment it to get the new head position
	    //Lets add proper direction based movement now
	    if (snake1Direction == "right") nx1++;
	    else if (snake1Direction == "left") nx1--;
	    else if (snake1Direction == "up") ny1--;
	    else if (snake1Direction == "down") ny1++;

	    //Lets add the game over clauses now
	    //This will restart the game if the snake hits the wall
	    //Lets add the code for body collision
	    //Now if the head of the snake bumps into its body, the game will restart
	    if (nx1 == -1 || nx1 == w / cw || ny1 == -1 || ny1 == h / cw ) { // || check_collision(nx1, ny1, snake_array1, snake_array2)) {
	        //pause game
	        extrapolated_playerIsDead = true;
	    }

	    var tail1;
	    tail1 = extrapolated_snake1.pop(); //pops out the last cell
        tail1.x = nx1; tail1.y = ny1;
        extrapolated_snake1.unshift(tail1); //puts back the tail as the first cell
        //Code for Printing extraploated_snake1
//        for (var i = 0; i < extrapolated_snake1.length; ++i)
//            log("es: " + extrapolated_snake1[i].x + ", " + extrapolated_snake1[i].y);
	}

	function extrapolateSnake2() {
	    extrapolated_snake2 = snake2;
	    //The movement code for the snake to come here.
	    //The logic is simple
	    //Pop out the tail cell and place it infront of the head cell
	    var nx2 = extrapolated_snake2[0].x;
	    var ny2 = extrapolated_snake2[0].y;
	    //These were the position of the head cell.
	    //We will increment it to get the new head position
	    //Lets add proper direction based movement now
	    if (snake2Direction == "right") nx2++;
	    else if (snake2Direction == "left") nx2--;
	    else if (snake2Direction == "up") ny2--;
	    else if (snake2Direction == "down") ny2++;

	    //Lets add the game over clauses now
	    //This will restart the game if the snake hits the wall
	    //Lets add the code for body collision
	    //Now if the head of the snake bumps into its body, the game will restart
	    if (nx2 == -1 || nx2 == w / cw || ny2 == -1 || ny2 == h / cw ){//|| check_collision(nx2, ny2, snake_array2, snake_array1))) {
	        //pause game
	        extrapolated_playerIsDead = true;
	    }
	    var tail2;
	    tail2 = extrapolated_snake2.pop(); //pops out the last cell
	    tail2.x = nx2; tail2.y = ny2;
	    extrapolated_snake2.unshift(tail2); //puts back the tail as the first cell
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

  	function setSnake1Direction(str)
  	{
  	    snake1Direction = str;
  	}

  	function setSnake2Direction(str)
  	{
  	    snake2Direction = str;
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
		var newSnake1 = [];
		//This will create a horizontal snake starting from the top left
		for (var i = 3; i < res.length; i += 2) //3 b/c res[0],res[1] are not coordinates
		    newSnake1.push({ x: res[i - 1], y: res[i] });
		if (typeof snake1 != "undefined" && snake1 != null && snake1.length > 0) {
		    extrapolateSnake1();
		    checkExtrapolatedSnake1(newSnake1);
		} else
		    snake1 = newSnake1;
	}

	function rollbackSnake1(res) {
	    snake1 = [];
	    //This will create a horizontal snake starting from the top left
	    for (var i = 4; i < res.length; i += 2) //2 b/c res[0] are not coordinates
	        snake1.push({ x: res[i - 1], y: res[i] });
	}
	
	function setSnake2(res)
	{
		var newSnake2 = [];
		//This will create a horizontal snake starting from the top left
		for (var i = 3; i < res.length; i += 2) //3 b/c res[0],res[1] are not coordinates
		    newSnake2.push({ x: res[i - 1], y: res[i] });
		if (typeof snake2 != "undefined" && snake2 != null && snake2.length > 0) {
		    extrapolateSnake2();
		    checkExtrapolatedSnake2(newSnake2);
		} else
		    snake2 = newSnake2;
	}

	function rollbackSnake2(res) {
	    snake2 = [];
	    //This will create a horizontal snake starting from the top left
	    for (var i = 4; i < res.length; i += 2) //2 b/c res[0] are not coordinates
	        snake2.push({ x: res[i - 1], y: res[i] });
	}
	
	function setFood(res)
	{
		food = { x: res[1], y: res[2] };
	}
	
	function setPlayerDied()
	{
	    playerIsDead = true;
	    clearInterval(render_loop)
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
	function paint_snake()
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
        ctx.font = "20px Arial";
        var score1_text = player2Name + "'s Score 2: " + score2;
        ctx.fillText(score1_text, 5, h - 5);
        var score2_text = player1Name + "'s Score 1: " + score1;
        ctx.fillText(score2_text, 5, h - 25);
        var latency_text = "Latency: " + latency;
        ctx.fillText(latency_text, 5, h - 45);
	}

	function paint_text() {
	    if (playerIsDead) {
	        //Lets paint the canvas now
	        ctx.fillStyle = "white";
	        ctx.fillRect(0, 0, w, h);
	        ctx.strokeStyle = "black";
	        ctx.strokeRect(0, 0, w, h);
	        //Lets paint the score
	        ctx.fillStyle = textColor;
	        ctx.font = "20px Arial";
	        var latency_text = "Latency: " + latency;
	        ctx.fillText(latency_text, 5, h - 65);
	        ctx.font = "30px Arial";
	        ctx.fillText("Player Died", 5, h - 35);
	        ctx.fillText("Press the 'r' key to reset", 5, h - 5);
	    }
	}

	function timeStamp(){
		a = new Date().getTime();
		send("Timestamp:");
	}
	
	function calcLatency(x,y){
		b = new Date().getTime();
		//log("clientTime: a = " + a + ", b = " + b);
		latency = ((b-a)-(y-x))/2;
		log("Latency: " + latency);
	}
	
	//Lets first create a generic function to paint cells
	function paint_cell(x, y, color)
	{
		ctx.fillStyle = color;
		ctx.fillRect(x*cw, y*cw, cw, cw);
		ctx.strokeStyle = "white";
		ctx.strokeRect(x*cw, y*cw, cw, cw);
	}

	function copy(o) {
	    var output, v, key;
	    output = Array.isArray(o) ? [] : {};
	    for (key in o) {
	        v = o[key];
	        output[key] = (typeof v === "object") ? copy(v) : v;
	    }
	    return output;
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
		if (oldD != d) {
		    send("Direction:" + d);
		    if (playerNumber == 1)
		        snake1Direction = d;
		    else
		        snake2Direction = d;
		}
		
		//Statement to reset the game
		if(playerIsDead && key == "82"){
		    playerIsDead = false;
		    extrapolated_playerIsDead = false;
		    snake1 = [];
		    snake2 = [];
		    extrapolated_snake1 = [];
		    extrapolated_snake2 = [];
			send("Reset");
		}
	});