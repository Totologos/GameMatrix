// GamePlay.h

#ifndef _GamePlay_h
#define _GamePlay_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include "tilte.h"
#else
	#include "..\GameMatrixEmulate\pch.h"
	#include "..\GameMatrixEmulate\tilte_emulate.h"
#endif



typedef enum tKeys
{
	KEY_NONE			= 0x0,
	KEY_ARROW_UP		= 0x1,
	KEY_ARROW_DOWN		= 0x2,
	KEY_ARROW_LEFT		= 0x4,
	KEY_ARROW_RIGHT		= 0x8,
	KEY_BUTTON_A		= 0x10,
	KEY_BUTTON_B		= 0x20
} tKeys;

typedef enum tGamePlayDir
{
	GAME_PLAY_DIR_NONE = 0,
	GAME_PLAY_DIR_UP = 1,
	GAME_PLAY_DIR_DOWN = 2,
	GAME_PLAY_DIR_LEFT = 4,
	GAME_PLAY_DIR_RIGHT = 8
} tGamePlayDir;

typedef enum tGameSequences
{
	GAME_SEQUENCE_START_POINT,
	GAME_SEQUENCE_MOOVED,
	GAME_SEQUENCE_GAME_OVER,
	GAME_SEQUENCE_WIN,
	GAME_SEQUENCE_RESTART,
	GAME_SEQUENCE_NEXT_LEVEL,
} tGameSequences;



class GamePlay
{
 private:
	 Tilte				**tiltes;	 
	 uint8_t			width;
	 uint8_t			height;
	 uint8_t			curX;
	 uint8_t			curY;
	 tKeys				lastKey;
	 tGameSequences		gameSequence;
	 tGamePlayDir		availableDirection;
	 tGamePlayDir		selectedDirection;
	 bool				onlyOneDir;
	 uint8_t			*levels;
	 uint8_t			levelId;
	 uint8_t			numOfLevels;	 

	 void				resetGame(void);
	 bool				canMoove(uint8_t x, uint8_t y);
	 bool				canSelect(uint8_t x, uint8_t y);
	 void				update(void);
	 void				eraseChoose(void);

 public:
	 void				init(Tilte **tiltes, uint8_t width, uint8_t height, uint8_t* levels, uint8_t numOfLevels);
	 void				loadGame(uint8_t index);
	 void				loadGame(void);
	 void				keyEvent(tKeys key);	
};

extern GamePlay gamePlay;

#endif

