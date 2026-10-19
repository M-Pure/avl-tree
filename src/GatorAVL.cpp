#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "GatorAVL.h"
using namespace std;

/* =========== UTILITY =========== */

// Destructor helper: postorder deallocate to avoid dangling pointers and memory leaks
void GatorAVL::destructorHelper(Node* curr) {
    if (curr == nullptr) {
        return;
    }
    destructorHelper(curr->left);
    destructorHelper(curr->right);
    delete curr;
}

// Unit Testing
GatorAVL::Node* GatorAVL::getRoot() const {
    return this->root;
}

// Unit Testing
int GatorAVL::getLevelCount() const {
    return Node::getHeight(root);
}



/* =========== INSERT =========== */

/* Recursively inserts and links starting at curr.
 * Assumes tree is nonempty (tree must have root)
 * Assumes ID is unique - guarantees node will be added for height rebalancing */
GatorAVL::Node* GatorAVL::insertHelp(Node* curr, string name, int id) {
    // Base case: Empty subtree, insert node.
    if (curr == nullptr) {
        cout << "successful" << endl;
        return new Node(name, id);
    }
    else if (id < curr->id) curr->left = insertHelp(curr->left, name, id);
    else if (id > curr->id) curr->right = insertHelp(curr->right, name, id);

    curr->updateHeight(); // Recalculate height BEFORE checking balance
    return balance(curr);
}

/* Invoked on each node in the path of insertion to ensure balance range is in [-1, 1]
 * Returns new parent if a rotation occurs, parameter node otherwise */
GatorAVL::Node* GatorAVL::balance(Node* internalNode) {
    int parentBalance = internalNode->getBalance();
    if (parentBalance >= -1 && parentBalance <= 1) return internalNode; // No balance needed, range is in [-1, 1]

    /* Balance will never be < -2 or > 2 because a rotation will happen at -2 or 2 to fix it
     * Double rotations (LR, RL) require parent of rotated parent to adjust left/right to
     * new parent (former child) before the second rotation takes place */

    if (parentBalance == 2) { // LL or LR alignment; child guaranteed on left
        int childBalance = internalNode->left->getBalance();

        if (childBalance == 1) { // LL, right rotate
            return internalNode->rotateRight();
        }
        // (childBalance == -1)
        else  { // LR, left rotate then right rotate
            internalNode->left = internalNode->left->rotateLeft();
            return internalNode->rotateRight();
        }
    }
    // (parentBalance == -2)
    else  { // RR or RL alignment; child guaranteed on right
        int childBalance = internalNode->right->getBalance();

        if (childBalance == -1) { // RR, left rotate
            return internalNode->rotateLeft();
        }
        // (childBalance == 1)
        else { // RL, right rotate then left rotate
            internalNode->right = internalNode->right->rotateRight();
            return internalNode->rotateLeft();
        }
    }
}

/* Guarantees nonempty tree, unique id for recursive helper function
 * Adds unique id to class idList for record
 * Updates root if it is rotated after an insertion */
void GatorAVL::insert(string name, int id) {
    // Edge case: Tree is empty. insertHelp requires the tree be nonempty
    if (root == nullptr) {
        cout << "successful" << endl;
        root = new Node(name, id);
        idList.push_back(id);
        return;
    }
    // Edge case: id already exists in AVL tree - no insertion nor rebalancing
    for (int number : idList) {
        if (id == number) {
            cout << "unsuccessful" << endl;
            return;
        }
    }
    /* Nonempty tree, unique ID, insertHelp assumptions satisfied
     * and the new node will be inserted */
    idList.push_back(id);
    root = insertHelp(root, name, id);
}



/* =========== REMOVE =========== */

/* Recursively searches for node with matching id, pushing touched nodes
 * to the output stack until found.
 * path's topmost value will be the node with given id, nullptr if no match was found
 * Assumes parameter curr is the root of the tree for a full tree search */
void GatorAVL::getPath(Node* curr, int id, stack<Node*>& path) const {
    path.push(curr);

    // Base case: DNE or found, end recursion
    if (curr == nullptr || id == curr->id) {
        return;
    }

    if (id < curr->id) {
        getPath(curr->left, id, path);
    }
    if (id > curr->id) {
        getPath(curr->right, id, path);
    }
}

/* Returns inorder successor of parameter node.
 * Assumes the parameter node is not null */
