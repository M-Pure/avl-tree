#define CATCH_CONFIG_MAIN
#include <algorithm>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include "catch.hpp"
#include "../src/GatorAVL.h"
#include "../src/parsing.h"


/*
	To check output (At the Project1 directory):
		g++ -std=c++14 -Werror -Wuninitialized -o build/test test-unit/test.cpp && build/test
*/

TEST_CASE("Insert, no rotations", "[insert]"){

	GatorAVL t;
	t.insert("Joshua", 50000000);
	t.insert("Sam", 30000000);
	t.insert("Michael", 40000000);
	t.insert("Pam", 20000000);
	t.insert("First", 10000000);
	t.insert("Alex", 60000000);
	t.insert("Robert", 80000000);
	t.insert("Lucy", 70000000);
	t.insert("Last", 90000000);

	vector<string> inorderNames = t.inorder();
	vector<string> expectedNames = {
		"First", "Pam", "Sam", "Michael", "Joshua",
		"Alex", "Lucy", "Robert", "Last"
	};
	REQUIRE(inorderNames.size() == expectedNames.size());
	REQUIRE(inorderNames == expectedNames);

}

TEST_CASE("Insert, Left Rotation", "[insert]"){

	GatorAVL t;
	t.insert("Josh", 10000000);
	t.insert("Adam", 20000000);
	t.insert("Alan", 30000000);


	vector<string> inorderNames = t.inorder();
	vector<string> expectedNames = {
		"Josh", "Adam", "Alan"
	};
	REQUIRE(inorderNames.size() == expectedNames.size());
	REQUIRE(inorderNames == expectedNames);

}

TEST_CASE("Insert, Left Right Rotation", "[insert]"){

	GatorAVL t;
	t.insert("Sam", 40000000);
	t.insert("Brian", 20000000);
	t.insert("Mike", 60000000);
	t.insert("Josh", 10000000);
	t.insert("Alicia", 50000000);
	t.insert("Amy", 80000000);
	t.insert("Michelle", 70000000);
	t.insert("Daniel", 75000000);

	vector<string> inorderNames = t.inorder();
	vector<string> expectedNames = {
		"Josh", "Brian", "Sam",
		"Alicia", "Mike", "Michelle",
		"Daniel", "Amy"
	};
	REQUIRE(inorderNames.size() == expectedNames.size());
	REQUIRE(inorderNames == expectedNames);
}

TEST_CASE("Insert, Right Rotation", "[insert]"){

	GatorAVL t;
	t.insert("Sam", 40000000);
	t.insert("Brian", 20000000);
	t.insert("Mike", 60000000);
	t.insert("Josh", 10000000);
	t.insert("Alicia", 50000000);
	t.insert("Amy", 80000000);
	t.insert("Michelle", 90000000);
	t.insert("Daniel", 95000000);

	vector<string> inorderNames = t.inorder();
	vector<string> expectedNames = {
		"Josh", "Brian", "Sam",
		"Alicia", "Mike", "Amy",
		"Michelle", "Daniel"
	};
	REQUIRE(inorderNames.size() == expectedNames.size());
	REQUIRE(inorderNames == expectedNames);
}

TEST_CASE("Insert, Right Left Rotation", "[insert]"){

	GatorAVL t;
	t.insert("Sam", 40000000);
	t.insert("Brian", 20000000);
	t.insert("Mike", 60000000);
	t.insert("Josh", 10000000);
	t.insert("Bob", 21000000);
	t.insert("Alicia", 50000000);
	t.insert("Amy", 80000000);
	t.insert("Sara", 23000000);
	t.insert("Michelle", 90000000);
	t.insert("Tom", 22000000);



	vector<string> inorderNames = t.inorder();
	vector<string> expectedNames = {
		"Josh", "Brian", "Bob",
		"Tom", "Sara", "Sam",
		"Alicia", "Mike", "Amy",
		"Michelle"
	};
	REQUIRE(inorderNames.size() == expectedNames.size());
	REQUIRE(inorderNames == expectedNames);
}

