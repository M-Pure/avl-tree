#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "GatorAVL.h"
#include "parsing.h"
using namespace std;

/* Note: 
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope. 
*/


 int main(){

 	GatorAVL tree;

 	// First line is an integer number of commands
 	string line = "";
 	getline(cin, line);
 	int numCommands = stoi(line);

 	/* Main loop iterates N number of times, where N is the integer first line of input */
 	for (int i = 1; i <= numCommands; ++i) {
 		vector<string> cmds;
 		getline(cin, line);
 		istringstream in(line); // Convert line to input stream
 		// Format is invalid, cannot pass to AVL functions
 		if (validateCommand(in, cmds) == "unsuccessful") {
 			cout << "unsuccessful" << endl;
 			continue;
 		}
 		/* If the command has a valid format, the AVL tree functions will further decide
 		 * if the data fits the given constraints and instance of the tree,
 		 * printing "unsuccessful" or "successful" accordingly */
 		if (cmds[0] == "insert") {
 			tree.insert(cmds[1], stoi(cmds[2]));
 		}
 		else if (cmds[0] == "remove") {
 			tree.remove(stoi(cmds[1]));
 		}
 		else if (cmds[0] == "search") {
 			// search ID or name?
 			bool isID = true;
 			for (char c : cmds[1]) {
 				if (!isdigit(c)) {
 					isID = false;
 				}
 			}
 			if (isID) {
 				tree.search(stoi(cmds[1]));
 			}
 			else {
 				tree.search(cmds[1]);
 			}
 		}
 		else if (cmds[0] == "removeInorder") {
 			tree.removeInorder(stoi(cmds[1]));
 		}
 		else if (cmds[0] == "printInorder") {
 			tree.printInorder();
 		}
 		else if (cmds[0] == "printPreorder") {
 			tree.printPreorder();
 		}
 		else if (cmds[0] == "printPostorder") {
 			tree.printPostorder();
 		}
 		else if (cmds[0] == "printLevelCount") {
 			tree.printLevelCount();
 		}
 	}
	return 0;
}



