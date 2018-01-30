/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

char textstring[] = "text, more text, and even more text!";

// Game attributes
int timeoutcount = 0;			// interrupt controller
int timeoutcountHI = 0;   // Highscore counter
int prime = 0000;         // starting highstore
int speed = 10;					// starting speed

int looper = 0;			// location for the sky
int opposite = 64;		// location for fround
int oppObst = 120;		// location for cactus 1
int oppObst2 = 48;		// location for snake
int oppObst22 = 0;    // speed factor of a snake
int oppObst3 = 48;		// location for cactus 2
int oppObst4 = 83;		// location for bird 1
int oppObst5 = 8;		  // location for bird 2
int oppCloud = 40;    // location for clouds

// pseudo random locationtion generator
int primeC = 300;
int primeC2 = 2000;
int primeC3 = 2000;
int primeC4 = 300;
int primeC5 = 1000;

#define STATE_START     0
#define STATE_GAME      1
#define STATE_END       2
int gamestate = STATE_START;

void user_isr( void )
{
 switch(gamestate){

   case STATE_START:
   display_logo(1, logo);
   int timeoutcount = 0;			// interrupt controller
   int timeoutcountHI = 0;   // Highscore counter
   int prime = 0000;         // starting store
   int speed = 10;					// starting speed

   int looper = 0;			// location for the sky
   int opposite = 64;		// location for fround
   int oppObst = 120;		// location for cactus 1
   int oppObst2 = 48;		// location for snake
   int oppObst22 = 0;    // speed factor of a snake
   int oppObst3 = 48;		// location for cactus 2
   int oppObst4 = 81;		// location for bird 1
   int oppObst5 = 8;		  // location for bird 2
   int oppCloud = 40;    // location for clouds

   volatile int * portE = (volatile int *) 0xbf886110;    //resetting the LEDs
 	*portE = 0x00;

   // pseudo random locationtion generator
   int primeC = 300;
   int primeC2 = 2000;
   int primeC3 = 2000;
   int primeC4 = 300;
   int primeC5 = 1000;

   if (getbtns() == 0x4)
      gamestate = STATE_GAME;
      break;

   case STATE_GAME:
   // Everytime an interrupt occurs this should happen:
 	IFS(0) &= ~0x00000100;				// reset the interrupt flag to 0

 	// updating the game and highscore counters
 	timeoutcount++;
  timeoutcountHI++;

   // highscore implementation
   if ( timeoutcountHI == 100)
   {
     prime = nextprime( prime );
     if ((getbtns() == 0) && (prime > 641)) // after reaching 641 (approximately 2 minutes in game time, the score will
       prime = nextprime( prime );           // be incremented twice as fast to make the game more awarding in the late stage)
     display_string( 0, itoaconv( prime ) );
     timeoutcountHI = 0;
     if (prime == 641)
       oppObst4 = oppObst4 -45;      //surprise bird change of position
   }

 	//  checking the game counter and performing the render accordingly
 	if ( timeoutcount == speed){

 		display_update();
 		// displaying cactus 1 and implementing pseudo random to its next spawning delay time
 		if (oppObst != 0)
 		{
 		  display_obstacle(oppObst, cactus);
 		  oppObst--;
 		}
 		if ( (is_prime (primeC++) == 1) && (oppObst == 0) )
 		{
 		  oppObst = 128;
 		  display_obstacle(oppObst, cactus);
 		}

 		// displaying bird 1 and implementing pseudo random to its next spawning delay time
 		if (oppObst4 != 0)
 		{
 		  display_bird(oppObst4, bird);
 		  oppObst4--;

 		  if ((oppObst == oppObst4) || (oppObst3 == oppObst4))
 			  oppObst4 = oppObst4 + 10;
 		}
 		if ( (is_prime (primeC4++) == 1) && (oppObst4 == 0) )
 		{
 		  oppObst4 = 128;
 		  display_bird(oppObst4, bird);
 		}

    // not implemented as the game becomes too difficult
 		// bird 2
 		/*
 		if (oppObst5 != 0)
 		{
 		  display_bird(oppObst5, bird);
 		  oppObst5--;
 		  if ((oppObst == oppObst5) || (oppObst3 == oppObst5))
 			  oppObst5 = oppObst5 + 10;

 		}
 		if ( (is_prime (primeC5++) == 1) && (oppObst5 == 0) )
 		{
 		  oppObst5 = 128;
 		  display_bird(oppObst5, bird);
 		  if(primeC5 > 1070)
 			  primeC5 = 1000;
 		}
 		*/

 		// displaying snake and implementing pseudo random to its next spawning delay time
 		if ((oppObst2 + oppObst22 +2) != 0)
 		{
 		  display_obstacle(oppObst2 + oppObst22 +2, snake);
 		  oppObst2--;
 		  oppObst22--;
 		}
 		if ( (is_prime (primeC2--) == 1) && ((oppObst2 + oppObst22 +2) == 0) ) //  random spawning
 		{
 		  oppObst2 = 128;
 		  oppObst22 = 0;
 		  display_obstacle(oppObst2 + oppObst22 +2, snake);
 		  if(primeC2 < 1985)
 			  primeC2 = 2000;
 		}

 		// displaying cactus 2 and implementing pseudo random to its next spawning delay time
 		if (oppObst3 != 0)
 		{
 		  display_obstacle(oppObst3, cactus);
 		  oppObst3--;
 		}
 		if ( (is_prime (primeC3++) == 1) && (oppObst3 == 0) )
 		{
 		  oppObst3 = 128;
 		  display_obstacle(oppObst3, cactus);
 		  if(primeC3 > 2017)
 			  primeC3 = 2000;
 		}

     // displaying ground and clouds
 		display_ground(opposite, ground);
 		opposite--;
     display_cloud(oppCloud, cloud);

     // displaying highscore
     display_HI(1, HI);

 		looper++;
 		timeoutcount = 0;

    if ( ((looper % 250) == 0 ) && ( speed > 5) )
      {speed--;
      (*( (volatile int *) 0xbf886110))++;  // incrementing portE to show the speed on the LEDs
      }

 	}

     // flying function
 	switch ( getbtns() ){

  default:
    display_character(8, fly);
    break;

 	case 0x4:    // BTN4 is pressed / the character is flying
 	  display_character_jump(8, fly);
 	  break;
 	}

 	// collision algorithm
 	// cactus 1
 	if  ( (( getbtns() == 0 ) || ( getbtns() == 2 ) || ( getbtns() == 1 )) && ( ( 3 < oppObst) && (oppObst < 13) ) ){
 	gamestate = STATE_END;}

 	// snake
 	if  ( (( getbtns() == 0 ) || ( getbtns() == 2 ) || ( getbtns() == 1 )) && ( ( 3 < (oppObst2 + oppObst22 +2) ) && ( (oppObst2 + oppObst22 +2)  < 13) ) ){
 	gamestate = STATE_END; }

 	// cactus 2
 	if  ( (( getbtns() == 0 ) || ( getbtns() == 2 ) || ( getbtns() == 1 )) && ( ( 3 < oppObst3) && (oppObst3 < 13) ) ){
 	gamestate = STATE_END; }

 	// bird 1
 	if  ( ( getbtns() == 4 ) && ( ( 3 < (oppObst4) ) && ( (oppObst4)  < 13) ) ){
 	gamestate = STATE_END; }   //modify to change the state

 	// bird 2
 	/*
 	if  ( ( getbtns() == 4 ) && ( ( 2 < (oppObst5) ) && ( (oppObst5)  < 14) ) ){
 	delay(1000); }
  */
   break;         // end of the case STATE_GAME

   case STATE_END:
   display_over (0, gameover);
   if (getbtns() == 0x2)
      gamestate = STATE_START;
      break;

 }

  return;
}


/* Lab-specific initialization goes here */
void labinit( void )
{
	volatile int * trise = (volatile int *) 0xbf886100;
	*trise = *trise & ~0xff;

	volatile int * portE = (volatile int *) 0xbf886110;
	*portE = 0x00;

	TRISD = TRISD | 0xFE0;              // setting the pins from 5 through 11 as inputs in port D

	T2CON = 0x70;                      //  pre-scaling by 1:256
	PR2 = (80000000 / 256) / 200;      //  choosing the right period, the counter counts up to 31250
	TMR2 = 0;                        //  initializing the timer
	T2CONSET = 0x8000;               //  start the timer

	// added code for interrupts part

	int * iec0 = (int *) 0xbf881060;                // enable the external  interrupt
	* iec0 = 0x00000100;   //IEC0bits.T2IE = 1;

	 asm volatile("ei");                           // enable interrupts on the micro-controller

	int * ipc2 = (int *) 0xbf8810b0;
	* ipc2 = 0x0000001f;                          // setting the priority to 7 and subpriority to 3

	// added code for the extrnal intrupt
	int * ipc1 = (int *) 0xbf8810a0;
	* ipc1 = 0x0000001f;                          // setting the priority to 7 and subpriority to 3

  return ;
}
