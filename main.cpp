#include "FA.h"
#include <fstream>

// Files are read in specific format as described in the project report.
FA CreateFAFromFile(const string& FileName)
{
	ifstream file(FileName, std::ifstream::in);

	// Variables to keep track of input
	int readerIndex = 0;
	int stateIndex = 0;
	int stateCount = -1;
	int transitionIndex = 0;
	int transitionCount = -1;
	int lengthOfAlphabet = 0;

	// Input alphabet
	vector<char> alphabet = vector<char>();
	
	// Input state names
	vector<string> stateNames = vector<string>();

	// Input state transition functions
	vector<vector<char>> validTransitions = vector<vector<char>>();
	vector<vector<int>> nextStateIndeces = vector<vector<int>>();

	// 1st index is initial, 2nd is final
	vector<vector<int>> isInitialFinal = vector<vector<int>>();

	// validTransitions[0][0] --> nextStateIndeces[0][0] 

	// Loop for reading the file
	while (true)
	{
		if (readerIndex == 0)  // Get number of alphabet symbols
		{
			file >> lengthOfAlphabet;
			alphabet.resize(lengthOfAlphabet);
		}
		else  // Read alphabet + do all other things necessary for setting up FA.
		{
			if (readerIndex - 1 < lengthOfAlphabet) // Finish reading alphabet.
			{
				file >> alphabet[readerIndex - 1];
			}
			else  // Get states
			{
				if (stateCount == -1)  // Get number of states from file.
				{
					file >> stateCount;

					stateNames.resize(stateCount);
					validTransitions.resize(stateCount);
					nextStateIndeces.resize(stateCount);
					isInitialFinal.resize(stateCount);
				}
				else  // Read all states.
				{
					if (stateIndex < stateCount)  // Only read states when the current state index is less than the state count.
					{
						if (stateNames[stateIndex] == "")  // Assign name to state first.
						{
							file >> stateNames[stateIndex];

							isInitialFinal[stateIndex].resize(2);

							file >> isInitialFinal[stateIndex][0];
							file >> isInitialFinal[stateIndex][1];
						}
						else  // Read in transition functions.
						{
							if (transitionCount == -1)  // Get transition function count for current state.
							{
								file >> transitionCount;

								validTransitions[stateIndex].resize(transitionCount);
								nextStateIndeces[stateIndex].resize(transitionCount);
							}
							else  // Read all transitions
							{
								if (transitionIndex < transitionCount)  // Read transitions when transition index is less than the count.
								{
									file >> validTransitions[stateIndex][transitionIndex];
									file >> nextStateIndeces[stateIndex][transitionIndex];

									transitionIndex++;
								}
								else  // We have finished reading all of this state's transitions.  Move on to next state and reset transition variables.
								{
									transitionIndex = 0;
									transitionCount = -1;

									stateIndex++;
								}
							}
						}
					}
					else  // We have finished reading all states.
					{
						// Do nothing; we are done.
					}
				}
			}
		}

		if (file.eof())
		{
			file.close();
			break;
		}
		else
			readerIndex++;
	}

	// Return statement should be modified so that data read from file is included in FA.  Use the statement "int blub = 0;" for debugging purposes.
	int blub = 0;
	return FA(alphabet, stateNames, validTransitions, nextStateIndeces, isInitialFinal);
}