TEST_CASE("Large Random Insert", "[insert]") {
	GatorAVL t;
	/* 1. Get all string names for expectedNames.
	 * 2. Turn those into another vector of ids for each name
	 * 3. Sort the ids in this new list
	 * 4. insert all values into the tree
	 * 5. Use inorder() do get inorder names in tree
	 * 6. Convert names in the vector to id's
	 * 7. REQUIRE statements */
	vector<string> names;
	vector<int> expectedIDs;

	random_device rd; // Seed
	mt19937 generator(rd()); // Random generator
	uniform_int_distribution<> idDistribution(10000000, 99999999); // Range

	/* Insert 100 random nodes into the tree, and the equivalent
	 * into vector of ids, to compare with inorder traversal of tree */
	while (expectedIDs.size() < 100) {
		int randomID = idDistribution(generator);
		// Ensure ID is unique
		if (count(expectedIDs.begin(), expectedIDs.end(), randomID) == 0) {
			expectedIDs.push_back(randomID);
			t.insert("", randomID);
		}
	}

	// Inorder IDs
	sort(expectedIDs.begin(), expectedIDs.end());
	vector<int> ids = t.inorderIDs();

	REQUIRE(ids.size() == expectedIDs.size());
	REQUIRE(ids == expectedIDs);

	// Remove 10 random nodes
	uniform_int_distribution<> nthDistribution(0, 80); // Limit to 80 to prevent indexOutOfBounds
	while (expectedIDs.size() > 90) {
		int randomNth = nthDistribution(generator);
		t.removeInorder(randomNth);
		auto it = expectedIDs.begin();
		it += randomNth;
		expectedIDs.erase(it);
	}
	ids = t.inorderIDs();
	REQUIRE(ids.size() == expectedIDs.size());
	REQUIRE(ids == expectedIDs);
}

TEST_CASE("Traversals", "[traversals]"){

	GatorAVL t;
	t.insert("M", 50000000);
	t.insert("F", 40000000);
	t.insert("P", 80000000);
	t.insert("I", 45000000);
	t.insert("N", 60000000);
	t.insert("T", 90000000);
	t.insert("O", 70000000);
	t.insert("Z", 95000000);

	vector<string> inorderNames = t.inorder();
	vector<string> expectedNames1 = {
		"F", "I", "M", "N",
		"O", "P", "T", "Z"
	};
	REQUIRE(inorderNames.size() == expectedNames1.size());
	REQUIRE(inorderNames == expectedNames1);

	vector<string> preorderNames = t.preorder();
	vector<string> expectedNames2 = {
		"M", "F", "I", "P",
		"N", "O", "T", "Z"
	};
	REQUIRE(preorderNames.size() == expectedNames2.size());
	REQUIRE(preorderNames == expectedNames2);

	vector<string> postorderNames = t.postorder();
	vector<string> expectedNames3 = {
		"I", "F", "O", "N",
		"Z", "T", "P", "M"
	};
	REQUIRE(postorderNames.size() == expectedNames3.size());
	REQUIRE(postorderNames == expectedNames3);
}

TEST_CASE("Level Count", "[level count]"){

	GatorAVL t;

	REQUIRE(t.getLevelCount() == 0);
	t.insert("M", 50000000);
	REQUIRE(t.getLevelCount() == 1);
	t.insert("F", 40000000);
	REQUIRE(t.getLevelCount() == 2);
	t.insert("P", 80000000);
	REQUIRE(t.getLevelCount() == 2);
	t.insert("I", 45000000);
	REQUIRE(t.getLevelCount() == 3);
	t.insert("N", 60000000);
	REQUIRE(t.getLevelCount() == 3);
	t.insert("T", 90000000);
	REQUIRE(t.getLevelCount() == 3);
	t.insert("O", 70000000);
	REQUIRE(t.getLevelCount() == 4);
	t.insert("Z", 95000000);
	REQUIRE(t.getLevelCount() == 4);

}

TEST_CASE("Search ID", "[search]"){

	GatorAVL t;

	REQUIRE(t.searchIDHelp(t.getRoot(), 50000000) == nullptr);

	t.insert("M", 50000000);
	t.insert("F", 40000000);
	t.insert("P", 80000000);


	REQUIRE(t.searchIDHelp(t.getRoot(), 50000000) == t.getRoot());
	REQUIRE(t.searchIDHelp(t.getRoot(), 40000000) == t.getRoot()->left);
	REQUIRE(t.searchIDHelp(t.getRoot(), 80000000) == t.getRoot()->right);
	REQUIRE(t.searchIDHelp(t.getRoot(), 10000000) == nullptr);
	REQUIRE(t.searchIDHelp(t.getRoot(), 90000000) == nullptr);




}

