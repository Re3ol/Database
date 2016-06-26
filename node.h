#pragma once
#include <iostream>
#include <string>
using namespace std;

class node {
private:
	//Your private stuff to implement the B Tree
	string* values;
	string* keys;
	node** children;
	int size;
	int numElements;
	bool leaf;  //variable to track if a node is a leaf
public:
	//Constructor
	node(int sizeOfNodes, bool leaf);

	//Destructor
	~node();

	//Inserts the value pair into the tree
	bool insert(string key, string value);

	// Searches for the key in the tree, if found, it returns
	// true else returns false.
	// If the key is found, it stores the value in the *value variable
	bool find(string key, string *value, int &valCount);

	//Searches for the key and deletes it. Returns true if deletion was
	//successful, returns false if the key wasn't found
	bool delete_key(string key);

	// concatenates the contents of the tree from an inorder traversal
	// into an output stream with each string item followed by a
	// newline character in the stream
	string toStr();

	//A function to split the child x of this node. i is the index of x in the child array, the child must be full
	void splitChild(int i, node *x);

	//A function that will remove a key and value in the given index position from a leaf
	bool deleteFromLeaf(int index);
	//A function that will remove a key and value in the given index position from a node that is not a leaf
	bool deleteFromNotLeaf(int index);

	//A function that will get the key/value before, where the key is present in the index position of the node
	string getBeforeKey(int index);
	string getBeforeValue(int index);

	//A function that will get the key/value after, where the key is present in the index position of the node
	string getAfterKey(int index);
	string getAfterValue(int index);

	//A function that will be used to fill up the child node present in the index position of the children array
	void fill(int index);

	//A function to borrow a key from the child node before the current child node we are in i.e children[index -1] -> children[index] node
	void getFromPrev(int index);

	//A function to borrow a key from the child node After the current child node we are in i.e children[index +1] -> children[index] node
	void getFromNext(int index);

	//Function to merge child node and index position with the node of index+1 of the parent node
	void merge(int index);

	//Function to quickly find the index position of a key or the next highest index position, good helper method for some of the methods
	int findKey(string key);

	//Allows bTree to access private members of this class in bTree functions
	friend class bTree;
};
