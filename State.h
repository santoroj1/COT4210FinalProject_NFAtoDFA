#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Contains set of transition functions
class State
{
private:
	// Contains reference of next state and list of valid transitions.  Use empty string for lambda-transition.
	struct TransitionFunction
	{
		int nextState;
		string validTransition;
	};

	vector<TransitionFunction> transitions;
	string name;

public:
	State(string& Name)
	{
		name = Name;
		transitions = vector<TransitionFunction>();
	}

	vector<TransitionFunction> GetTransitions() const
	{
		return transitions;
	}

	// Print name + transitions for each alphabet symbol
	string OutString(vector<string> alphabet)
	{
		return "";
	}
};

#endif // !STATE_H