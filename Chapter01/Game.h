#pragma once
#include "SDL/SDL.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	//Pomocne funkcije
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//Promeniljive klase ili strukture pocinju sa m da bi se lakse prepoznale

	//Pravljenje SDL prozora. Pokazivac je da bi smo dinamicki upravljali njime
	SDL_Window* mWindow;
	//Da li igra treba da se nastavi
	bool mIsRunning;
};
