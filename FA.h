#ifndef FA_H
#define FA_H

#include "State.h"
#include <map>
#include <fstream>

using namespace std;

// Could be NFA or DFA depending on states + transition functions.
class FA
{
private:
	vector<string> alphabet;
	vector<State> state;

public:
	FA()
	{
		// Reads in a file and constructs NFA.

		// Current state:
		// state[i]
		// Next states:
		// state[state[i].GetTransitions()[j].nextState]

		alphabet = vector<string>();
		state = vector<State>();
	}
};

#endif