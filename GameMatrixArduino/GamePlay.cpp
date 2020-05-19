// 
// 
// 

#include "GamePlay.h"



void GamePlay::init(Tilte** tiltes, uint8_t width, uint8_t height)
{
	this->tiltes = tiltes;
	this->width = width;
	this->height = height;


}

void GamePlay::loadGame(uint8_t* level)
{
	for (uint8_t y = 0; y < this->height; y++)
	{
		for (uint8_t x = 0; x < this->width; x++)
		{
			this->tiltes[x][y].SetState(level[ y * this->width + x ] == 1 ? TILTE_STATE_LOCK : TILTE_STATE_EMPTY);
		}
	}

	resetGame();
}

void GamePlay::resetGame()
{
	bool foundStart = false;
	this->curX = 0;
	this->curY = 0;

	this->gameSequence = GAME_SEQUENCE_START_POINT;

	for (uint8_t y = 0; y < this->height; y++)
	{
		for (uint8_t x = 0; x < this->width; x++)
		{
			if (this->tiltes[x][y].GetState() != TILTE_STATE_LOCK)
			{
				this->tiltes[x][y].SetState(TILTE_STATE_EMPTY);
				if (!foundStart)
				{
					this->curX = x;
					this->curY = y;
					foundStart = true;
				}
			}
		}
	}
	this->tiltes[this->curY][this->curX].SetState(TILTE_STATE_CHOISE);
}

void  GamePlay::keyEvent(tKeys key)
{
	if (this->lastKey != key)
	{
		switch (this->gameSequence)
		{
		case GAME_SEQUENCE_START_POINT:
			switch (key)
			{
			case KEY_ARROW_UP:
			case KEY_ARROW_DOWN:
			case KEY_ARROW_LEFT:
			case KEY_ARROW_RIGHT:
				this->tiltes[this->curY][this->curX].SetState(TILTE_STATE_EMPTY);
				do
				{
					switch (key)
					{
					case KEY_ARROW_UP:		this->curY = (this->curY - 1 + this->height) % this->height;	break;
					case KEY_ARROW_DOWN:	this->curY = (this->curY + 1 + this->height) % this->height;	break;
					case KEY_ARROW_LEFT:	this->curY = (this->curX - 1 + this->width) % this->width;		break;
					case KEY_ARROW_RIGHT:	this->curY = (this->curX + 1 + this->width) % this->width;		break;
					}
				} while (this->tiltes[this->curY][this->curX].GetState() != TILTE_STATE_EMPTY);
				this->tiltes[this->curY][this->curX].SetState(TILTE_STATE_CHOISE);
				break;

			case KEY_BUTTON_A:
				this->tiltes[this->curY][this->curX].SetState(TILTE_STATE_START);
				this->gameSequence = GAME_SEQUENCE_MOOVED;
				this->selectedDirection = GAME_PLAY_DIR_NONE;
				this->update();
				break;
			case KEY_BUTTON_B:
				// do nothing... maybe select another level
				break;

			}
			break;

		case GAME_SEQUENCE_MOOVED:
			switch (key)
			{
			case KEY_ARROW_UP:
				if (canSelect(this->curX, this->curY - 1))
				{
					eraseChoose();
					this->curY--;
					this->tiltes[this->curY][this->curX].SetState(TILTE_STATE_VALIDATED);
					this->selectedDirection = GAME_PLAY_DIR_UP;
					update();
				}
				break;
			}
		}
	}
	this->lastKey = key;
}

void  GamePlay::update(void)
{

	uint8_t numOfChoise = 0;
	if (this->canMoove(this->curX + 1, this->curY))
		numOfChoise++;

	if (this->canMoove(this->curX - 1, this->curY))
		numOfChoise++;

	if (this->canMoove(this->curX, this->curY + 1))
		numOfChoise++;

	if (this->canMoove(this->curX, this->curY - 1))
		numOfChoise++;

	if (numOfChoise == 0)
	{
		// game over or win !

	}
	else if (this->selectedDirection == GAME_PLAY_DIR_NONE)
	{
		// wait user press à key
		if (this->canMoove(this->curX + 1, this->curY))
			this->tiltes[this->curX + 1, this->curY]->SetState(TILTE_STATE_CHOISE);

		if (this->canMoove(this->curX - 1, this->curY))
			this->tiltes[this->curX - 1, this->curY]->SetState(TILTE_STATE_CHOISE);

		if (this->canMoove(this->curX, this->curY + 1))
			this->tiltes[this->curX, this->curY + 1]->SetState(TILTE_STATE_CHOISE);

		if (this->canMoove(this->curX, this->curY - 1))
			this->tiltes[this->curX, this->curY - 1]->SetState(TILTE_STATE_CHOISE);
	}
	else
	{
		switch (this->selectedDirection)
		{
		case GAME_PLAY_DIR_UP:
			if (this->canMoove(this->curX, this->curY - 1))
			{
				this->curY--;				
				this->tiltes[this->curX, this->curY]->SetState(TILTE_STATE_VALIDATED);
			}
			else
			{
				this->selectedDirection = GAME_PLAY_DIR_NONE;
			}
		}
	}

}


bool GamePlay::canMoove(uint8_t x, uint8_t y)
{
	if (x >= this->width)
		return false;
	if (y >= this->height)
		return false;
	return this->tiltes[x][y].GetState() == TILTE_STATE_EMPTY;
}

bool GamePlay::canSelect(uint8_t x, uint8_t y)
{
	if (x >= this->width)
		return false;
	if (y >= this->height)
		return false;
	return this->tiltes[x][y].GetState() == TILTE_STATE_CHOISE;
}

void GamePlay::eraseChoose(void)
{
	if (this->canSelect(this->curX-1, this->curY))
		this->tiltes[this->curX-1][this->curY].SetState(TILTE_STATE_EMPTY);

	if (this->canSelect(this->curX+1, this->curY))
		this->tiltes[this->curX+1][this->curY].SetState(TILTE_STATE_EMPTY);

	if (this->canSelect(this->curX, this->curY-1))
		this->tiltes[this->curX][this->curY-1].SetState(TILTE_STATE_EMPTY);

	if (this->canSelect(this->curX, this->curY+1))
		this->tiltes[this->curX][this->curY+1].SetState(TILTE_STATE_EMPTY);
	
}


GamePlay gamePlay;

