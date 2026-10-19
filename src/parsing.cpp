#include <string>
#include <sstream>
#include <vector>
#include "parsing.h"
using namespace std;

// Names must contain only the characters " ", a-z, A-Z
bool validateName(string name) {
	for (char c : name) {
		if (!isalpha(c)) {
			if (c != ' ') {
				return false;
			}
		}
	}
	return true;
}

// ID must be 8 characters long, each of which is a digit
bool validateID(string id) {
	if (id.size() != 8) {
		return false;
	}
	for (char c : id) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

/* Pushes *valid* commands to cmds and returns "successful" if
 * command *format* is valid, "unsuccessful" otherwise
 * When "successful", the actual operation may be invalid given the AVL tree's state,
 * in which case the tree function itself will carry out further validation */
string validateCommand(istringstream& in, vector<string>& cmds) {
	string function = "";
	in >> function;

	if (function == "printInorder" || function == "printPreorder" ||
		function == "printPostorder" || function == "printLevelCount" ) {
		cmds.push_back(function);
		return "successful";
	}

	// Clear unneeded space for longer commands
	string space = "";
	getline(in, space, ' ');

	if (function == "insert") {
		string garbage = "";
		getline(in, garbage, '"');
		string name = "";
		getline(in, name, '"');
		if (!validateName(name)) {
			return "unsuccessful";
		}
		getline(in, garbage, ' ');
		string id = "";
		getline(in, id);
		if (!validateID(id)) {
			return "unsuccessful";
		}
		cmds.push_back(function);
		cmds.push_back(name);
		cmds.push_back(id);
		return "successful";
	}
	else if (function == "remove") {
		string id = "";
		getline(in, id);
		if (!validateID(id)) {
			return "unsuccessful";
		}
		cmds.push_back(function);
		cmds.push_back(id);
		return "successful";
	}
	else if (function == "search") {
		// Is it a search "Name" or search ID?
		if (in.peek() == '"') { // search "NAME"
			string garbage = "";
			getline(in, garbage, '"');
			string name = "";
			getline(in, name, '"');
			if (!validateName(name)) {
				return "unsuccessful";
			}
			cmds.push_back(function);
			cmds.push_back(name);
			return "successful";
		}
		// search ID
		string id = "";
		getline(in, id);
		if (!validateID(id)) {
			return "unsuccessful";
		}
		cmds.push_back(function);
		cmds.push_back(id);
		return "successful";
	}
	else if (function == "removeInorder") {
		string n = "";
		getline(in, n);
		cmds.push_back(function);
		cmds.push_back(n);
		return "successful";
	}
	else { // Function name invalid
		return "unsuccessful";
	}
}