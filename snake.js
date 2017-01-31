//Main Code taken from http://thecodeplayer.com/walkthrough/html5-game-tutorial-make-a-snake-game-using-html5-canvas-jquery
//Edited by Joshua Sosa to include a two player mode, custom colors, and new collisions.

$(document).ready(function(){
	//Canvas stuff
	var canvas = $("#canvas")[0];
	var ctx = canvas.getContext("2d");
	var w = $("#canvas").width();
	var h = $("#canvas").height();
	
	//Lets save the cell width in a variable for easy control
	var cw = 10;
	var d1;
    var d2;
	var food;
	var score1;
    var score2;
	var snake1Color = "Blue";
	var snake2Color = "Red";
	var foodColor = "Green";
	var textColor = "Black";
    var players = 1;
	
	//Lets create the snake now
	var snake_array1; //an array of cells to make up the snake1
	var snake_array2; //an array of cells to make up the snake2
  
	function init()
	{
		d1 = "right"; //default direction
		if(players == 2 ){
        d2 = "down";
        create_snakes();
        create_food();
      } else {
		create_snake();
		create_food(); //Now we can see the food particle
      }
		//finally lets display the score
		score1 = 0;
        score2 = 0;
		
		//Lets move the snake now using a timer which will trigger the paint function
		//every 60ms
		if(typeof game_loop != "undefined") clearInterval(game_loop);
		game_loop = setInterval(paint, 60);
	}
	init();
	
	function create_snake()
	{
		var length = 5; //Length of the snake
		snake_array1 = []; //Empty array to start with
		for(var i = length-1; i>=0; i--)
		{
			//This will create a horizontal snake starting from the top left
			snake_array1.push({x: i, y:0});
		}
	}
  
	function create_snakes()
	{
		var length1 = 5; //Length of the snake1
		var length2 = 5; //Length of the snake2
		snake_array1 = []; //Empty array to start snake1 with
		snake_array2 = []; //Empty array to start snake2 with
		for(var i = length1-1; i>=0; i--)
		{
			//This will create a horizontal snake starting from the top left
			snake_array1.push({x: i+1, y:0});
		}
        for(i = length2-1; i>=0; i--)
		{
			//This will create a horizontal snake starting from the top left
			snake_array2.push({x: 0, y:i});
		}
	}
	
	//Lets create the food now
	function create_food()
	{
		food = {
			x: Math.round(Math.random()*(w-cw)/cw), 
			y: Math.round(Math.random()*(h-cw)/cw)
		};
		//This will create a cell with x/y between 0-44
		//Because there are 45(450/10) positions accross the rows and columns
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
		
		//The movement code for the snake to come here.
		//The logic is simple
		//Pop out the tail cell and place it infront of the head cell
		var nx1 = snake_array1[0].x;
		var ny1 = snake_array1[0].y;
        var nx2 = snake_array2[0].x;
		var ny2 = snake_array2[0].y;
		//These were the position of the head cell.
		//We will increment it to get the new head position
		//Lets add proper direction based movement now
		if(d1 == "right") nx1++;
		else if(d1 == "left") nx1--;
		else if(d1 == "up") ny1--;
		else if(d1 == "down") ny1++;
		if(d2 == "right") nx2++;
		else if(d2 == "left") nx2--;
		else if(d2 == "up") ny2--;
		else if(d2 == "down") ny2++;
      
		//Lets add the game over clauses now
		//This will restart the game if the snake hits the wall
		//Lets add the code for body collision
		//Now if the head of the snake bumps into its body, the game will restart
		if(nx1 == -1 || nx1 == w/cw || ny1 == -1 || ny1 == h/cw || check_collision(nx1, ny1, snake_array1, snake_array2))
		{
			//restart game
			init();
			//Lets organize the code a bit now.
			return;
		}
        if(nx2 == -1 || nx2 == w/cw || ny2 == -1 || ny2 == h/cw || check_collision(nx2, ny2, snake_array2, snake_array1))
		{
			//restart game
			init();
			//Lets organize the code a bit now.
			return;
		}
		
		//Lets write the code to make the snakes eat the food
		//The logic is simple
		//If the new head position matches with that of the food,
		//Create a new head instead of moving the tail
        var tail1;
        if(nx1 == food.x && ny1 == food.y){
			tail1 = {x: nx1, y: ny1};
			score1++;
			//Create new food
			create_food();
		}else{
			tail1 = snake_array1.pop(); //pops out the last cell
			tail1.x = nx1; tail1.y = ny1;
		}
		//The snake can now eat the food.
		snake_array1.unshift(tail1); //puts back the tail as the first cell
        var tail2;
        if(nx2 == food.x && ny2 == food.y){
			tail2 = {x: nx2, y: ny2};
			score2++;
			//Create new food
			create_food();
		}else{
			tail2 = snake_array2.pop(); //pops out the last cell
			tail2.x = nx2; tail2.y = ny2;
		}
		//The snake can now eat the food.
		snake_array2.unshift(tail2); //puts back the tail as the first cell
			
        var c;
		for(var i = 0; i < snake_array1.length; i++)
		{
			c = snake_array1[i];
			//Lets paint 10px wide cells
			paint_cell(c.x, c.y, snake1Color);
		}
        for(i = 0; i < snake_array2.length; i++)
		{
			c = snake_array2[i];
			//Lets paint 10px wide cells
			paint_cell(c.x, c.y, snake2Color);
		}
		
		//Lets paint the food
		paint_cell(food.x, food.y, foodColor);
		//Lets paint the score
		ctx.fillStyle = textColor;
		var score1_text = "Score 1: " + score1;
		ctx.fillText(score1_text, 5, h-5);
        if(players == 2) {
            var score2_text = "Score 2: " + score2;
            ctx.fillText(score2_text, 120, h-5);
        }
	}
	
	//Lets first create a generic function to paint cells
	function paint_cell(x, y, color)
	{
		ctx.fillStyle = color;
		ctx.fillRect(x*cw, y*cw, cw, cw);
		ctx.strokeStyle = "white";
		ctx.strokeRect(x*cw, y*cw, cw, cw);
	}
	
	//This function will check if the provided arrays contain a matching x/y coordinate
	// or if the given x/y coordinates exist in an array of cells or not
	function check_collision(x, y, array1, array2)
	{
		//This loop will check if the provided x/y coordinates exist
		//in an array of cells or not
		for(var i = 0; i < array1.length; i++)
		{
			if(array1[i].x == x && array1[i].y == y)
               return true;
		}
		
		//This loop will check if the provided arrays contain a matching x/y coordinate
        for (i = 0; i < array1.length; i++) {
            for(var j = 0; j < array2.length; j++){
                if (array1[i].x == array2[j].x && array1[i].y == array2[j].y){
                    return true;
                }
            }
        }
		return false;
	}
	
	//Lets add the keyboard controls now
	$(document).keydown(function(e){
		var key = e.which;
		//We will add another clause to prevent reverse gear
		if(key == "37" && d1 != "right") d1 = "left";
		else if(key == "38" && d1 != "down") d1 = "up";
		else if(key == "39" && d1 != "left") d1 = "right";
		else if(key == "40" && d1 != "up") d1 = "down";
		if(players == 2){
			if(key == "65" && d2 != "right") d2 = "left";
			else if(key == "87" && d2 != "down") d2 = "up";
			else if(key == "68" && d2 != "left") d2 = "right";
			else if(key == "83" && d2 != "up") d2 = "down";
		} else {
			if(key == "65" && d1 != "right") d1 = "left";
			else if(key == "87" && d1 != "down") d1 = "up";
			else if(key == "68" && d1 != "left") d1 = "right";
			else if(key == "83" && d1 != "up") d1 = "down";
		}
		//The snake is now keyboard controllable
	});
		
});