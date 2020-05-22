// 
// 
// 

#include "GamePlay.h"

void GamePlay::loadGame(uint8_t levelId)
{
	if (levelId < this->numOfLevels)
	{
		this->levelId = levelId;
	}
	loadGame();
}

void GamePlay::loadGame(void)
{
	size_t offset = this->levelId * this->height * this->width;
	for (size_t y = 0; y < this->height; y++)
	{
		for (size_t x = 0; x < this->width; x++)
		{
			this->tiltes[x][y].SetState(this->levels[offset + y * (size_t)this->width + x] == 1 ? TILTE_STATE_LOCK : TILTE_STATE_EMPTY);
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
	this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_CHOISE);
}

void  GamePlay::keyEvent(tKeys key)
{
	if (this->lastKey != key && key != KEY_NONE)
	{
		switch (this->gameSequence)
		{
		case GAME_SEQUENCE_START_POINT:
		{
			switch (key)
			{
			case KEY_ARROW_UP:
			case KEY_ARROW_DOWN:
			case KEY_ARROW_LEFT:
			case KEY_ARROW_RIGHT:
				this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_EMPTY);
				do
				{
					switch (key)
					{
					case KEY_ARROW_UP:		this->curY = (this->curY - 1 + this->height) % this->height;	break;
					case KEY_ARROW_DOWN:	this->curY = (this->curY + 1 + this->height) % this->height;	break;
					case KEY_ARROW_LEFT:	this->curX = (this->curX - 1 + this->width) % this->width;		break;
					case KEY_ARROW_RIGHT:	this->curX = (this->curX + 1 + this->width) % this->width;		break;
					}
				} while (this->tiltes[this->curX][this->curY].GetState() != TILTE_STATE_EMPTY);
				this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_CHOISE);
				break;

			case KEY_BUTTON_A:
				this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_START);
				this->gameSequence = GAME_SEQUENCE_MOOVED;
				this->selectedDirection = GAME_PLAY_DIR_NONE;
				this->update();
				break;
			case KEY_BUTTON_B:
				loadGame(this->levelId < this->numOfLevels - 1 ? this->levelId + 1 : 0);
				break;

			}
			break;
		}
		case GAME_SEQUENCE_MOOVED:
		{
			switch (key)
			{
			case KEY_ARROW_UP:
			case KEY_ARROW_DOWN:
			case KEY_ARROW_LEFT:
			case KEY_ARROW_RIGHT:
				if (canSelect(this->curX, this->curY - 1) && (key == KEY_ARROW_UP || this->onlyOneDir))
				{
					eraseChoose();
					this->curY--;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
					this->selectedDirection = GAME_PLAY_DIR_UP;
					this->update();
				}
				else if (canSelect(this->curX, this->curY + 1) && (key == KEY_ARROW_DOWN || this->onlyOneDir))
				{
					eraseChoose();
					this->curY++;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
					this->selectedDirection = GAME_PLAY_DIR_DOWN;
					this->update();
				}				
				else if (canSelect(this->curX - 1, this->curY) && (key == KEY_ARROW_LEFT || this->onlyOneDir))
				{
					eraseChoose();
					this->curX--;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
					this->selectedDirection = GAME_PLAY_DIR_LEFT;
					this->update();
				}
				else if (canSelect(this->curX + 1, this->curY) && (key == KEY_ARROW_RIGHT || this->onlyOneDir))
				{
					eraseChoose();
					this->curX++;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
					this->selectedDirection = GAME_PLAY_DIR_RIGHT;
					this->update();
				}
				break;
			case KEY_BUTTON_A:
			case KEY_BUTTON_B:
				this->resetGame();
				break;
			}
			break;
		}
		case GAME_SEQUENCE_RESTART:
			if (key == KEY_BUTTON_A)
			{
				this->resetGame();
			}
			break;

		default :
			break;
		}
	}
	else
	{
		EVERY_N_MILLISECONDS(20)
		{
			this->update();
		}
	}
	this->lastKey = key;
}

