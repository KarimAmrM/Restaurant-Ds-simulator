#pragma once

#include "..\Defs.h"
#include "Order.h"
#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int NumberOfDishes; //number of dishes before break
	int BreakDuration;  //time steps of the break;    
	bool Free; //flag to check either the cook is free or not
	Order* CurrentOrder;//the assigned order to the cook
	bool Injured ;  //flag to check either the cook is injured or not
	int ordersCompleted;
	int excpetedReturn;
	int restPeriod;

public:
	Cook(int id, ORD_TYPE r_Type, int s, int n, int bd,int rest);
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	int GetSpeed() const;
	int GetNumberOfDishes() const;
	int GetBreakDuration() const;
	bool isFree() const; //a getter for bool free
	bool isInjured() const;//a getter for bool Injured;
	Order* GetCurrentOrder();
	void setID(int);
	void setType(ORD_TYPE);
	void setSpeed(int s);
	void setNumberOfDishes(int n);
	void setBreakDuration(int bd);
	void setisfree(bool isFree);
	void setisinjured(bool isInjured);
	void AssignOrder(Order* o, int Stime);//a function to assign an order to the cook
	bool toBreak (int timestep);
	bool toRest(int timestep);
	
	void removeOrder();//frees the cook of his current oreder after it completion
	bool returnToAvail(int timeStep);
	

};
