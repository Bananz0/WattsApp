/*	Pictor.h	VERSION 0.3.2
 *	Screen driver for the IlMatto and 2.2" screen combo
 *	Use port A for control & port C for data.
 *
 *	Note that this code is subject to change, check the changelog when updating.
 *	Project by Henry Wilson, Terra Barber, Emma Curati-Alasonatti and Chris Turner.
 *
 *	This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 licence
 *	This licence can be found at http://creativecommons.org/licenses/
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PICTOR
#define PICTOR

#ifndef PICTOR_FONTSTART
#define PICTOR_FONTSTART ' '
#endif
#ifndef PICTOR_FONTEND
#define PICTOR_FONTEND '~'
#endif

//	Generic libraries
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//	AVR libraries
#include <avr/io.h>
//#include <avr/interrupt.h>
#include <util/delay.h>//needed for screen initialisation
#include <avr/pgmspace.h> //TODO: Work out where this really should go...

//	POINT STRUCTURE	TODO: Look into signed mode.
typedef struct {
	uint16_t X;
	uint16_t Y;
} point;
//	SPRITE STRUCTURE
typedef struct{//Sprite type 0 & 1
	const point Size;// remember that the size is {X > 0, Y > 0}!
	uint16_t* RGB; //can be in PROGMEM
}sprite;
typedef struct{//Sprite type 2 & 3
	const point Size;
	uint8_t* data; //can be in PROGMEM
	uint16_t FGcolour;
	uint16_t BGcolour;
}spriteMono;
typedef struct{//Sprite type 4, 5, 6 & 7
	const point Size;
	uint8_t* data; //can be in PROGMEM
	uint16_t* palette; //can be in PROGMEM
}spriteEncoded;
typedef struct{//Sprite type 8, 9, A & B
	const point Size;
	uint8_t* data; //can be in PROGMEM
	uint16_t* palette; //can be in PROGMEM
}spritePalette;
//	FONT STRUCTURE
#define font uint8_t

extern uint8_t pictorRot;

//	DATA PORTS -- don't use port B if you intend to use USB!
#define CTRLPORT	PORTB
#define CTRLDDR		DDRB
#define DATAPORT	PORTC
#define DATADDR		DDRC
#define DATAPINS	PINC

//	CONRTOL BITS
#define BACKLIGHT	0b00000010
#define RESET		0b11111011
#define WRITE		0b11110111
#define COMND		0b11101111
#define READ		0b11011111
#define VSYNC		0b01000000

//	USEFUL
#define ON			1
#define OFF			0
#define INVERT		-1

//	POINTS
#define ORIGIN		(point){0,	0}
#define UNIT_POINT	(point){1,	1}
#define PICTOR_MAX	(point){239,319}
#define PICTOR_SIZE	(point){240,320}

// //	COLOURS		  RRRRRGGGGGGBBBBB
// #define BLACK	0b0000000000000000
// #define WHITE	0b1111111111111111
// #define RED		0b1111100000000000
// #define ORANGE	0b1111101011100000
// #define YELLOW	0b1111111111100000
// #define GREEN	0b0000011111100000
// #define CYAN	0b0000011111111111
// #define BLUE	0b0000000000011111
// #define MAGENTA	0b1111100000011111
// //	COLOUR MODIFIERS
// #define PALE	0b1000010000010000 |
// #define DEEP	0b0111001111001110 &
// #define DARK	0b0011000111000110 &

// Original base colors with 5 bits each for RGB (15 bits total)
#define BLACK           0b0000000000000000  // 0,0,0
#define WHITE           0b1111111111111111  // 31,31,31
#define RED             0b1111100000000000  // 31,0,0
#define DARK_RED        0b1000000000000000  // 16,0,0
#define LIGHT_RED       0b1111110000010000  // 31,1,0
#define ORANGE          0b1111101011100000  // 31,23,0
#define DARK_ORANGE     0b1011000111000000  // 22,14,0
#define LIGHT_ORANGE    0b1111111100000000  // 31,31,0
#define YELLOW          0b1111111111100000  // 31,31,0
#define DARK_YELLOW     0b1011011011000000  // 22,22,0
#define LIGHT_YELLOW    0b1111111111110011  // 31,31,3
#define GREEN           0b0000011111100000  // 0,31,0
#define DARK_GREEN      0b0000001100000000  // 0,12,0
#define LIGHT_GREEN     0b1000111111111000  // 16,31,8
#define CYAN            0b0000011111111111  // 0,31,31
#define DARK_CYAN       0b0000001101101100  // 0,13,12
#define LIGHT_CYAN      0b1000111111111111  // 16,31,31
#define BLUE            0b0000000000011111  // 0,0,31
#define DARK_BLUE       0b0000000000001000  // 0,0,8
#define LIGHT_BLUE      0b1000000000111111  // 16,0,31
#define MAGENTA         0b1111100000011111  // 31,0,31
#define DARK_MAGENTA    0b1000000000001000  // 16,0,8
#define LIGHT_MAGENTA   0b1111111000111111  // 31,8,31
#define PURPLE          0b0111100000011110  // 15,0,14
#define PINK            0b1111101111101111  // 31,29,15
#define BROWN           0b1000010000000000  // 16,4,0
#define GRAY            0b1000010000100001  // 16,4,1
#define DARK_GRAY       0b0100001000010000  // 8,2,0
#define LIGHT_GRAY      0b1100110011001100  // 25,12,12

// Color modifiers (using bitwise operations)
#define PALE           0b1000010000010000 |  // Adds a gray tint
#define DEEP           0b0111001111001110 &  // Intensifies while preserving midtones
#define DARK           0b0011000111000110 &  // Reduces overall brightness
#define BRIGHT         0b1110111110111110 |  // Increases overall brightness
#define SATURATE       0b1110011110011110 |  // Enhances color intensity
#define DESATURATE     0b0111101111101111 &  // Moves color toward gray

// Suggested additional modifiers
#define WARM           0b1111101000100000 |  // Adds warm tint
#define COOL           0b0000001111111111 &  // Adds cool tint



//	Create a colour encoded in 656-RGB from individual RGB bytes.
uint16_t encode656Colour(const uint8_t Red, const uint8_t Green, const uint8_t Blue);

//===============
// BASE LCD CMDS
//===============
//	Sends a command byte to the screen.
void pictorCmdWrite(const uint8_t Cmd);
//	Sends a data byte to the screen.
void pictorByteWrite(const uint8_t Data);
//	Sends two data bytes to the screen.
void pictorWordWrite(uint16_t Value);
//	Returns a data byte read from the screen.
uint8_t pictorByteRead(void);
//	Returns a word from two data bytes read from the screen.
uint16_t pictorWordRead(void);
//	Sets the state of the back-light, 1 = ON, 0 = OFF, -1 = TOGGLE
void pictorBacklightState(const int8_t State);
//	Toggles the VSYNC pin to update the screen in VSYNC mode
void pictorFrame(void);

//================
// LCD ARRAY CMDS
//================
//	Writes a command and a byte array containing its parameters to the screen.
void pictorWrite(const uint8_t Cmd, const uint8_t* Buffer, const uint8_t Count);
//	Writes a command and reads out an array of byte data into a buffer from the screen.
void pictorRead(const uint8_t Cmd, uint8_t* Buffer, const uint8_t Count);

//====================
// LCD COMPOSITE CMDS
//====================
//	Sets a drawing canvas on the screen.
void pictorCanvasSet(const point A, const point B);
void pictorSetRotation(const uint8_t rotation);

//=========================
// LCD DRAW CHAR FUNCTIONS
//=========================
//	Draws Char onto the screen at Pos with two colours of Font.
void pictorDrawC(const unsigned char Char, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale);
//	Draws String onto the screen at Pos with two colours of Font.
uint8_t pictorDrawS(const unsigned char* Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale);
uint8_t pictorDrawSP(const unsigned char* Buffer, point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale);

//=========================
// LCD DRAW Number FUNCTIONS
//=========================
//	Draws a Number in decimal onto the screen at Pos, with two colours of Font, padded to fill len spaces(use a len of 0 to ignore padding).
void pictorDrawD(const int16_t Number, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale, uint8_t len);
//	Draws a Value in hex onto the screen at Pos with two colours of Font.
uint8_t pictorDrawX(const uint8_t Value, const point Pos, const uint16_t ForegroundColour, const uint16_t BackgroundColour, const font* Font, uint8_t scale);

void pictorDrawF(const double Number,const point Pos,const uint16_t ForegroundColour,const uint16_t BackgroundColour,const font* Font, uint8_t scale, uint8_t len) ;

//=================
// LCD DRAW SHAPES
//=================
//	Draws a single pixel with Colour at Pos
void pictorDrawPixel(const point Pos, const uint16_t Colour);
//      Draws a line with Colour between points A and B, using Bresenham's algorithm.
void pictorDrawLine(point A, point B, const uint16_t Colour);
//	Draws a solid box with Colour between points A and B.
void pictorDrawBox(point A, point B, const uint16_t Colour);
//	Fills the screen with Colour.
void pictorDrawAll(const uint16_t Colour);
//draw part of a sprite selected by X1 and X2
void pictorDrawSpritePartial(const sprite* Sprite, const point Pos, const uint8_t Scale, point X1, point X2);
// Draws a Sprite of type Type, top left aligned at Pos scaled by Scale.
void pictorDrawSpriteType(const void* Sprite, const point Pos, const uint8_t type, const uint8_t Scale);
//	Draws a circle using Bresenham's algorithm with a given centre point and radius
void pictorDrawCircle(const point Centre, const uint8_t Radius, const uint16_t Colour);
//	Draws a sprite at a given angle of rotation
void pictorDrawRotatedSprite(const void* Sprite, const point Pos, const uint8_t type, const uint8_t Scale, float angle);

//====================
// LCD INTITALISATION
//====================
//	Initialise the screen. Mode is a bit-field.
void pictorInit(const uint8_t Mode);

// Dump the entire of ram and program memory to the screen
void pictorMemDump(void);

#endif

#ifdef __cplusplus
}
#endif