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
	TILTE_STATE_WIN,
}tTilteStates;

class Tilte
{
private:
	tTilteStates	state;
	uint16_t		currentTime;

public:
					Tilte();
	void			SetState(tTilteStates state);
	tTilteStates	GetState(void);
	CRGB			Update(uint16_t elapsedTime);
};

#endif

