#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "state.h"
#include "info.h"
#include "observer.h"

//class Observer;

class Subject {
	State s;
	std::vector<Observer *> observers;
	protected:
	void setState(State news);
	public:
	void attach(Observer *o);
	void notifyObservers();
	virtual Info getInfo() const = 0;
	State getState() const;
};

#endif