int main()
{
	cout << "COT4210 Final Project - NFA to DFA Converter" << endl 
		<< "by Mark Tushemereirwe, John Santoro, and Gregory Wood" << endl << endl;

	FA nfa(CreateFAFromFile("NFA-2.txt"));
	FA dfa = FA();

	// Maximum column width...
	int columnWidth = 29;

	/*nfa.PrintFA(columnWidth);

	vector<int> statesToUnify = vector<int>();

	statesToUnify.push_back(0);
	statesToUnify.push_back(2);

	nfa.AddUnionState(statesToUnify);

	statesToUnify.clear();

	statesToUnify.push_back(1);
	statesToUnify.push_back(3);

	nfa.AddUnionState(statesToUnify);

	cout << "After Union:  " << endl;
	nfa.PrintFA(columnWidth);*/

	while (true)
	{
		bool breakThis = false;
		bool isInitial = false;
		bool isFinal = false;
		bool barf = false;
		string inputString = "";
		string startState = "";
		string endState = "";
		int inputState = 0;
		int startStateIndex = 0;
		int endStateIndex = 0;
		vector<int> statesToUnify = vector<int>();
		
		cout << "Type command here.  'q' = quit; 'p' = print NFA; 'c' = convert to DFA;" << endl
			<< "'t' = test input; 'a' = add state; 'z' = add transition; 'u' = add unified state" << endl
			<< "'y' = erase transition; 'd' = delete state; 'e' = E-Closure" << endl;
		char ui = cin.get();

		// Clear input.
		cin.ignore();
		
		switch (ui)
		{
		case 'e':  // Perform epsilon-closure on the state machine.  This is a pre-step for NFA to DFA conversion.
			cout << "Enter index of state to remove epsilon-transitions from." << endl;
			cin >> inputState;

			// Clear input.
			cin.ignore();
			inputString = nfa.StateNamePublic(inputState);
			cout << "Epsilon-Closure of state \"" << inputString << "\" was " << (nfa.RemoveLambdaTransitions(inputState) ? "successful." : "not successful.") << endl;
			inputString = "";
			break;
		case 'd':  // Delete a state.
			cout << "Enter index of state to delete." << endl;
			cin >> inputState;

			// Clear input.
			cin.ignore();

			inputString = nfa.StateNamePublic(inputState);
			cout << "Deletion of state \"" << inputString << "\" was " << (nfa.RemoveState(inputState) ? "successful." : "not successful.") << endl;
			inputString = "";
			break;
		case 'y':  // Erase a transition.
			cout << "Enter starting state index." << endl;
			cin >> startStateIndex;

			// Clear input.
			cin.ignore();

			cout << "Enter ending state index." << endl;
			cin >> endStateIndex;

			// Clear input.
			cin.ignore();

			cout << "Enter transition character to remove." << endl;
			ui = cin.get();

			// Clear input.
			cin.ignore();

			cout << "Transition \"" << nfa.StateNamePublic(startStateIndex) << "\" --\'" << ui << "\'--> " << nfa.StateNamePublic(endStateIndex)
				<< " was " << (nfa.RemoveTransition(startStateIndex, endStateIndex, ui) ? "successfully " : "unsuccessfully ") << "removed." << endl;
			ui = '\0';
			break;
		case 'u':  // Unify two states.  Note that a new state is produced.
			inputState = 0;
			statesToUnify.clear();

			while (inputState != -1)
			{
				cout << "Enter the index of a state you wish to unify.  Use -1 to stop inputting sequence." << endl;
				cin >> inputState;

				// Clear input.
				cin.ignore();

				if (inputState != -1)
					statesToUnify.push_back(inputState);
			}

			cout << "Addition of unified state " << (nfa.ReplaceStatesWithUnion(statesToUnify) ? "successful." : "not successful.") << endl;
			break;
		case 'z':  // Add a transition.
			cout << "Enter start state:  " << endl;
			getline(cin, startState);  // Clearing not needed.

			cout << "Enter end state:  " << endl;
			getline(cin, endState);  // Clearing not needed.

			cout << "Enter desired transition character.  Use '_' for lambda:" << endl;
			ui = cin.get();

			// Clear input.
			cin.ignore();

			cout << "Addition of " << startState << " --\'" << ui << "\'--> " << endState << " transition "
				<< (nfa.AddTransition(startState, endState, ui) ? "successful" : "not successful") << endl;

			ui = '\0';
			break;
		case 'a':  // Add a state.
			cout << "Enter new state name:  " << endl;
			getline(cin, inputString);  // Clearing not needed.

			cout << "Is initial?  y/n" << endl;
			while (ui != 'y' && ui != 'n')
			{
				ui = cin.get();

				// Clear input.
				cin.ignore();

				if (ui == 'y')
					isInitial = true;
				else if (ui == 'n')
					isInitial = false;
				else
					cout << "Invalid input.  Please enter 'y' or 'n'." << endl;
			}

			cout << "Is final?  y/n" << endl;
			ui = '\0';  // Clear character so that this while-loop is reached.
			while (ui != 'y' && ui != 'n')
			{
				ui = cin.get();

				// Clear input.
				cin.ignore();

				if (ui == 'y')
					isFinal = true;
				else if (ui == 'n')
					isFinal = false;
				else
					cout << "Invalid input.  Please enter 'y' or 'n'." << endl;
			}
			cout << "Addition of state \"" << inputString << "\" was " << (nfa.AddState(isInitial, isFinal, "|" + inputString + "|", false) ? "successful." : "not successful.") << endl;
			break;
		case 't':  // Test an input
			cout << "Enter input to test.  Enter nothing for lambda string:  " << endl;
			getline(cin, inputString);  // Clearing not needed.

			cout << "Show steps?  y/n" << endl;
			while (ui != 'y' && ui != 'n')
			{
				ui = cin.get();

				// Clear input.
				cin.ignore();

				if (ui == 'y')
				{
					cout << "I'm showing my work." << endl;
				}
				else if (ui == 'n')
				{
					cout << "You'll never know what I'm doing." << endl;
				}
				else
					cout << "Invalid input.  Please enter 'y' or 'n'." << endl;
			}
			barf = nfa.CheckInputBFS(inputString, ui == 'y');
			cout << "String \"" << inputString << "\"" << (barf ? " is" : " is not") << " accepted by the FA." << endl;
			break;
		case 'c':
			cout << "Converting NFA to DFA.  Show steps?  y/n:  " << endl;
			ui = '\0';
			while (ui != 'y' && ui != 'n')
			{
				ui = cin.get();

				// Clear input.
				cin.ignore();

				if (ui == 'y')
				{
					cout << "I'm showing my work." << endl;
				}
				else if (ui == 'n')
				{
					cout << "You'll never know what I'm doing." << endl;
				}
				else
					cout << "Invalid input.  Please enter 'y' or 'n'." << endl;
			}
			dfa = nfa.ConvertToDFA(ui == 'y');
			break;
		case 'p':
			cout << "Printing FA..." << endl;
			nfa.PrintFA(columnWidth);
			break;
		case 'q':
			cout << "Goodbye." << endl;
			breakThis = true;
			break;
		default:
			cout << "Unrecognized input.  Please try again." << endl;
			break;
		}

		// Break if user desires to quit.
		if (breakThis)
			break;
	}

	cin.get();

	return 0;
}