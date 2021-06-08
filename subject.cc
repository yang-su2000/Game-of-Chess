#include "subject.h"

void Subject::attach(Observer *o) {
        observers.emplace_back(o);
}

void Subject::notifyObservers() {
        for (auto &ob : observers) ob->notify(*this);
}

State Subject::getState() const { return s; }

void Subject::setState(State news) { s = news; }
