#pragma once
#include <vector>

//Game Object model koji cemo koristiti je hibridni izmedju monoliticnog(Osoba -> Radnik -> Specifican radnik...)
//i Komponent modela(Unity fazon, svaka klasa ima svoje komponente samo)
//Base klasa(Actor) ce imati par virtualnih funkcija, ali svaki actor ce imati vektor komponenti
class Actor
{
public:
	//Koristi se da se prati stanje glumca. Na primer Actor se update-uje samo dok je aktivan
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	//Konstruktor/Destruktor.
	//Treba mu pristup Game klasi zbog vise razlog, jedan od kojih je da kreira druge actore
	//Zbog toga je ubacen denepdency injection ovde da bi mogao da korisit pokazivac za pravljenje vise actor-a ili pristup drugim game funkcijama
	Actor(class Game* game);
	virtual ~Actor();

	//Update f-ja koja se zove iz Game
	void Update(float deltaTime);
	//Loop update svih komponenti zakacenih za Actor-a
	void UpdateComponents(float deltaTime);
	//Update specifican za Actor-a
	virtual void UpdateActor(float deltaTime);

	//Geteri/seteri
	const Vector2& GetPosition() const {return mPosition;}
	void SetPosition(const Vector2& pos){mPosition = pos;}
	float GetScale() const {return mScale;}
	void SetScale(float scale) { mScale = scale;}
	float GetRotation() const {return mRotation;}
	void SetRotation(float rotation){mRotation = rotation;}

	State GetState() const {return mState;}
	void SetState(State state){mState = state;}

	//Dodaj ili unisti komponentu
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	//Actor state
	State mState;
	//Transform
	Vector2 mPosition; //Centrira poziciju Actor-a
	float mScale; //Uniform scale actor-a (1.0f za 100%)
	float mRotation; //Ugao rotacije u radijansu
	//Komponente koje drzi ovaj aktor
	std::vector<class Component*> mComponents;
	class Game* mGame;
};
