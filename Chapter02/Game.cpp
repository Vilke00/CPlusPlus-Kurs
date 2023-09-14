#include "Game.h"

#include "Actor.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mIsRunning(true)
,mRenderer(nullptr)
,mUpdatingActors(false)
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

	//Kreiranje renderera
	mRenderer = SDL_CreateRenderer(
	mWindow, //Prozor za koji se pravi render
	-1,		 //Obicno -1. Nije samo ako igra ima vise prozora
	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //Ovi flagovi su za koriscenje grafickog drivera i vsync. Ovo su jedini flagovi sa CreateRenderer
	);

	if(!mRenderer)
	{
		SDL_Log("Failed to initialize the renderer: %s", SDL_GetError());
		return false;
	}

	
	
	return true;
}

void Game::Shutdown()
{
	//Moramo unisititi sve stvorene objekte
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
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
	SDL_Event event;
	//Loopujemo kroz event queue i proveravamo svaki frame da li postoji neki event
	while(SDL_PollEvent(&event))//SDL_Poll uzima event preko pointera. To cuva informacije o eventu koji je upravo sklonjen iz queue
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//Uzimamo stanje tastature. Vraca pokazivac na niz koji sadrzi stanje(Recimo stanje je pritisnuo ESC)
	const Uint8* state = SDL_GetKeyboardState(NULL);

	//End loop if escape
	if(state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	
	
}

void Game::UpdateGame()
{
	//Ogranicavam fps u sustini na 60
	while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	//Delta time je razlika u tikovima od poslednjeg frame-a konvertovana u sekunde
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//Update tick counts for next frame

	//Ogranicavam da se igra ne fast forwarduje kada se odpauzira
	if(deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mUpdatingActors = true;
	for(auto actor:mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	//Pomera iz pending u actors
	for(auto pending:mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//Add dead actors to temp
	std::vector<Actor*> deadActors;
	for(auto actor : mActors)
	{
		if(actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	//Delete dead actors -> moves them from mActors
	for(auto actor : deadActors)
	{
		delete actor;
	}
	
}

void Game::GenerateOutput()
{
	//Grafika je output
	//Sadrzi 3 koraka: Nacrtamo u back bufferu boju(trenutni buffer)
	//Nacrtamo celu scenu
	//Zamenimo prednji i zadnji buffer

	//Odabir pozadinske boje
	SDL_SetRenderDrawColor(
		mRenderer,
		30,		//R
		218,	//G
		149,	//B
		255		//A
	);

	//Brise back buffer i postavlja se na pozadinu u trenutnu boju iscrtavanja
	SDL_RenderClear(mRenderer);

	//Nacrtaj igru

	

	//Zameni buffere
	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor)
{
	//Ako se actor-i updateuju, dodaje se u pending
	if(mUpdatingActors)
	{
		//Dodaje na kraj vektora. Efikasniji od push_back jer ne pravi kopiju i ne pomera nego direktno konstruise objekat
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	//Todo
}



