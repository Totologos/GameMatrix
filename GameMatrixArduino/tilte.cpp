// 
// 
// 

#include "tilte.h"

#define BLINK_PERIOD	500
#define BLINK_MIN_DIM	10
#define BLINK_MAX_DIM	200

#define TILTE_DIM		200
#define TILTE_RICH		200

#define TILTE_COLOR_LOCK			0	// Red
#define TILTE_COLOR_START			96	// Green
#define TILTE_COLOR_CHOISE			64	// Yellow
#define  TILTE_COLOR_VALIDATED		32	// Orange


Tilte::Tilte()
{
	this->x = 0;
	this->y = 0;
	this->state = TILTE_STATE_EMPTY;
	this->currentTime = 0;
}

Tilte::Tilte(uint8_t x, uint8_t y)
{
	this->x = x;
	this->y = y;
	this->state = TILTE_STATE_EMPTY;
	this->currentTime = 0;
}

void Tilte::SetState(tTilteStates state)
{
	this->state = state;
}

tTilteStates Tilte::GetState(void)
{
	return this->state;
}

CRGB Tilte::Update(uint8_t elapsedTime)
{
	this->currentTime += elapsedTime;

	switch (this->state)
	{
	case TILTE_STATE_CHOISE :
		if (this->currentTime < BLINK_PERIOD)
		{
			// dim up
			return CHSV(BLINK_MIN_DIM + ((BLINK_MAX_DIM - BLINK_MIN_DIM) * this->currentTime / BLINK_PERIOD), TILTE_RICH, 0);
		}		
		else if (this->currentTime < (2 * BLINK_PERIOD) )
		{
			// dim down
			return CHSV(BLINK_MAX_DIM - ((BLINK_MAX_DIM - BLINK_MIN_DIM) * (this->currentTime - BLINK_PERIOD) / BLINK_PERIOD), TILTE_RICH, 0);
		}
		else
		{
			this->currentTime = 0;
			return CHSV(BLINK_MAX_DIM, 0, 0);
		}
		
	case TILTE_STATE_LOCK:
		return CHSV(TILTE_DIM, TILTE_RICH, TILTE_COLOR_LOCK);

	case TILTE_STATE_START:
		return CHSV(TILTE_DIM, TILTE_RICH, TILTE_COLOR_START);

	case TILTE_STATE_VALIDATED:
		return CHSV(TILTE_DIM, TILTE_RICH, TILTE_COLOR_VALIDATED);

	case TILTE_STATE_EMPTY:
	default:
		return CRGB(0, 0, 0); // off !
	}
	
}


