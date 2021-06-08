#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;

class Observer {
	public:
		virtual bool notify(Subject &whoFrom) = 0;
		virtual ~Observer() = default;
};

#endif
