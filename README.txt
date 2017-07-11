COT4210 Final Project - NFA to DFA Converter
by Mark Tushemereirwe, Moshi Xu, John Santoro, and Gregory Wood

By default, this program reads the file "NFA-2.txt".  It then prompts the user with the following
options:

	Option			Key Command	Description
1.	Quit				'q'			Exits the program.
2.	Print NFA			'p'			Prints the NFA (or DFA, if the NFA has been converted).
3.	Convert To DFA		'c'			Converts the NFA to a DFA.  This method does nothing if
								the finite automation is already a DFA.
4.	Test Input			't'			Performs BFS on state machine, bounded by input, to
								determine whether the input is accepted.  O(2^n) time and
								space for NFAs and O(n) time and space for DFAs.
5.	Load New NFA		'l'			Loads a new NFA from the specified file in the working
								directory.

To compile this program, use the following command in a console window:

make

And then run the program using

./a.out