GatorAVL::Node* GatorAVL::inorderSuccessor(Node* node) const {
    /* Inorder successor: Right subtree's leftmost child */
    Node* curr = node->right; // Start at right subtree
    while (curr->left != nullptr) { // Continue left as far as possible
        curr = curr->left;
    }
    return curr; // Cannot go any further left
}

/* Delete helpers assume path contains a valid node to be deleted
 * remove() verifies that a matching node was found before calling them */
void GatorAVL::deleteNoChildren(stack<Node*>& path) {
    Node* delNode = path.top();
    if (delNode == root) { // Edge case: deleting sole root node
        delete delNode;
        root = nullptr;
        return;
    }
    // If deleted node is not the root, it must have a parent
    path.pop();
    Node* parent = path.top();
    if (delNode == parent->left) { // Deleted node is parent's left child
        delete delNode;
        parent->left = nullptr;
    }
    else { // Deleted node is parent's right child
        delete delNode;
        parent->right = nullptr;
    }
    // Update heights back to root
    while (!path.empty()) {
        path.top()->updateHeight();
        path.pop();
    }
}

void GatorAVL::deleteOneChild(stack<Node*>& path) {
    Node* delNode = path.top();
    // Is the one child on the left or right?
    Node* child = delNode->left ? delNode->left : delNode->right;
    if (delNode == root) {
        delete delNode;
        root = child;
        return;
    }
    // If deleted node is not the root, it must have a parent
    path.pop();
    Node* parent = path.top();
    // Is the deleted node the left or right child of its parent?
    if (delNode == parent->left) {
        delete delNode;
        parent->left = child;
    }
    else { // Deleted node is right child of it's parent
        delete delNode;
        parent->right = child;
    }
    // Update heights back to root
    while (!path.empty()) {
        path.top()->updateHeight();
        path.pop();
    }
}

void GatorAVL::deleteTwoChildren(stack<Node*>& path) {
    Node* delNode = path.top();
    Node* successor = inorderSuccessor(delNode);
    // Store successor data before deleting it
    string successorName = successor->name;
    int successorID = successor->id;
    // Delete successor - can only have 0 or 1 child, or else it is not a successor
    remove(successor->id);
    /* Changing delNode data before removing successor causes remove
     * to delete delNode instead of the successor */
    delNode->name = successorName;
    delNode->id = successorID;
    // To restore the successorID to the list of IDs in tree
    idList.push_back(successorID);
}

void GatorAVL::remove(int id) {
    stack<Node*> path;
    getPath(root, id, path);
    Node* nodeToDelete = path.top();
    if (nodeToDelete == nullptr) { // id does not exist
        cout << "unsuccessful" << endl;
        return;
    }
    // id must exist: Find and remove it from private tree member idList
    for (auto it = idList.begin(); it != idList.end(); ) {
        if (*it == id) {
            it = idList.erase(it);
        }
        else {
            ++it;
        }
    }
    /* --------- Case 1: No children --------- */
    if (nodeToDelete->left == nullptr && nodeToDelete->right == nullptr) {
        deleteNoChildren(path);
        cout << "successful" << endl;
    }
    /* --------- Case 3: 2 children --------- */
    else if (nodeToDelete->left != nullptr && nodeToDelete->right != nullptr) {
        deleteTwoChildren(path);
        // Calls remove(), so success message printed within it to prevent duplicates
    }
    /* --------- Case 2: 1 child --------- */
    else {
        deleteOneChild(path);
        cout << "successful" << endl;
    }
}

/* Gets inorder list of Node* from utility function, passes the id
 * of the nth node to remove() */
void GatorAVL::removeInorder(int n) {
    // Edge case: n is negative
    if (n < 0) {
        cout << "unsuccessful" << endl;
        return;
    }
    vector<Node*> inorderList;
    inorderNodes(root, inorderList);
    /* Valid n value can be at most 1 less than total node count
     * i.e. tree with 5 elements, valid indices [0, 4] */
    if (n >= inorderList.size()) {
        cout << "unsuccessful" << endl;
        return;
    }
    remove(inorderList[n]->id);
}



/* =========== SEARCH =========== */

/* Recursively searches for node with given id, returning
 * node if found and nullptr otherwise */
