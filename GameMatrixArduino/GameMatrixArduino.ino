/*
 Name:		Sketch1.ino
 Created:	18/05/2020 09:53:43
 Author:	Totologos
*/
#include "GamePlay.h"

#define GAME_WIDTH		7
#define GAME_HEIGHT		7
#define NUM_LEDS		(GAME_WIDTH * GAME_HEIGHT)

#define DATA_LED_PIN	3

#define ARROW_UP_PIN	1
#define ARROW_DOWN_PIN	1
#define ARROW_LEFT_PIN	1
#define ARROW_RIGHT_PIN	1
#define BUTTON_A_PIN	1
#define BUTTON_B_PIN	1

Tilte tiltes[GAME_WIDTH][GAME_HEIGHT];
CRGB leds[NUM_LEDS];

uint8_t keys = 0;
uint8_t keysPrev = 0;
uint32_t endDebounce = 0;
uint8_t keyStart = 0;

uint8_t level1[] =
{
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
};

// the setup function runs once when you press reset or power the board
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(ARROW_DOWN_PIN, INPUT_PULLUP);
	pinMode(ARROW_UP_PIN, INPUT_PULLUP);
	pinMode(ARROW_LEFT_PIN, INPUT_PULLUP);
	pinMode(ARROW_RIGHT_PIN, INPUT_PULLUP);
	pinMode(BUTTON_A_PIN, INPUT_PULLUP);
	pinMode(BUTTON_B_PIN, INPUT_PULLUP);


	for (uint8_t y = 0; y < 7; y++)
	{
		for (uint8_t x = 0; x < 7; x++)
		{
			tiltes[x][y] = Tilte(x, y);
		}
	}

	FastLED.addLeds<NEOPIXEL, DATA_LED_PIN>(leds, NUM_LEDS);

	gamePlay.init((Tilte**)tiltes, GAME_WIDTH, GAME_HEIGHT);

	gamePlay.loadGame(level1);
}

// the loop function runs over and over again until power down or reset
void loop()
{


	// Update display
	EVERY_N_MILLISECONDS(5)
	{
		for (uint8_t i = 0; i < GAME_HEIGHT; i++)
		{
			for (uint8_t j = 0; j < GAME_WIDTH; i++)
			{
				leds[i * GAME_HEIGHT + j] = tiltes[i][j].Update(5);
			}
		}
		FastLED.show();
	}

	keys |= digitalRead(ARROW_DOWN_PIN) ? KEY_ARROW_DOWN : 0;
	keys |= digitalRead(ARROW_UP_PIN) ? KEY_ARROW_UP : 0;
	keys |= digitalRead(ARROW_LEFT_PIN) ? KEY_ARROW_LEFT : 0;
	keys |= digitalRead(ARROW_RIGHT_PIN) ? KEY_ARROW_RIGHT : 0;
	keys |= digitalRead(BUTTON_A_PIN) ? KEY_BUTTON_A : 0;
	keys |= digitalRead(BUTTON_B_PIN) ? KEY_BUTTON_B : 0;

	if (keys != keysPrev)
	{
		endDebounce = GET_MILLIS() + 50;
		keysPrev = keys;
	}
	else if (GET_MILLIS() > endDebounce)
	{
		gamePlay.keyEvent((tKeys)keys);
	}

	delay(2);
}
