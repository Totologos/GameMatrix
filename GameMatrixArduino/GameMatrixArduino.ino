/*
 Name:		GameMatrixAdruino.ino
 Created:	18/05/2020 09:53:43
 Author:	Totologos
*/
#include "GamePlay.h"

// size of game
#define GAME_WIDTH		7
#define GAME_HEIGHT		7
#define NUM_LEDS		(GAME_WIDTH * GAME_HEIGHT)

// hardware definition
#define DATA_LED_PIN	2

#define ARROW_UP_PIN  5
#define ARROW_DOWN_PIN  7
#define ARROW_LEFT_PIN  6
#define ARROW_RIGHT_PIN 8
#define BUTTON_A_PIN  9
#define BUTTON_B_PIN  10

Tilte** tiltes;
CRGB leds[NUM_LEDS];

uint8_t keys = KEY_NONE;
uint8_t keysPrev = KEY_NONE;
uint32_t endDebounce = 0;

// tilte position => led index
const uint8_t ledsIndex[GAME_WIDTH][GAME_WIDTH] =
{
	{ 0, 13, 14, 27, 28, 41, 42},
	{ 1, 12, 15, 26, 29, 40, 43},
	{ 2, 11, 16, 25, 30, 39, 44},
	{ 3, 10, 17, 24, 31, 38, 45},
	{ 4, 9,  18, 23, 32, 37, 46},
	{ 5, 8,  19, 22, 33, 36, 47},
	{ 6, 7,  20, 21, 34, 35, 48}
};

#define NUM_OF_LEVELS	10
uint8_t levels[] =

	{
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1,

		1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 1, 1, 1, 1,

		1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1,

		1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 1, 1, 1, 1,

		1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1,

		0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
	
		1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1,
};

// the setup function runs once when you press reset or power the board
void setup()
{
	// init hardware ressources 
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(ARROW_DOWN_PIN, INPUT_PULLUP);
	pinMode(ARROW_UP_PIN, INPUT_PULLUP);
	pinMode(ARROW_LEFT_PIN, INPUT_PULLUP);
	pinMode(ARROW_RIGHT_PIN, INPUT_PULLUP);
	pinMode(BUTTON_A_PIN, INPUT_PULLUP);
	pinMode(BUTTON_B_PIN, INPUT_PULLUP);

	FastLED.addLeds<WS2811, DATA_LED_PIN, GRB>(leds, NUM_LEDS);


	// create tiltes matrix and init game play
	tiltes = new Tilte*[GAME_HEIGHT];
	for (uint8_t i = 0; i < GAME_WIDTH; i++)
	{
		tiltes[i] = new Tilte[GAME_WIDTH];
	}

	gamePlay.init((Tilte**) tiltes, GAME_WIDTH, GAME_HEIGHT, levels, NUM_OF_LEVELS);

	gamePlay.loadGame(0);

}

// the loop function runs over and over again until power down or reset
void loop()
{

	// Update display
	EVERY_N_MILLISECONDS(5)
	{
		for (uint8_t i = 0; i < GAME_HEIGHT; i++)
		{
			for (uint8_t j = 0; j < GAME_WIDTH; j++)
			{
				leds[ ledsIndex[j][i] ] = tiltes[i][j].Update(5);
			}
		}
		FastLED.show();
	}

	keys = KEY_NONE;
	keys |= !digitalRead(ARROW_DOWN_PIN) ? KEY_ARROW_DOWN : 0;
	keys |= !digitalRead(ARROW_UP_PIN) ? KEY_ARROW_UP : 0;
	keys |= !digitalRead(ARROW_LEFT_PIN) ? KEY_ARROW_LEFT : 0;
	keys |= !digitalRead(ARROW_RIGHT_PIN) ? KEY_ARROW_RIGHT : 0;
	keys |= !digitalRead(BUTTON_A_PIN) ? KEY_BUTTON_A : 0;
	keys |= !digitalRead(BUTTON_B_PIN) ? KEY_BUTTON_B : 0;

	if (keys != keysPrev && keys != KEY_NONE)
	{
		endDebounce = GET_MILLIS() + 50;	
		
	}
	else if (GET_MILLIS() > endDebounce && keys != KEY_NONE)
	{
		gamePlay.keyEvent((tKeys)keys);
	}
	else
	{
		gamePlay.keyEvent(KEY_NONE);
	}
	keysPrev = keys;
}
