#include "Game.h"

Game::Game()
:mWindow(nullptr)
,mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	//Inicijalizacija video biblioteke. Vraca int
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	//Takodje moze se inicijalizovati i AUDIO, HAPTIC(Force feedback sistem) i GAMECONTROLLER(Podrska za kontroler)

	if(sdlResult !=0)
	{
		//SDL_Log je printf u sustini
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Kreiranje prozora
	mWindow = SDL_CreateWindow(
	"Game Programming in C++ (Chapter 1)", //Window title
	100, //Top-left x koord
	100, //Top-left y koord
	1024, //Sirina
	768, //Duzina
	0 //Flag(0 znaci nema flag)
	);

	//Flags moze biti SDL_WINDOW_FULLSCREEN, _FULLSCREEN_DESKTOP(Koristi desktop rezoluciju), _OPENGL(Support za opengl), _RESIZABLE(Korisnik moze da menja velicinu)

	if(!mWindow)//Ako je greska bice nullptr
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while(mIsRunning)
	{
		//3 koraka iz kojih se sastoji svaki frame
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	
}

void Game::UpdateGame()
{
	
}

void Game::GenerateOutput()
{
	
}