void  GamePlay::update(void)
{
	switch (this->gameSequence)
	{
	case GAME_SEQUENCE_MOOVED:
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
			this->gameSequence = GAME_SEQUENCE_WIN;
			for (uint8_t y = 0; y < this->height; y++)
			{
				for (uint8_t x = 0; x < this->width; x++)
				{
					if (this->tiltes[x][y].GetState() == TILTE_STATE_EMPTY)
					{
						this->gameSequence = GAME_SEQUENCE_GAME_OVER;
					}
				}
			}
			this->curX = 0;
			this->curY = 0;
		}
		else if (this->selectedDirection == GAME_PLAY_DIR_NONE)
		{
			onlyOneDir = numOfChoise == 1;
			// wait user press à key
			if (this->canMoove(this->curX + 1, this->curY))
				this->tiltes[this->curX + 1][this->curY].SetState(TILTE_STATE_CHOISE);

			if (this->canMoove(this->curX - 1, this->curY))
				this->tiltes[this->curX - 1][this->curY].SetState(TILTE_STATE_CHOISE);

			if (this->canMoove(this->curX, this->curY + 1))
				this->tiltes[this->curX][this->curY + 1].SetState(TILTE_STATE_CHOISE);

			if (this->canMoove(this->curX, this->curY - 1))
				this->tiltes[this->curX][this->curY - 1].SetState(TILTE_STATE_CHOISE);
		}
		else
		{
			switch (this->selectedDirection)
			{
			case GAME_PLAY_DIR_UP:
				if (this->canMoove(this->curX, this->curY - 1))
				{
					this->curY--;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
				}
				else
				{
					this->selectedDirection = GAME_PLAY_DIR_NONE;
				}
				break;
			case GAME_PLAY_DIR_DOWN:
				if (this->canMoove(this->curX, this->curY + 1))
				{
					this->curY++;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
				}
				else
				{
					this->selectedDirection = GAME_PLAY_DIR_NONE;
				}
				break;
			case GAME_PLAY_DIR_LEFT:
				if (this->canMoove(this->curX - 1, this->curY))
				{
					this->curX--;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
				}
				else
				{
					this->selectedDirection = GAME_PLAY_DIR_NONE;
				}
				break;
			case GAME_PLAY_DIR_RIGHT:
				if (this->canMoove(this->curX + 1, this->curY))
				{
					this->curX++;
					this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_VALIDATED);
				}
				else
				{
					this->selectedDirection = GAME_PLAY_DIR_NONE;
				}
				break;
			}
		}
		break;
	}

	case GAME_SEQUENCE_WIN:
	case GAME_SEQUENCE_GAME_OVER:

		if (this->gameSequence == GAME_SEQUENCE_GAME_OVER && this->tiltes[curX][curY].GetState() != TILTE_STATE_LOCK)
		{
			this->tiltes[curX][curY].SetState(TILTE_STATE_LOOSE);
		}
		else if (this->gameSequence == GAME_SEQUENCE_WIN && this->tiltes[curX][curY].GetState() != TILTE_STATE_LOCK)
		{
			this->tiltes[curX][curY].SetState(TILTE_STATE_START);
		}

		if (this->curX < this->width -1)
		{
			this->curX++;
		}
		else if (this->curY < this->height -1)
		{
			this->curX = 0;
			this->curY++;
		}
		else
		{
			this->gameSequence = GAME_SEQUENCE_RESTART;
		}

		

		break;

	default:
		break;
	}

}


bool GamePlay::canMoove(uint8_t x, uint8_t y)
{
	if (x >= this->width)
		return false;
	if (y >= this->height)
		return false;
	return this->tiltes[x][y].GetState() == TILTE_STATE_EMPTY || this->tiltes[x][y].GetState() == TILTE_STATE_CHOISE;
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