TEST_CASE("Search Name", "[search]"){

	GatorAVL t;
	vector<int> nameMatches;
	t.searchNameHelp(t.getRoot(), "FindMe", nameMatches);
	REQUIRE(nameMatches.size() == 0);

	t.insert("FindMe", 40000000);
	t.searchNameHelp(t.getRoot(), "FindMe", nameMatches);
	REQUIRE(nameMatches.size() == 1);
	REQUIRE(nameMatches[0] == 40000000);
	nameMatches.clear();

	t.insert("FindMe", 20000000);
	t.insert("FindMe", 60000000);
	t.insert("FindMe", 30000000);
	t.insert("FindMe", 50000000);
	t.insert("FindMe", 10000000);
	t.insert("FindMe", 70000000);
	t.searchNameHelp(t.getRoot(), "FindMe", nameMatches);
	vector<int> expectedResult = {
		40000000, 20000000, 10000000, 30000000,
		60000000, 50000000, 70000000
	};
	REQUIRE(nameMatches.size() == expectedResult.size());
	REQUIRE(nameMatches == expectedResult);
}

TEST_CASE("Remove, no child", "[remove]"){
	GatorAVL t;
	// Edge: id does not xist
	t.remove(49387291);
	t.insert("J", 50000000);
	vector<string> names = t.inorder();
	REQUIRE(names.size() == 1);
	// Edge: removing when only the root exists
	t.remove(50000000);
	names = t.inorder();
	REQUIRE(names.size() == 0);

	t.insert("J", 50000000);
	t.insert("S", 90000000);
	t.insert("A", 10000000);
	t.remove(10000000);
	names = t.inorder();
	vector<string> expectedNames = {"J", "S"};
	REQUIRE(names.size() == 2);
	REQUIRE(names == expectedNames);
	REQUIRE(t.getRoot()->left == nullptr);

	t.insert("A", 10000000);
	t.remove(90000000);
	names = t.inorder();
	expectedNames = {"A", "J"};
	REQUIRE(names.size() == 2);
	REQUIRE(names == expectedNames);
	REQUIRE(t.getRoot()->right == nullptr);
}

TEST_CASE("Remove, 1 child", "[remove]"){
	GatorAVL t;
	// Edge: removing invalid id
	t.remove(39208173);
	// Edge: removing root
	t.insert("J", 50000000);
	t.insert("Q", 80000000);
	t.remove(50000000);
	vector<string> names = t.inorder();
	vector<string> expectedNames = {"Q"};
	REQUIRE(names.size() == 1);
	REQUIRE(names == expectedNames);
	t.remove(80000000);
	t.insert("J", 50000000);
	t.insert("C", 20000000);
	t.remove(50000000);
	names = t.inorder();
	expectedNames = {"C"};
	REQUIRE(names.size() == 1);
	REQUIRE(names == expectedNames);

	t.remove(20000000);
	t.insert("J", 50000000);
	t.insert("C", 20000000);
	t.insert("P", 70000000);
	t.insert("A", 10000000);
	t.insert("Z", 90000000);
	t.insert("E", 30000000);
	t.insert("D", 25000000);

	t.remove(30000000);
	names = t.inorder();
	expectedNames = {"A", "C", "D", "J", "P", "Z"};
	REQUIRE(names.size() == 6);
	REQUIRE(names == expectedNames);

	t.remove(70000000);
	names = t.inorder();
	expectedNames = {"A", "C", "D", "J", "Z"};
	REQUIRE(names.size() == 5);
	REQUIRE(names == expectedNames);
}

