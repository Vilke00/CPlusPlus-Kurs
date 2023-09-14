#pragma once
#include <vector>

#include "SDL/SDL.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	
	SDL_Window* mWindow;
	bool mIsRunning;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;

	//Pending actors nam treba za dodavanje novih actora dok iteracija prolazi kroz mActors
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	bool mUpdatingActors;
};
