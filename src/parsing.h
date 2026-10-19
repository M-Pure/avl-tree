#pragma once
#include <string>
#include <sstream>
#include <vector>
using namespace std;

bool validateName(string name);

bool validateID(string id);

string validateCommand(istringstream& in, vector<string>& cmds);

