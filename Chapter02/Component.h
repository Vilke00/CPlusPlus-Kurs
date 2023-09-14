#pragma once

class Component
{
public:
	//Konstruktor
	//Sto je manji update order, to ce pre da se update
	//Ponovo postoji pokazivac u konstruktoru. To je da bi komponenta mogla da pristupi transform podacima ili drugim potrebnim infomacijama
	Component(class Actor* owner, int updateOrder = 100);
	//Destruktor
	virtual ~Component();
	//Update po delta time
	virtual void Update(float deltaTime);
	int GetUpdateOrder() const {return mUpdateOrder;}
protected:
	//Actor koji poseduje
	class Actor* mOwner;
	//Update order komponenti
	int mUpdateOrder;
};
