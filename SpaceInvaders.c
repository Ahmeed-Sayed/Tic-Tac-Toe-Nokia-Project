// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs whiPlayeddraw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs whiPlayeddraw ~80mA total

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;


// *************************** Images ***************************



const unsigned char x[] ={
 0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x70, 0x00, 0x8F, 0xF0, 0x00, 0x07, 0x00, 0x00, 0x70, 0x00,
 0x7F, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x00, 0x00, 0xFF, 0x80,
 0x00, 0x00, 0x07, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x08, 0xFF, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x07, 0x8F, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
 0x07, 0x80, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x00, 0x07, 0x00, 0x00, 0x77, 0x77, 0x7F, 0xF8, 0x77, 0x78, 0x00, 0x00, 0xFF,

};
const unsigned char o[] ={
 0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xF7, 0x70, 0x07, 0x7F, 0xFF, 0x00, 0x00, 0xF8, 0x00,
 0x00, 0x00, 0x00, 0x8F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x70, 0x00, 0x07, 0x70, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x7F, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0x07, 0x00, 0x00, 0x70, 0x00, 0x07, 0x70, 0x00, 0x07, 0x00, 0x00, 0xF0, 0x00,
 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x00, 0x00, 0xFF, 0xF8, 0x77, 0x77, 0x8F, 0xFF, 0x00, 0x00, 0xFF,

};

const unsigned char movecursior[] ={
 0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
};
const unsigned char movecursiorRemoved[] ={
 
	0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0xD7, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
};


// *************************** Capture image dimensions out of BMP**********
#define row_1 0
#define row_2 1
#define row_3 2
#define resultCursior 7
#define scoreCursior  9

unsigned short Yloc = 1, Xloc = 0, Tcounter = 0; 
unsigned short movment, choice;
char gameboard[3][3]={0},Played= 'o';
int Player =  1;

void PortF_Init(void);
void moveToNext();
void playchoosed();
void resetArray();
void Game_result() ;
void ShowWinner();


int main(void){
	int movmentF = 0, choiceF = 0 ;
	TExaS_Init(NoLCD_NoScope); 
	PortF_Init();
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_Clear();
  EnableInterrupts();
	// Start screen 
	Nokia5110_SetCursor(2, 1);
	Nokia5110_OutString("X-O Game");
	Nokia5110_SetCursor(2, 2);
	Nokia5110_OutString("Made by");
	Nokia5110_SetCursor(1, 3);
	Nokia5110_OutString("Ahmed Sayed");
	
	Delay100ms(2);
	
	Nokia5110_ClearBuffer();
	resetArray(); //make the gamebaord array = 0

	
  while(1){ 
		choice   =  GPIO_PORTF_DATA_R & 0x10 ;  // assign choose to SW1
    movment  =  GPIO_PORTF_DATA_R & 0x01 ;  // assign moving to SW2
   	if(!movment && !movmentF ) {
			moveToNext(); // Toggle to next location
			movmentF = 1 ; // movment flag to avoid bugs if the user kept pressing SW2
			
		}if( !choice  && !choiceF ){                      //SW2 is pressed
			playchoosed(); // Put X or O in the current location
 			Game_result(); // check if someone won or if it's a tie
			choiceF = 1 ;
		}
		// Avoid error if the user keept pressing SW1 or SW2
		if(movment != 0x00 ){
			movmentF = 0 ;
		}
		if(choice != 0x00 ){
			choiceF = 0 ;
		}
	}	    
 }	


void ShowWinner()  // print the game result to the screen
{
	if(Tcounter==9){ // if the players played9 times and no winer was found, print it is a tie game
	Nokia5110_ClearBuffer() ;
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2,2);
	Nokia5110_OutString("tie game!!");}
	else{  // if X or O won, print the winner
	Nokia5110_ClearBuffer() ;
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor( 2 , 2 );
	Nokia5110_OutChar(Played);
	Nokia5110_SetCursor(4, 2);
	Nokia5110_OutString("Wins!!");
	}
	Delay100ms(2);
	resetArray(); // Set the game array to 0 again
	Xloc=0;
	Yloc=1;
	Tcounter=0;
		
}
void resetArray()
{int i = 0 ;
	int j = 0 ;
	for(  i = 0; i < 3 ; i++){
	for(  j = 0; j < 3 ; j++){
			gameboard[i][j] = 0;
		}
	}
}
void moveToNext()
{
	
	if (gameboard[Yloc-1][Xloc] == 0 ){ // if the current location is free to play on
	   Nokia5110_PrintBMP(Xloc*32+2, Yloc*16-2, movecursiorRemoved, 0); // remove the cursior icon so X or O can be played on
	   Nokia5110_DisplayBuffer();
	}
	else if (gameboard[Yloc-1][Xloc] == 'x') // if the current location has X symbol , put X on the current location
	{
		Nokia5110_PrintBMP(Xloc*32+2, Yloc*16-2, x, 0);
	  Nokia5110_DisplayBuffer();
	}
	else if (gameboard[Yloc-1][Xloc] == 'o') // if the current location has o symbol, put o on the current location
	{
		Nokia5110_PrintBMP(Xloc*32+2, Yloc*16-2, o, 0);
	  Nokia5110_DisplayBuffer();
	}
	Xloc++;
	// if the last row was reached, return to the first one
  if( (Xloc+1) % 4 == 0)
	{
		Xloc = 0;
		Yloc++;
		// if the last coloumn was reached, return to the first one
		if( (Yloc+1) % 5 == 0)
		{
			Yloc = 1 ;
		}
	}	
	if(gameboard[Yloc-1][Xloc]!=0){ // if the current location has X or O in it , skip and move to the next location
		moveToNext();
		return;
	}
	Nokia5110_PrintBMP(Xloc*32+2, Yloc*16-2, movecursior, 0); //move cursior to the new location
	Nokia5110_DisplayBuffer();	
		
}