TEST_CASE("Remove, 2 children", "[remove]") {
	GatorAVL t;
	t.remove(68391082);
	// Edge: Removing root
	t.insert("B", 50000000);
	t.insert("A", 40000000);
	t.insert("C", 60000000);

	t.remove(50000000);
	vector<string> names = t.inorder();
	vector<string> expectedNames = {"A", "C"};
	REQUIRE(names.size() == 2);
	REQUIRE(names == expectedNames);
	REQUIRE(t.getRoot()->name == "C");

	t.remove(40000000);
	t.remove(60000000);
	t.insert("J", 50000000);
	t.insert("F", 30000000);
	t.insert("T", 70000000);
	t.insert("C", 20000000);
	t.insert("V", 90000000);
	t.insert("G", 40000000);
	t.insert("M", 68000000);
	t.insert("A", 10000000);
	t.insert("L", 66000000);
	t.insert("O", 69000000);
	t.insert("U", 75000000);
	t.insert("X", 95000000);
	t.insert("K", 63000000);
	t.remove(50000000);
	REQUIRE(t.getRoot()->name == "K");
	REQUIRE(t.getRoot()->id == 63000000);
	names = t.inorder();
	expectedNames = {
		"A", "C", "F", "G", "K",
		"L", "M", "O", "T", "U",
		"V", "X"
	};
	REQUIRE(t.getRoot()->right->left->left->left == nullptr);
	REQUIRE(names.size() == 12);
	REQUIRE(names == expectedNames);

	t.insert("successor right child", 80000000);
	t.remove(70000000);
	REQUIRE(t.getRoot()->right->name == "U");
	REQUIRE(t.getRoot()->right->id == 75000000);
	REQUIRE(t.getRoot()->right->right->left->name == "successor right child");
	names = t.inorder();
	REQUIRE(names.size() == 12);
	expectedNames = expectedNames = {
		"A", "C", "F", "G", "K",
		"L", "M", "O", "U",
		"successor right child", "V", "X"
	};
	REQUIRE(names == expectedNames);
}

TEST_CASE("Remove Nth", "[remove]") {
	GatorAVL t;
	t.removeInorder(5);
	t.insert("B", 50000000);
	t.insert("A", 40000000);
	t.insert("C", 60000000);
	t.removeInorder(3);
	vector<string> names = t.inorder();
	vector<string> expectedNames = {"A", "B", "C"};
	REQUIRE(names.size() == 3);
	REQUIRE(names == expectedNames);
	t.removeInorder(2);
	names = t.inorder();
	REQUIRE(names.size() == 2);
	expectedNames = {"A", "B"};
	REQUIRE(names == expectedNames);

	t.remove(40000000);
	t.remove(50000000);
	t.insert("J", 50000000);
	t.insert("F", 30000000);
	t.insert("T", 70000000);
	t.insert("C", 20000000);
	t.insert("V", 90000000);
	t.insert("G", 40000000);
	t.insert("M", 68000000);
	t.insert("A", 10000000);
	t.insert("L", 66000000);
	t.insert("O", 69000000);
	t.insert("U", 75000000);
	t.insert("X", 95000000);

	t.removeInorder(9);
	names = t.inorder();
	expectedNames = {
		"A", "C", "F", "G", "J",
		"K", "L", "M", "O", "U",
		"V", "X"
	};
	REQUIRE(names.size() == 12);
	REQUIRE(names == expectedNames);
}

TEST_CASE("Invalid Commands", "[commands]") {
	vector<string> invalidCommands = {
		"insert \"ValidName\" 5849309281049720183291",
		"insert \"Valid Name\" 5489321a",
		"insert \"Invalid-Name\" 48390182",
		"insert \"Invalid\"Name\" abck3oidja",
		"insnert \"Valid Name\" 39485909",
		"searhc \"Name\"",
		"search 7890831237a",
		"pritnInorder",
		"printPreodrer",
		"printPotsorder",
		"printLevelCoutn",
		"removeInodrer 5",
		"remove 4389928a",
		"remove 320091870",
		"insert \"A11y\" 45679999"
	};

	vector<string> cmds;
	for (string command : invalidCommands) {
		istringstream stream(command);
		REQUIRE(validateCommand(stream, cmds) == "unsuccessful");
	}
	REQUIRE(cmds.size() == 0);
	string valid = "insert \"Gator\" 55554444";
	istringstream stream(valid);
	REQUIRE(validateCommand(stream, cmds) == "successful");
	REQUIRE(cmds.size() == 3);
}
	t.insert("K", 63000000);