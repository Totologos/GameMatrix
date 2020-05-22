// tilte.h

#ifndef _TILTE_h
#define _TILTE_h

#include "pch.h"
#include <stdint.h>

typedef enum tTilteStates
{
	TILTE_STATE_EMPTY,
	TILTE_STATE_LOCK,
	TILTE_STATE_START,
	TILTE_STATE_CHOISE,
	TILTE_STATE_VALIDATED
} tTilteStates;

class Tilte
{
private:
	uint8_t			x;
	uint8_t			y;
	tTilteStates	state;
	uint16_t		currentTime;

public:
	Tilte();
	void			init(uint8_t x, uint8_t y);

	void			SetState(tTilteStates state);
	tTilteStates	GetState(void);
	char			Update(uint8_t elapsedTime);
	uint8_t			GetX(void) { return this->x; }
	uint8_t			GetY(void) { return this->y; }

};

#endif
