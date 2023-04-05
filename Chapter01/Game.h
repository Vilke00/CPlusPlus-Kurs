#pragma once
#include <vector>

#include "SDL/SDL.h"

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 position;
	Vector2 velocity;
};

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
	//Sistem za crtanje grafike
	SDL_Renderer* mRenderer;

	Vector2 mPaddleOnePos;
	//Pomeranje igraca. 0 ne pomera se, -1 ide gore, 1 ide dole
	int mPaddleOneDir;
	Vector2 mPaddleTwoPos;
	int mPaddleTwoDir;

	//Vector2 mBallPos;
	//Brzina i pravac lopte
	//Vector2 mBallVel;
	std::vector<Ball> mBallList;

	//Za delta time
	Uint32 mTicksCount;
};
