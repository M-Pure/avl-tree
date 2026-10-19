#pragma once
#include <algorithm>
#include <stack>
#include <string>
#include <vector>
using namespace std;


class GatorAVL {
private:

    // Nodes are private and cannot be directly accessed outside GatorAVL
    struct Node {
        string name;
        int id;
        Node* left;
        Node* right;
        int height;
        Node(string name, int id) : name(name), id(id), left(nullptr), right(nullptr), height(1) {}

        // Utility function that allows nullptr checks (h = 0) not possible with this keyword
        static int getHeight(Node* node) { return node ? node->height : 0; }

        int getBalance() const { return getHeight(this->left) - getHeight(this->right); }

        // Invoked after a node is inserted or rotated
        void updateHeight() { this->height = 1 + max(getHeight(this->left), getHeight(this->right)); }

        /* ======= ROTATIONS =======
         * Rotations update height of the imbalanced parent and its child (new parent)
         * Returns the post-rotation parent (former child) for pointer correction */

        Node* rotateRight() { // LL unbalanced parent, or right child of RL unbalanced parent
            Node* temp = this->left->right;
            Node* newParent = this->left;
            this->left->right = this;
            this->left = temp;
            // New child (former parent) height influences parent height; must update first
            this->updateHeight();
            newParent->updateHeight();
            return newParent;
        }

        Node* rotateLeft() { // RR unbalanced parent, or left child of LR unbalanced parent
            Node* temp = this->right->left;
            Node* newParent = this->right;
            this->right->left = this;
            this->right = temp;
            // New child (former parent) height influences parent height; must update first
            this->updateHeight();
            newParent->updateHeight();
            return newParent;
        }

    };

    // Members
    Node* root;
    vector<int> idList;

    // Helper functions

    /* =========== INSERT =========== */
    Node* insertHelp(Node* curr, string name, int id);
    Node* balance(Node* internalNode);

    /* =========== REMOVE =========== */
    void getPath(Node* curr, int id, stack<Node*>& path) const;
    Node* inorderSuccessor(Node* node) const;
    void deleteNoChildren(stack<Node*>& path);
    void deleteOneChild(stack<Node*>& path);
    void deleteTwoChildren(stack<Node*>& path);

    /* =========== TRAVERSE =========== */
    void inorderNodes(Node* curr, vector<Node*>& nodes) const;
    void inorderHelp(Node* curr, vector<string>& names) const;
    void preorderHelp(Node* curr, vector<string>& names) const;
    void postorderHelp(Node* curr, vector<string>& names) const;

public:
    // Default constructor = empty tree
    GatorAVL() : root(nullptr) {}

    // Destructor to postorder deallocate dynamic tree nodes
    void destructorHelper(Node* curr);
    ~GatorAVL() { destructorHelper(root); }

    /* Intermediary/helper functions made public for Unit Testing */

    /* =========== UTILITY =========== */
    Node* getRoot() const;
    int getLevelCount() const;

    /* =========== SEARCH =========== */
    Node* searchIDHelp(Node* curr, int id) const;
    void searchNameHelp(Node* curr, string name, vector<int>& matches) const;

    /* =========== TRAVERSE =========== */
    vector<string> inorder() const;
    vector<int> inorderIDs() const;
    vector<string> preorder() const;
    vector<string> postorder() const;


    /* =========== MAIN FUNCTIONALTIY =========== */
    void insert(string name, int id);
    void remove(int id);
    void removeInorder(int n);
    void search(int id) const;
    void search(string name) const;
    void printInorder() const;
    void printPreorder() const;
    void printPostorder() const;
    void printLevelCount() const;

};