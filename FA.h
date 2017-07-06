#ifndef FA_H
#define FA_H

#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

// Could be NFA or DFA depending on states + transition functions.
class FA
{
private:
	vector<char> alphabet;
	vector<vector<string>> stateNames;
	// vector<vector<int>> nextState;

	vector<vector<string>> validTransition;

	// [0] = initial; [1] = final
	vector<vector<bool>> isInitialFinal;

public:
	// Default constructor
	FA()
	{
		alphabet = vector<char>();
	}

	// Constructor with a purpose.
	FA(vector<char> Alphabet, vector<string> StateNames, vector<vector<char>> ValidTransitions, vector<vector<int>> NextStateIndeces, vector<vector<int>> IsInitialFinal)
	{
		// Initialize alphabet.  Note that alphabet implicitly contains lambda ('_').
		alphabet = Alphabet;

		stateNames = vector<vector<string>>(StateNames.size());
		validTransition = vector<vector<string>>(StateNames.size());
		isInitialFinal = vector<vector<bool>>(StateNames.size());
		
		for (int i = 0; i < stateNames.size(); i++)
		{
			stateNames[i] = vector<string>();
			stateNames[i].push_back(StateNames[i]);

			validTransition[i] = vector<string>(StateNames.size());
			isInitialFinal[i] = vector<bool>(2);

			// Is initial?
			isInitialFinal[i][0] = (IsInitialFinal[i][0] == 0 ? false : true);
			// Is final?
			isInitialFinal[i][1] = (IsInitialFinal[i][1] == 0 ? false : true);

			for (int j = 0; j < ValidTransitions[i].size(); j++)
			{
				validTransition[i][NextStateIndeces[i][j]] += ValidTransitions[i][j];
			}
		}

		int blub = 0;
	}
	
	FA ConvertToDFA()
	{
		// return new FA that has been converted to DFA.
		return FA();
	}
};

#endif