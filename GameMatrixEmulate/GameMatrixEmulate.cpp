// GameMatrix.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "pch.h"
#include "tilte_emulate.h"
#include "..\GameMatrixArduino\GamePlay.h"

#define GAME_WIDTH		7
#define GAME_HEIGHT		7
#define NUM_LEDS		(GAME_WIDTH * GAME_HEIGHT)

Tilte tiltes[GAME_WIDTH][GAME_HEIGHT];

uint8_t level1[] =
{
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
};

void setup()
{
	for (uint8_t y = 0; y < 7; y++)
	{
		for (uint8_t x = 0; x < 7; x++)
		{
			tiltes[x][y] = Tilte(x, y);
		}
	}

	gamePlay.init((Tilte**)tiltes, GAME_WIDTH, GAME_HEIGHT);

	gamePlay.loadGame(level1);
}

void loop()
{
	for (uint8_t i = 0; i < GAME_HEIGHT; i++)
	{
		for (uint8_t j = 0; j < GAME_WIDTH; i++)
		{
			std::cout << tiltes[i][j].Update(5);
		}
		std::cout << '\n';
	}
}

int main()
{
    std::cout << "Setup()\n"; 
	setup();
	std::cout << "loop()\n";
	for (int i = 0; i < 1; i++)
	{
		loop();
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
