// tilte.h

#ifndef _TILTE_h
#define _TILTE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "FastLED.h"


typedef enum tTilteStates
{
	TILTE_STATE_EMPTY,
	TILTE_STATE_LOCK,
	TILTE_STATE_START,
	TILTE_STATE_CHOISE,
	TILTE_STATE_VALIDATED,
	TILTE_STATE_LOOSE,
}tTilteStates;

class Tilte
{
private:
	uint8_t			x;
	uint8_t			y;
	tTilteStates	state;
	uint16_t		currentTime;

public:
					Tilte();

	void			Init(uint8_t x, uint8_t y);
	void			SetState(tTilteStates state);
	tTilteStates	GetState(void);
	CRGB			Update(uint16_t elapsedTime);
	uint8_t			GetX(void)						{ return this->x; }
	uint8_t			GetY(void)						{ return this->y; }

};

#endif