void Game_result(){ // check if someone one
	
 if( gameboard[0][0] == gameboard[0][1] && gameboard[0][1] == gameboard[0][2] && gameboard[0][0] != 0)   //first coloum check
 {
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	 if(gameboard[0][0]!='x'){
	
			 Played='o';}
	  
    
	 ShowWinner();
	
 }
 else if (gameboard[1][0] == gameboard[1][1] && gameboard[1][1] == gameboard[1][2] && gameboard[1][0] != 0){ // second coloumn check
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
 if(gameboard[1][0]!='x'){
	
			 Played='o';}
	  
	 ShowWinner();
	}
  else if (gameboard[2][0] == gameboard[2][1] && gameboard[2][1] == gameboard[2][2] && gameboard[2][0] != 0){  // third coloumn check
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	 if(gameboard[2][0]!='x'){
	  
			 Played='o';}
	  

	 ShowWinner();
}
	else if (gameboard[0][0] == gameboard[1][0] && gameboard[1][0] == gameboard[2][0] && gameboard[1][0] != 0){  // first row check
	 
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	  if(gameboard[0][0]!='x'){
	
			 Played='o';}
		

	 ShowWinner();
}
	else if (gameboard[0][1] == gameboard[1][1] && gameboard[1][1] == gameboard[2][1] && gameboard[0][1] != 0){  //second row check
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	if(gameboard[0][1]!='x'){
	
			 Played='o';}
	  

	 ShowWinner();
}
	else if (gameboard[0][2] == gameboard[1][2] && gameboard[1][2] == gameboard[2][2] && gameboard[0][2] != 0){ // third row check
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	if(gameboard[0][2]!='x'){
	
			 Played='o';}
	
	 ShowWinner();
}
	else if (gameboard[0][0] == gameboard[1][1] && gameboard[1][1] == gameboard[2][2] && gameboard[0][0] != 0){  // left to right diagonal check
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	if(gameboard[0][0]!='x'){ // assign played to the winner
	  
			 Played='o';}
	  

	 ShowWinner();
}
	else if (gameboard[2][0] == gameboard[1][1] && gameboard[1][1] == gameboard[0][2] && gameboard[2][0] != 0){ // right to left diagonal ccheck
	  
	  Nokia5110_ClearBuffer() ;
	  Nokia5110_DisplayBuffer();
	if(gameboard[2][0]!='x'){
	 
			 Played='o';}
	 
	 ShowWinner();
}
	
 
if (Tcounter==9){ // if it is a tie go to show winner function
	ShowWinner();
}}
void playchoosed(){  // put the current player symobol on board

    if(Player == 1 )
    {
        if(gameboard[Yloc-1][Xloc] == 0){  
                Nokia5110_PrintBMP(Xloc*32+2, Yloc*16-2, movecursiorRemoved, 0); // remove the cursior from the current location
                Nokia5110_DisplayBuffer();

                Nokia5110_PrintBMP(Xloc*32+2, Yloc*16-2, x, 0); // put x on the current location
                Nokia5110_DisplayBuffer();

              Player = 2 ;// change player

              gameboard[Yloc-1][Xloc] = 'x'; // save x on the game array
              Played= 'x';
					     Tcounter++; // one move exerted
        }
    }
    else
    {
        if(gameboard[Yloc-1][Xloc] == 0){
          Nokia5110_PrintBMP(Xloc *32+2, Yloc*16-2, movecursiorRemoved, 0);   // remove the cursior from the current location
            Nokia5110_DisplayBuffer();

            Nokia5110_PrintBMP(Xloc *32+2, Yloc*16-2, o , 0);// put o on the current location
            Nokia5110_DisplayBuffer();

          Player = 1 ; //change player

          gameboard[Yloc-1][Xloc] = 'o'; // save o on the game array
          Played= 'o';
					Tcounter++; // one move exerted
        }
    }

}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0 
}

// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
}
void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
