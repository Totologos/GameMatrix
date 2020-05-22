// 
// 
// 

#include "tilte.h"

#define BLINK_PERIOD	(uint32_t)1000
#define BLINK_MIN_DIM	(uint32_t)10
#define BLINK_MAX_DIM	(uint32_t)190

#define TILTE_DIM		200
#define TILTE_RICH		230

#define TILTE_COLOR_LOCK			0	// Red
#define TILTE_COLOR_START			96	// Green
#define TILTE_COLOR_CHOISE			160	// Blue
#define  TILTE_COLOR_VALIDATED		64	// Yellow
#define TILTE_COLOR_LOOSE			192 // Purple


Tilte::Tilte()
{
	this->state = TILTE_STATE_EMPTY;
	this->currentTime = 0;
}

void Tilte::SetState(tTilteStates state)
{
	if (state != this->state)
	{
		this->currentTime = 0; 
		this->state = state;
	}	
}

tTilteStates Tilte::GetState(void)
{
	return this->state;
}

/* must be call as soon as possible 
 * return current color of LED */
CRGB Tilte::Update(uint16_t elapsedTime)
{
	this->currentTime += elapsedTime;

	switch (this->state)
	{
	case TILTE_STATE_CHOISE :
		if (this->currentTime < BLINK_PERIOD)
		{
			// dim down
			return CHSV(TILTE_COLOR_CHOISE, TILTE_RICH, BLINK_MAX_DIM - ((BLINK_MAX_DIM - BLINK_MIN_DIM) * (uint32_t)this->currentTime / BLINK_PERIOD));
		}		
		else if (this->currentTime < (2 * BLINK_PERIOD) )
		{
			// dim up
			return CHSV(TILTE_COLOR_CHOISE, TILTE_RICH, BLINK_MIN_DIM + ((BLINK_MAX_DIM - BLINK_MIN_DIM) * ((uint32_t)this->currentTime - BLINK_PERIOD) / BLINK_PERIOD));
		}
		else
		{
			this->currentTime = 0;
			return CHSV(TILTE_COLOR_CHOISE, TILTE_RICH, BLINK_MAX_DIM);
		}
		
	case TILTE_STATE_LOCK:
		return CHSV(TILTE_COLOR_LOCK, TILTE_RICH, TILTE_DIM);

	case TILTE_STATE_WIN:
	case TILTE_STATE_START:
		return CHSV(TILTE_COLOR_START, TILTE_RICH, TILTE_DIM);

	case TILTE_STATE_VALIDATED:
		return CHSV(TILTE_COLOR_VALIDATED, TILTE_RICH, TILTE_DIM);

	case TILTE_STATE_LOOSE:
		return CHSV(TILTE_COLOR_LOOSE, TILTE_RICH, TILTE_DIM);
	case TILTE_STATE_EMPTY:
	default:
		return CRGB(0, 0, 0); // off !
	}
	
}


