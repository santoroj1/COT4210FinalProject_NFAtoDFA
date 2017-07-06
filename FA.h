#ifndef FA_H
#define FA_H

#include <string>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

// Could be NFA or DFA depending on states + transition functions.
class FA
{
private:
	vector<char> alphabet;
	vector<vector<string>> stateNames;
	vector<vector<string>> validTransition;

	// [0] = initial; [1] = final
	vector<vector<bool>> isInitialFinal;

	int startStateIndex;

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

			if (isInitialFinal[i][0])
				startStateIndex = i;

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

	vector<int> GetNextStateIndeces(int CurrentState, char InputChar)
	{
		vector<int> nextStates = vector<int>();

		for (int i = 0; i < validTransition[CurrentState].size(); i++)
		{
			for (int j = 0; j < validTransition[CurrentState][i].size(); j++)
			{
				if (validTransition[CurrentState][i][j] == InputChar)  // We've found a transition.
				{
					nextStates.push_back(i);
				}
			}
		}

		return nextStates;
	}

	vector<int> GetNextStateIndeces(int CurrentState)
	{
		vector<int> nextStates = vector<int>();

		for (int i = 0; i < validTransition[CurrentState].size(); i++)
		{
			if (validTransition[CurrentState][i] != "")  // We've found a transition.
			{
				nextStates.push_back(i);
			}
		}

		return nextStates;
	}

	void BFSPrint()
	{
		queue<int> BFSQ = queue<int>();  // Initialize queue or stack
		queue<int> levelQ = queue<int>();  // Controls number of tabs for formatting.

		vector<bool> visited = vector<bool>(stateNames.size());

		BFSQ.push(startStateIndex);  // Push/enqueue root node
		levelQ.push(0);

		while (BFSQ.size() > 0)  // As long as there’s still an element do...
		{
			int curNodeBFS = BFSQ.front();  // Dequeue/pop element
			int curLevel = levelQ.front();

			BFSQ.pop();
			levelQ.pop();

			// Read/process element here.
			for (int i = 0; i < curLevel; i++)
				cout << "\t";

			cout << stateNames[curNodeBFS][0] << endl;
			visited[curNodeBFS] = true;

			vector<int> nextStateIndeces = GetNextStateIndeces(curNodeBFS);

			for (int i = 0; i < nextStateIndeces.size(); i++)
			{
				if (!visited[nextStateIndeces[i]])
				{
					BFSQ.push(nextStateIndeces[i]);  // Enqueue/push child elements
					levelQ.push(curLevel + 1);
				}
			}
		}
	}

	void PrintNFA()
	{
		cout << "State\t";

		// Print column for each alphabet character and lambda.
		for (int i = 0; i < alphabet.size() + 1; i++)
		{
			if (i < alphabet.size())
				cout << alphabet[i] << "\t";
			else
				cout << "_";
		}

		cout << endl;

		for (int i = 0; i < validTransition.size(); i++)
		{
			cout << stateNames[i][0] << "\t";

			for (int j = 0; j < alphabet.size() + 1; j++)
			{
				vector<string> nextStateNames = vector<string>();

				if (j < alphabet.size())
				{
					vector<int> nextIndeces = GetNextStateIndeces(i, alphabet[j]);

					for (int k = 0; k < nextIndeces.size(); k++)
						nextStateNames.push_back(stateNames[nextIndeces[k]][0]);
				}
				else
				{
					vector<int> nextIndeces = GetNextStateIndeces(i, '_');

					for (int k = 0; k < nextIndeces.size(); k++)
						nextStateNames.push_back(stateNames[nextIndeces[k]][0]);
				}

				for (int k = 0; k < nextStateNames.size(); k++)
				{
					cout << nextStateNames[k] << (k < nextStateNames.size() - 1 ? ", " : "");
				}

				cout << "\t";
			}

			cout << endl;
		}
	}
};

#endif