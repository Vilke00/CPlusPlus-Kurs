#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mIsRunning(true)
,mRenderer(nullptr)
,mTicksCount(0)
,mPaddleDir(0)
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
	
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f/2.0f;
	mBallPos.x = 1024.0f/2.0f;
	mBallPos.y = 768.0f/2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
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

	
	//Ako su pritisnuti u isto vreme, zaustavlja ga jer samo dodaje vrednosti
	mPaddleDir = 0;
	if(state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if(state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
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

	//Updateuje sa paddle brzine 300px po sekundi
	if(mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		//Granice ekrana
		if(mPaddlePos.y < (paddleH/2.0f + thickness))
		{
			mPaddlePos.y = paddleH/2.0f + thickness;
		}
		else if(mPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if(
		//Da li je razlika dovoljno mala izmedju lopte i igraca
		diff <= paddleH / 2.0f &&
		//Lopta je na tacnoj x poziciji
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		//Lopta se pomera levo
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	//Desni zid
	else if(mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	//Da li je udarilo gornji u zid i da li se pomera prema zidu
	if(mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	//Donji zid
	else if(mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
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

	//Boja pravougaonika
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	//Gornji zid
	SDL_Rect wall{
	0,
	0,
	1024,
	thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);

	//Donji zid
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	//Desni
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2),//Delim sa 2 da bih konvertovao x i y sa center pointa u top left
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	//Zameni buffere
	SDL_RenderPresent(mRenderer);
}

