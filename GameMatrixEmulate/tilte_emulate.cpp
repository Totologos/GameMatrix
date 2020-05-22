// 
// 
// 

#include "..\GameMatrixEmulate\tilte_emulate.h"

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

void Tilte::init(uint8_t x, uint8_t y)
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

char Tilte::Update(uint8_t elapsedTime)
{
	this->currentTime += elapsedTime;

	switch (this->state)
	{
	case TILTE_STATE_CHOISE:
		return 'c';

	case TILTE_STATE_LOCK:
		return '*';

	case TILTE_STATE_START:
		return 's';

	case TILTE_STATE_VALIDATED:
		return 'x';

	case TILTE_STATE_EMPTY:
	default:
		return '_';
	}

}


