// GameMatrix.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "pch.h"
#include "tilte_emulate.h"
#include "..\GameMatrixArduino\GamePlay.h"

#define GAME_WIDTH		7
#define GAME_HEIGHT		7
#define NUM_LEDS		(GAME_WIDTH * GAME_HEIGHT)

Tilte **tiltes;

uint8_t level1[] =
{
	1, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
};

void setup()
{
	tiltes = new Tilte*[GAME_HEIGHT];
	for (uint8_t i = 0; i < GAME_WIDTH; i++)
	{
		tiltes[i] = new Tilte[GAME_WIDTH];
	}		

	for (uint8_t y = 0; y < GAME_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < GAME_WIDTH; x++)
		{
			tiltes[x][y].init(x, y);
		}
	}

	gamePlay.init(tiltes, GAME_WIDTH, GAME_HEIGHT);

	gamePlay.loadGame(level1);
}

void displayArray()
{
	
	// display array
	for (uint8_t y = 0; y < GAME_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < GAME_WIDTH; x++)
		{
			std::cout << tiltes[x][y].Update(5) << ' ';
		}
		std::cout << '\n';
	}
}

int main()
{
	char key = ' ';
	std::cout << "Setup()\n"; 
	setup();
	std::cout << "loop()\n";
	while( key != 'q')
	{
		switch (key)
		{
		case '8':
			gamePlay.keyEvent(KEY_ARROW_UP);
			break;
		case '2':
			gamePlay.keyEvent(KEY_ARROW_DOWN);
			break;
		case '4':
			gamePlay.keyEvent(KEY_ARROW_LEFT);
			break;
		case '6':
			gamePlay.keyEvent(KEY_ARROW_RIGHT);
			break;
		case '1':
			gamePlay.keyEvent(KEY_BUTTON_A);
			break;		
		case '3':
				gamePlay.keyEvent(KEY_BUTTON_B);
				break;
		default:
			break;
		}
		
		for(int i =0; i <50; i++)
			gamePlay.keyEvent(KEY_NONE);
		displayArray();
		std::cin >> key;
	}
	std::cout << "end\n";

}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
