# Among-Us Labyrinth

`What happens when the only crew,`      
`Are the killer imposter and you.`

***

**Varul Srivastava**        
**2019111015**

*** 

## CONTROLS 

1.  `1` to turn off lights
2. `0` to turn on lights
3. `w,a,s,d` for navigation
4. `SPACEBAR` for actions (like pressing a button)
5. `q` to quit

## Tasks 

1. Task 1 - Vapourize Imposter      
This task is to vapourize/ kill imposter by a button press. The imposter will be spawned in the beginning of the game and follow us untill it reaches us and kills us, or we vapourize it.

2. Task 2 - Treasure Unlock     
This task is to unlock the treasure (coins), and along with it, it also unlocks obstacles (fire-boxes), stepping on which, we will lose some life. It also opens doors for us to exit/finish the game.

## Scoring 

1. Treasure Points - The golden coins will contain 10 points each. There are 10 such coins.
2. Lights-Off Points - The player earns 2 points for each second the game's lights are off.

## Life 

1. Player loses life on colliding with obstacles (fire-boxes).
2. Player dies when comes in contact with the imposter.

## Timer 

1. Game runs for 300 seconds (5 minutes). This is challenging time to complete the entire game in light-off mode.

## Imposter 
1. Red colored spaceman is the imposter.

## Player 
1. Blue Colored spaceman is the player.

***PLAYER AND IMPOSTER LOOK LIKE AMONG US CHARACTERS***

## Maze 
New (unique) maze is generated each time using the random maze generation algorithm. The maze is in a 30x30 grid. Green color signifies road. Brown color signifies walls.

## Lighting 

Lighting is implemented, and it is made to be near-realistic experience.
There is a small radius of visibility, that remains constant. The rest depends on the walls that come in the way.

## Bonus implemented

1. After game is over, appropriate message is displayed.
2. The imposter uses shortest path to reach the player instead of brute force, thus making the game a-tad challenging.
3. Obstacles are shown as red boxes, denoting fire-boxes. Coins are in shape of diamond, and golden in color.


## Other things to Look out for 
1. The text rendered is without use of any external library, the text renderer is made from scratch.
2. Life is shown using a dynamic health bar along with the heads-up-display.
3. Some optimization was done to use GPU ram judiciously, and prevent memory leaks.


