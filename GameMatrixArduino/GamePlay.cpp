// 
// 
// 

#include "GamePlay.h"

void GamePlay::init(Tilte **tiltes, uint8_t width, uint8_t height, uint8_t* levels, uint8_t numOfLevels)
{
	this->tiltes = tiltes;
	this->width = width;
	this->height = height;
	this->numOfLevels = numOfLevels;
	this->levels = levels;
}

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

/* reset the game: init the game board and define the start point.
 * if the start point has already define in the previous game, used it
 * if not use the first empty tilte */
void GamePlay::resetGame()
{
	bool foundStart = false;
	this->curX = 0;
	this->curY = 0;

	bool foundDefaultStart = false;
	uint8_t tmpX = 0;
	uint8_t tmpY = 0;

	this->gameSequence = GAME_SEQUENCE_START_POINT;

	for (uint8_t y = 0; y < this->height; y++)
	{
		for (uint8_t x = 0; x < this->width; x++)
		{
			if (this->tiltes[x][y].GetState() != TILTE_STATE_LOCK)
			{
				if (!foundStart && this->tiltes[x][y].GetState() == TILTE_STATE_START)
				{
					// a previous start point has founded, used it!
					this->curX = x;
					this->curY = y;
					foundStart = true;
				}

				this->tiltes[x][y].SetState(TILTE_STATE_EMPTY);
				if (!foundDefaultStart)
				{
					// first empty tilte
					foundDefaultStart = true;
					tmpX = x;
					tmpY = y;
				}
			}
		}
	}

	if (!foundStart)
	{
		this->curX = tmpX;
		this->curY = tmpY;
	}
	this->tiltes[this->curX][this->curY].SetState(TILTE_STATE_CHOISE); // Init start point
}

/* must be call as soon as possible. (cyclic function) 
 * arg1 must be contain the current key pressed or no key */
void  GamePlay::keyEvent(tKeys key)
{
	if (this->lastKey != key && key != KEY_NONE)
	{
		// a new key is pressed, it is a new event...
		switch (this->gameSequence)
		{
		case GAME_SEQUENCE_START_POINT:
		{
			/* move the start point in function of the arrow event
			 * button A, start a game (current position is the start tilte)
			 * button B, load the next level */
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
			/* a new key is pressed during the game */
			switch (key)
			{
			case KEY_ARROW_UP:
			case KEY_ARROW_DOWN:
			case KEY_ARROW_LEFT:
			case KEY_ARROW_RIGHT:
				/* if there is only one direction available any arrow key to the movement *
				 * if not, move in expected direction */
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
		/* no keybaod event, update the game every 20ms*/
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
		// define if it is possible to update the position.
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
			// it is not possible de move: game over or win !
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
			// it is possible to move and there is no previsous selected direction.
			// the user must de defined a direction by pressed an arrow key...
			// so wait user press à key, and display the availble direction
			onlyOneDir = numOfChoise == 1;
			
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
			// it is possible to move and an direction is already defined, so move
			// to the next tilte.
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
		// end of game...
		// all tiltes are but in same colors...
		// and the end, wait the user press a key (to restart the game)
		if (this->gameSequence == GAME_SEQUENCE_GAME_OVER 
			&& this->tiltes[curX][curY].GetState() != TILTE_STATE_LOCK 
			&& this->tiltes[curX][curY].GetState() != TILTE_STATE_START )
		{
			this->tiltes[curX][curY].SetState(TILTE_STATE_LOOSE);
		}
		else if (this->gameSequence == GAME_SEQUENCE_WIN 
			&& 	this->tiltes[curX][curY].GetState() != TILTE_STATE_LOCK
			&& this->tiltes[curX][curY].GetState() != TILTE_STATE_START )
		{
			this->tiltes[curX][curY].SetState(TILTE_STATE_WIN);
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