GatorAVL::Node* GatorAVL::searchIDHelp(Node* curr, int id) const {
    // Base case: DNE or found
    if (curr == nullptr || id == curr->id) {
        return curr;
    }
    if (id < curr->id) {
        return searchIDHelp(curr->left, id);
    }
    return searchIDHelp(curr->right, id);
}

void GatorAVL::search(int id) const {
    Node* result = searchIDHelp(root, id);
    if (result) {
        cout << result->name << endl;
    }
    else {
        cout << "unsuccessful" << endl;
    }
}

/* Preorder traverses tree looking for name matches, duplicates allowed.
 * Pushes all matches to output vector in the order they are found */
void GatorAVL::searchNameHelp(Node* curr, string name, vector<int>& matches) const {
    // End subtree recursion
    if (curr == nullptr) {
        return;
    }
    if (name == curr->name) {
        matches.push_back(curr->id);
    }
    searchNameHelp(curr->left, name, matches);
    searchNameHelp(curr->right, name, matches);
}

void GatorAVL::search(string name) const {
    vector<int> matches;
    searchNameHelp(root, name, matches);
    if (matches.size() == 0) {
        cout << "unsuccessful" << endl;
    }
    for (int match : matches) {
        cout << match << endl;
    }
}



/* =========== TRAVERSE =========== */

/* Helpers for printing functions that recursively traverse tree,
 * adding names to output vector starting at curr (root)
 * preorder: NLR
 * inorder: LNR
 * postorder: LRN    */

void GatorAVL::inorderHelp(Node* curr, vector<string>& names) const {
    // Base case: empty tree/subtree
    if (curr == nullptr) {
        return;
    }
    /* =======  Inorder = Left, Node, Right  ======= */
    inorderHelp(curr->left, names);
    names.push_back(curr->name);
    inorderHelp(curr->right, names);
}

void GatorAVL::inorderNodes(Node* curr, vector<Node*>& nodes) const {
    if (curr == nullptr) {
        return;
    }
    inorderNodes(curr->left, nodes);
    nodes.push_back(curr);
    inorderNodes(curr->right, nodes);
}

void GatorAVL::preorderHelp(Node* curr, vector<string>& names) const {
    if (curr == nullptr) {
        return;
    }
    /* =======  Preorder = Node, Left, Right  ======= */
    names.push_back(curr->name);
    preorderHelp(curr->left, names);
    preorderHelp(curr->right, names);
}

void GatorAVL::postorderHelp(Node* curr, vector<string>& names) const {
    if (curr == nullptr) {
        return;
    }
    /* ======= Postorder = Left, Right, Node ======= */
    postorderHelp(curr->left, names);
    postorderHelp(curr->right, names);
    names.push_back(curr->name);
}


/* Return vector of pre/in/postorder names using recursive helper functions */

vector<string> GatorAVL::inorder() const {
    vector<string> names;
    inorderHelp(root, names);
    return names;
}

vector<int> GatorAVL::inorderIDs() const {
    vector<Node*> nodes;
    inorderNodes(root, nodes);
    vector<int> ids;
    for (Node* node : nodes) {
        ids.push_back(node->id);
    }
    return ids;
}

vector<string> GatorAVL::preorder() const {
    vector<string> names;
    preorderHelp(root, names);
    return names;
}

vector<string> GatorAVL::postorder() const {
    vector<string> names;
    postorderHelp(root, names);
    return names;
}



/* =========== PRINT =========== */

/* Prints list of pre/in/postorder names */
void GatorAVL::printInorder() const {
    vector<string> names = inorder();
    if (names.size() == 0) { // Empty tree
        return;
    }
    cout << names[0]; // To prevent an extra comma on last name printed
    for (unsigned int i = 1; i < names.size(); ++i) {
        cout << ", " << names[i];
    }
    cout << endl;
}

void GatorAVL::printPreorder() const {
    vector<string> names = preorder();
    if (names.size() == 0) {
        return;
    }
    cout << names[0];
    for (unsigned int i = 1; i < names.size(); ++i) {
        cout << ", " << names[i];
    }
    cout << endl;
}

void GatorAVL::printPostorder() const {
    vector<string> names = postorder();
    if (names.size() == 0) {
        return;
    }
    cout << names[0];
    for (unsigned int i = 1; i < names.size(); ++i) {
        cout << ", " << names[i];
    }
    cout << endl;
}

void GatorAVL::printLevelCount() const {
    cout << getLevelCount() << endl;
}