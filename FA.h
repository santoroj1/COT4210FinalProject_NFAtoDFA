#ifndef FA_H
#define FA_H

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <sstream>
#include <iomanip>

using namespace std;

// Could be NFA or DFA depending on states + transition functions.
class FA
{
private:
	// Parameters -------------------------------------------------------------------------------------------
	vector<char> alphabet;
	vector<vector<string>> stateNames;

	// Reference vector by transition[startStateIndex][endStateIndex].
	vector<vector<string>> validTransition;

	// [0] = initial; [1] = final
	vector<vector<bool>> isInitialFinal;

	// Use -1 to indicate that the FA has no starting state.
	int startStateIndex;
	bool isDFA;
	// End Parameters ---------------------------------------------------------------------------------------

	// Helper Methods ---------------------------------------------------------------------------------------

	// Spacing for formatting
	string GetSpaces(string InString, int FieldWidth)
	{
		string spaces = "";
		int numSpaces = FieldWidth - InString.size();
		
		for (int i = 0; i < numSpaces; i++)
			spaces += " ";

		return spaces;
	}

	// Spacing for formatting.  Handles character input.
	string GetSpaces(int FieldWidth)
	{
		string spaces = "";
		int numSpaces = FieldWidth - 1;

		for (int i = 0; i < numSpaces; i++)
			spaces += " ";

		return spaces;
	}

	// Puts multiple sets of state names together based on state indeces and then unifies them.
	vector<string> UnionStateNames(vector<int> StateIndeces)
	{
		vector<string> newStateNames = vector<string>();

		// Simply loop over all new states and then add these to newStateNames.
		for (int i = 0; i < StateIndeces.size(); i++)
		{
			for (int j = 0; j < stateNames[StateIndeces[i]].size(); j++)
			{
				newStateNames.push_back(stateNames[StateIndeces[i]][j]);
			}
		}

		// Remove duplicate elements.
		vector<string> seenElements = vector<string>();

		for (int i = 0; i < newStateNames.size(); i++)
		{
			bool wasElementFound = false;

			for (int j = 0; j < seenElements.size(); j++)
			{
				if (newStateNames[i] == seenElements[j])
				{
					wasElementFound = true;
					break;
				}
			}

			if (wasElementFound)
			{
				newStateNames.erase(newStateNames.begin() + i);
				i--;
			}
		}

		return newStateNames;
	}

	// Reorders state names in vector.  This ensures that name-checking can be done quickly.
	void ReorderNames(vector<string>& OriginalNameSet)
	{
		for (int i = 0; i < OriginalNameSet.size(); i++)  // Simple sorting...
		{
			for (int j = 1; j < OriginalNameSet.size(); j++)
			{
				// Comparison:  Check to see if one string is greater than the other.
				if (OriginalNameSet[i].compare(OriginalNameSet[j]) > 0)
				{
					// If so, swap as [i] must come after [j].
					string temp = OriginalNameSet[i];
					OriginalNameSet[i] = OriginalNameSet[j];
					OriginalNameSet[j] = temp;
				}
			}
		}
	}

	// Gets string for all of the names in 'stateNames' without set notation.
	string GetStateNameNFA(int StateIndex)
	{
		string s = "|";

		for (int i = 0; i < stateNames[StateIndex].size(); i++)
		{
			s += stateNames[StateIndex][i];
			if (i < stateNames[StateIndex].size() - 1)
				s += ", ";
		}
		s += "|";

		return s;
	}

	// Gets state name using no additional formatting.
	string GetStateNameGeneral(int StateIndex)
	{
		string s = "";

		for (int i = 0; i < stateNames[StateIndex].size(); i++)
		{
			s += stateNames[StateIndex][i];
			if (i < stateNames[StateIndex].size() - 1)
				s += ", ";
		}

		return s;
	}

	// Converts vector<string> to properly formatted name.
	string GetNameFromVector(vector<string> StateNames)
	{
		string s = "";

		if (isDFA)
		{
			s += "{";
			for (int i = 0; i < StateNames.size(); i++)
			{
				s += StateNames[i];
				if (i < StateNames.size() - 1)
					s += ", ";
			}
			s += "}";
		}
		else
		{
			s += "|";
			for (int i = 0; i < StateNames.size(); i++)
			{
				s += StateNames[i];

				if (i < StateNames.size() - 1)
					s += ", ";
			}
			s += "|";
		}

		return s;
	}

	// Gets string for all of the names of the states using set notation.
	string GetStateNameDFA(int StateIndex)
	{
		string s = "";

		s += "{";
		for (int i = 0; i < stateNames[StateIndex].size(); i++)
		{
			s += stateNames[StateIndex][i];
			if (i < stateNames[StateIndex].size() - 1)
				s += ", ";
		}
		s += "}";

		return s;
	}

	// Gets a state index given an input name.  Note that this method runs in O(n), where n is the number
	// of states.  Returns -1 if the name does not exist.
	int GetIndexByName(string StateName)
	{
		for (int i = 0; i < stateNames.size(); i++)  // Iterate over all state names.
		{
			// Check to see whether the name entered is in the same format as the state names, given whether
			// this is an NFA or DFA.
			if ((isDFA && GetStateNameDFA(i) == StateName) || (!isDFA && GetStateNameNFA(i) == StateName))
				return i;
		}

		return -1;  // If we've reached this point, we've found no state by the name StateName.
	}

	// Vector input must be sorted properly for this method to work.
	int GetIndexByName(vector<string> StateNameTest)
	{
		for (int i = 0; i < stateNames.size(); i++)  // Iterate over all state names.
		{
			bool isStateEqual = true;

			// Iterate over all components of state names.  Each one must equal StateNameTest.
			if (stateNames[i].size() == StateNameTest.size())
			{
				for (int j = 0; j < stateNames[i].size(); j++)
				{
					if (stateNames[i][j] != StateNameTest[j])
					{
						isStateEqual = false;
						break;
					}
				}
			}
			else
				isStateEqual = false;

			if (isStateEqual)
				return i;
		}

		return -1;
	}
	// End Helper Methods -----------------------------------------------------------------------------------
public:
	// Default constructor
	FA()
	{
		alphabet = vector<char>();
		stateNames = vector<vector<string>>();
		validTransition = vector<vector<string>>();
		isInitialFinal = vector<vector<bool>>();

		startStateIndex = -1; 
		isDFA = false;
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

		isDFA = false;
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

	// Tries to unify a set of states.  The new state will contain the names of all input states along with all of their transitions.
	bool AddUnionState(vector<int> StatesToUnify)
	{
		// Initialize a vector to contain all of the names of the states to unify.  Also, unify Names from indeces + Remove duplicate names.
		vector<string> newStateName = UnionStateNames(StatesToUnify);
		ReorderNames(newStateName);

		// Check to see whether this is a final state.
		bool isFinal = false;
		for (int i = 0; i < StatesToUnify.size(); i++)
		{
			if (isInitialFinal[StatesToUnify[i]][1])
			{
				isFinal = true;
				break;
			}
		}

		// If all the states exist, we're ready to go.  Now unify the states.

		// Sort name before attempting to add.
		AddState(false, isFinal, newStateName);  // For now, do not worry about whether this is an initial state.
		string actualStartStateName = GetNameFromVector(newStateName);

		// Add all of the outgoing transitions for each state.
		for (int i = 0; i < StatesToUnify.size(); i++)
		{
			// string actualEndStateName = (isDFA ? GetStateNameDFA(stateIndeces[i]) : GetStateNameNFA(stateIndeces[i]));

			for (int j = 0; j < validTransition[StatesToUnify[i]].size(); j++)
			{
				string actualEndStateName = GetNameFromVector(stateNames[j]);

				for (int k = 0; k < validTransition[StatesToUnify[i]][j].size(); k++)
				{
					// Add all transitions here.
					AddTransition(actualStartStateName, actualEndStateName, validTransition[StatesToUnify[i]][j][k]);
				}
			}
		}

		return true;
	}

	// Adds an empty state to the FA.  Returns whether the addition was successful (i.e. there shouldn't be two starting states; two states should not have
	// the same name.)
	bool AddState(bool IsInitial, bool IsFinal, string Name)
	{
		// First check to make sure name is not a null string.
		if (Name == "")
			return false;

		if (GetIndexByName(Name) != -1)  // How to check whether a state of name 'Name' exists
			return false;

		if (IsInitial)
		{
			for (int i = 0; i < isInitialFinal.size(); i++)
			{
				if (isInitialFinal[i][0])  // We've found an initial state, so do not allow addition of another initial state.
					return false;
			}
		}

		// Add new row and column to the adjacency matrix + add state to structure.

		// Add state name
		stateNames.push_back(vector<string>());
		stateNames[stateNames.size() - 1].push_back(Name);

		// Add initial/final status
		isInitialFinal.push_back(vector<bool>(2));
		isInitialFinal[isInitialFinal.size() - 1][0] = IsInitial;
		isInitialFinal[isInitialFinal.size() - 1][1] = IsFinal;

		// Update starting state index.
		if (IsInitial)
			startStateIndex = isInitialFinal.size() - 1;

		// Update Matrix
		for (int i = 0; i < validTransition.size() + 1; i++)
		{
			if (i < validTransition.size())
			{
				validTransition[i].push_back("");  // Add NULL transition to latest new state.
			}
			else
			{
				validTransition.push_back(vector<string>(validTransition[0].size()));  // Add state with no transitions.
				break;  // Otherwise, the loop will continue forever since size is constantly increasing.
			}
		}

		return true;  // If we've made it to this point, we will successfully return a new state.
	}

	// Overload function used to add state which is union of states.
	bool AddState(bool IsInitial, bool IsFinal, vector<string> StateNameVector)
	{
		if (GetIndexByName(StateNameVector) != -1)  // How to check whether a state of name 'Name' exists
			return false;

		if (IsInitial)
		{
			for (int i = 0; i < isInitialFinal.size(); i++)
			{
				if (isInitialFinal[i][0])  // We've found an initial state, so do not allow addition of another initial state.
					return false;
			}
		}

		// Add new row and column to the adjacency matrix + add state to structure.

		// Add state name
		stateNames.push_back(vector<string>());

		for (int i = 0; i < StateNameVector.size(); i++)
		{
			stateNames[stateNames.size() - 1].push_back(StateNameVector[i]);
		}

		// Add initial/final status
		isInitialFinal.push_back(vector<bool>(2));
		isInitialFinal[isInitialFinal.size() - 1][0] = IsInitial;
		isInitialFinal[isInitialFinal.size() - 1][1] = IsFinal;

		// Update starting state index.
		if (IsInitial)
			startStateIndex = isInitialFinal.size() - 1;

		// Update Matrix
		for (int i = 0; i < validTransition.size() + 1; i++)
		{
			if (i < validTransition.size())
			{
				validTransition[i].push_back("");  // Add NULL transition to latest new state.
			}
			else
			{
				validTransition.push_back(vector<string>(validTransition[0].size()));  // Add state with no transitions.
				break;  // Otherwise, the loop will continue forever since size is constantly increasing.
			}
		}

		return true;  // If we've made it to this point, we will successfully return a new state.
	}

	// Tries to add a transition.  Note that in order for this to be successful, the start state and end state must already exist AND
	// the TransitionChar must be part of the alphabet AND the transition must not already exist.
	bool AddTransition(string StartState, string EndState, char TransitionChar)
	{
		bool isCharInAlphabet = false;

		for (int i = 0; i < alphabet.size() + 1; i++)
		{
			if (i < alphabet.size())
			{
				if (TransitionChar == alphabet[i])
				{
					isCharInAlphabet = true;
					break;
				}
			}
			else if (TransitionChar == '_')
				isCharInAlphabet = true;
		}

		// If the character is not in the alphabet or is a lambda, break now.
		if (!isCharInAlphabet)
			return false;

		// Now check to see whether the starting and ending states exist.
		int startStateIndex = GetIndexByName(StartState);
		int endStateIndex = GetIndexByName(EndState);

		if (startStateIndex == -1 || endStateIndex == -1)  // Either one or both of the states do not exist; return false.
			return false;
		
		// Finally, check to make sure added transition is not a duplicate.
		for (int i = 0; i < validTransition[startStateIndex][endStateIndex].size(); i++)
		{
			if (validTransition[startStateIndex][endStateIndex][i] == TransitionChar)  // We've found a duplicate.  Return false.
				return false;
		}

		// Now add the transition.
		validTransition[startStateIndex][endStateIndex] += TransitionChar;

		return true;
	}

	// Checks to see whether an input is valid using BFS.
	bool CheckInputBFS(string StringInput, bool Debug)
	{
		// Test all possible solutions for given input using BFS.  If we arrive at state that is not final or cannot finish reading input, return false.
		queue<string> inputQ = queue<string>();  // Stores remaining string for given item in stateIndexQ.
		queue<int> stateIndexQ = queue<int>();

		inputQ.push(StringInput);
		stateIndexQ.push(startStateIndex);

		int currentStateIndex = -1;  // Check to make sure these are valid at the end.
		string currentInputString = "";
		bool isInFinalState = false;  // Check to see if, upon completely reading input, the FA is in a final (accepting) state.

		while (stateIndexQ.size() > 0)
		{
			currentInputString = inputQ.front();  // Get first item in queues and then pop them.
			currentStateIndex = stateIndexQ.front();

			char currentChar = '_';  // Get current character.  If string is empty, this will remain lambda.
			if (currentInputString.size() > 0)
				currentChar = currentInputString[0];

			inputQ.pop();
			stateIndexQ.pop();

			if (Debug)
			{
				std::cout << "State \"" << stateNames[currentStateIndex][0] << "\":  \'" << currentChar << "\' (\"" << currentInputString << "\") --> ";
			}

			if (currentChar != '_')  // The string must not be empty; otherwise, we will not enqueue this item.
			{
				vector<int> nextStateIndeces = GetNextStateIndeces(currentStateIndex, currentChar);  // Get transitions for input character, if there are any.
				vector<int> nextStateIndecesLambda = GetNextStateIndeces(currentStateIndex, '_');  // Get lambda-transitions, if there are any.

				int sTotal = nextStateIndeces.size() + nextStateIndecesLambda.size();
				int sIndex = 0;  // Index used for formatting text.

				// Enqueue lambda-transitions.  
				for (int i = 0; i < nextStateIndecesLambda.size(); i++)
				{
					if (Debug)
					{
						std::cout << "\"" << stateNames[nextStateIndecesLambda[i]][0] << "\"" << (sIndex < sTotal - 1 ? ", " : "");
					}

					inputQ.push(currentInputString);
					stateIndexQ.push(nextStateIndecesLambda[i]);
					sIndex++;
				}

				// Erase input character from string; it has been read.
				currentInputString = currentInputString.erase(0, 1);

				// Enqueue all other transitions
				for (int i = 0; i < nextStateIndeces.size(); i++)
				{
					if (Debug)
					{
						std::cout << "\"" << stateNames[nextStateIndeces[i]][0] << "\"" << (sIndex < sTotal - 1 ? ", " : "");
					}

					inputQ.push(currentInputString);
					stateIndexQ.push(nextStateIndeces[i]);
					sIndex++;
				}
			}
			else
			{
				if (!isInFinalState)  // Check to see if the machine is in a final state.  If so, break out of while-loop immediately to save time.
				{
					isInFinalState = isInitialFinal[currentStateIndex][1];
					if (isInFinalState)
						break;
				}
			}

			if (Debug)
				std::cout << endl;
		}

		return isInFinalState;
	}

	// Checks to see whether an input is valid using DFS (much more memory efficient and usually much faster than BFS except when input does not return a solution).
	bool CheckInputDFS(string StringInput, bool Debug)
	{
		// Test all possible solutions for given input using DFS.  If we arrive at state that is not final or cannot finish reading input, return false.
		stack<string> inputQ = stack<string>();  // Stores remaining string for given item in stateIndexQ.
		stack<int> stateIndexQ = stack<int>();

		inputQ.push(StringInput);
		stateIndexQ.push(startStateIndex);

		int currentStateIndex = -1;  // Check to make sure these are valid at the end.
		string currentInputString = "";
		bool isInFinalState = false;  // Check to see if, upon completely reading input, the FA is in a final (accepting) state.

		while (stateIndexQ.size() > 0)
		{
			currentInputString = inputQ.top();  // Get first item in queues and then pop them.
			currentStateIndex = stateIndexQ.top();

			char currentChar = '_';  // Get current character.  If string is empty, this will remain lambda.
			if (currentInputString.size() > 0)
				currentChar = currentInputString[0];

			inputQ.pop();
			stateIndexQ.pop();

			if (Debug)
			{
				std::cout << "State \"" << stateNames[currentStateIndex][0] << "\":  \'" << currentChar << "\' (\"" << currentInputString << "\") --> ";
			}

			if (currentChar != '_')  // The string must not be empty; otherwise, we will not enqueue this item.
			{
				vector<int> nextStateIndeces = GetNextStateIndeces(currentStateIndex, currentChar);  // Get transitions for input character, if there are any.
				vector<int> nextStateIndecesLambda = GetNextStateIndeces(currentStateIndex, '_');  // Get lambda-transitions, if there are any.

				int sTotal = nextStateIndeces.size() + nextStateIndecesLambda.size();
				int sIndex = 0;  // Index used for formatting text.

								 // Enqueue lambda-transitions
				for (int i = 0; i < nextStateIndecesLambda.size(); i++)
				{
					if (Debug)
					{
						std::cout << "\"" << stateNames[nextStateIndecesLambda[i]][0] << "\"" << (sIndex < sTotal - 1 ? ", " : "");
					}

					inputQ.push(currentInputString);
					stateIndexQ.push(nextStateIndecesLambda[i]);
					sIndex++;
				}

				// Erase input character from string; it has been read.
				currentInputString = currentInputString.erase(0, 1);

				// Enqueue all other transitions
				for (int i = 0; i < nextStateIndeces.size(); i++)
				{
					if (Debug)
					{
						std::cout << "\"" << stateNames[nextStateIndeces[i]][0] << "\"" << (sIndex < sTotal - 1 ? ", " : "");
					}

					inputQ.push(currentInputString);
					stateIndexQ.push(nextStateIndeces[i]);
					sIndex++;
				}
			}
			else
			{
				if (!isInFinalState)  // Check to see if the machine is in a final state.  If so, break out of while-loop immediately to save time.
				{
					isInFinalState = isInitialFinal[currentStateIndex][1];
					if (isInFinalState)
						break;
				}
			}

			if (Debug)
				std::cout << endl;
		}

		return isInFinalState;
	}

	void PrintFA(int ColumnWidth)
	{
		cout << "State" + GetSpaces("State", ColumnWidth);

		// Print column for each alphabet character and lambda.
		for (int i = 0; i < alphabet.size() + 1; i++)
		{
			if (i < alphabet.size())
				cout << alphabet[i] << GetSpaces(ColumnWidth);
			else
				cout << "_";
		}

		cout << endl;

		for (int i = 0; i < validTransition.size(); i++)
		{
			string curInput = (isDFA ? GetStateNameDFA(i) : GetStateNameNFA(i)) + " - " + (isInitialFinal[i][0] ? "I" : "") + (isInitialFinal[i][1] ? "F" : "");

			cout << curInput << GetSpaces(curInput, ColumnWidth);

			for (int j = 0; j < alphabet.size() + 1; j++)
			{
				vector<string> nextStateNames = vector<string>();

				if (j < alphabet.size())
				{
					vector<int> nextIndeces = GetNextStateIndeces(i, alphabet[j]);

					for (int k = 0; k < nextIndeces.size(); k++)
						nextStateNames.push_back(isDFA ? GetStateNameDFA(nextIndeces[k]) : GetStateNameNFA(nextIndeces[k]));
				}
				else
				{
					vector<int> nextIndeces = GetNextStateIndeces(i, '_');

					for (int k = 0; k < nextIndeces.size(); k++)
						nextStateNames.push_back(isDFA ? GetStateNameDFA(nextIndeces[k]) : GetStateNameNFA(nextIndeces[k]));
				}

				curInput = GetNameFromVector(nextStateNames);

				/*for (int k = 0; k < nextStateNames.size(); k++)
				{
					cout << nextStateNames[k] << (k < nextStateNames.size() - 1 ? ", " : "");
				}*/

				// cout << "\t";

				cout << curInput << GetSpaces(curInput, ColumnWidth);
			}

			cout << endl;
		}
	}
};

#endif