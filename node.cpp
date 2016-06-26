
#include "node.h"

//
//Constructor
//
node::node(int sizeOfNodes, bool leaf){
	this->size = sizeOfNodes;
	this->leaf = leaf; //is the node a leaf
	numElements = 0; //A variable to track the number of elements in the node
	values = new string[2*sizeOfNodes]; //An array that will hold the values
	keys = new string[2*sizeOfNodes]; //An Array that will hold the keys
	children = new node*[2*sizeOfNodes + 1]; //An Array that will hold the children of the node

}

//
//Deconstructor
//
node::~node(){
	delete[] values;
	delete[] keys;
	delete[] children;
}

//
//Inserts the value pair into the tree
//Parameter:
//key: the key need inserted.
//value: the value need inserted.
//
bool node::insert(string key, string value){
	//first we will initialize as the index of the rightmost element so it becomes easy to move over smaller elements
	int i = numElements - 1;

	//Next we check if this node is a leaf node
	if(leaf == true)
	{
		//find the location of the new key to be inserted and move all the greater keys ahead
		while( i >= 0 && keys[i] > key){
			keys[i+1] = keys[i]; //move the key over to the new index position
			values[i+1] = values[i]; //move the value over to the new index position
			i--;
		}
		//Last we have to insert the new key and value into the new found location
		keys[i+1] = key;
		values[i+1] = value;
		numElements++; //increments numElements by 1 ( + 1)
		return true;
	}
	else{ //in the case that this node is not a leaf

		//first it will find the child the new key should be inserted too
		while(i >= 0 && keys[i] > key){
			i--; //decrement i
		}

		//Next we need to check if the child is full, if it is we need to split it
		if(children[i+1]->numElements == 2*size - 1){
			splitChild(i+1, children[i+1]);
			//now to check which of the children of the new split node gets the key
			if(keys[i + 1] < key){
				i++;
			}
		}
		return children[i+1]->insert(key,value);
	}
}

//
//Searches for the key in the tree
//Parameter:
//key: the key need find.
//value: the string of result found.
//
bool node::find(string key, string *value, int &valCount){
	//First it looks for a key that is either greater than or equal to key
	int i = 0;
	int flag = 0;

	while(i < numElements && key > keys[i]){
		i++;
	}
	flag = i;

	//if key is found return true and stores the value in  *value
	/*
	if(i == size){ //note i went out of bounds have to put it back in bounds
		//cout << numElements << endl;
		//cout<< i << endl;
		i = i -1;
	}
	*/

	if(keys[i] == key && i < numElements ){
		//cout<< "the keyi is: " + keys[i] << endl;
		//cout<< "the valuei is: " + values[i] << endl;
		while (i < numElements) {
			if (keys[i] == key) {
				value[valCount] = values[i];
				valCount++;

				flag = i;
			}
			i++;	
		}

		if( leaf == true)
			return true;

		else
			return children[flag]->find(key, value, valCount);
	}
	if(leaf == true){ //if this is a leaf and it was not found then return false
		return false;
	}
	//Since this is not a leaf then go to the correct child node and find it
	return children[i]->find(key, value, valCount);
}

//
//Searches for the key and deletes it.
//Parameter:
//key:the key need deleted.
//
bool node::delete_key(string key){
	int index = findKey(key); // first lets find the index of the key we want to delete

	//if the key to be removed is in this node
	if(index < numElements && keys[index] == key){
		//if node is a a leaf node deleteFromLeaf is called, otherwise deleteFromNotLeaf is called
		if(leaf){
			return deleteFromLeaf(index);
		}
		else{
			return deleteFromNotLeaf(index);
		}
	}
	else{
		//If this is a leaf node, then the key must not be present so return false
		if(leaf){
			return false;
		}
		//If not it means the key to be deleted is present in the sub-tree rooted with this node
		//Now we will use bool present to indicate whether the key is present in the sub-tree rooted with the last child of this node
		bool present;
		if(index == numElements){
			present = true;
		}
		else{
			present = false;
		}

		//if the child where the key is present has less than a size + 1 / 2 elements then we need to fill it
		//int nearHalf = (size + 1) / 2;
		if(children[index]->numElements < size){
			fill(index); //we fill that node
		}

		//If the last child was merged, then it was merged with the previous child, meaning we have to be recurse on the (index - 1) child, or we can do it on the filled child
		if(present && index > numElements){
			return children[index - 1]->delete_key(key);
		}
		else{
			return children[index]->delete_key(key);
		}

	}
	return false;
}

//
// concatenates the contents of the tree from an inorder traversal
// into an output stream with each string item followed by a
// newline character in the stream
//
string node::toStr(){
	int i; //index counter
	string returnStr = ""; //return string
	for(i = 0; i < numElements; i++){
		//first checks if this a leaf, if it is not it goes to the children node first, and calls its toStr method
		if (leaf == false){
			returnStr = returnStr + children[i]->toStr();
		}
		returnStr = returnStr + keys[i] + " " + values[i] + "\n"; //gets the keys in the children
	}

	if(leaf == false){ //to print out the last child since we have size + 1 children
		returnStr = returnStr + children[i] -> toStr();
	}
	return returnStr;
}

//
//Split the child node.
//Parameter:
//i:the position 
//x:the node need to be split.
//Please note that node x must be full when this method is called or it will cause problems
void node::splitChild(int i, node *x){

	//First we have to create a new node
	node *y = new node(x->size, x->leaf);
	//variable to hold the splitting point
	/*
	int splitP;
	if(size%2 == 0){
		splitP = size/2;
	}
	else{
	   splitP = (size+1)/2;
	}
	*/
	y->numElements = size - 1; //it will take about half of the elements

	//copy the greater keys of x into y, use j instead of i since i is already being used
	for(int j= 0; j < size - 1; j++){
		y->keys[j] = x->keys[j + size]; //transfers over the keys
		y->values[j] = x->values[j + size]; //transfers over the values
	}

	//if x is is not a leaf then we copy over the last half children of x
	if(x->leaf == false){
		for(int j = 0; j < size; j++){
			y->children[j] = x->children[j+size]; //copy operation of the children
		}
	}

	//Next we must reduce the element counter in node x
	x->numElements = size - 1;

	//Next we shift the children of the current node to create space
	for(int j = numElements; j >= i+1; j--){
		children[j+1] = children[j];
	}
	children[i+1] = y; //we now link the new child to the node

	//last we need to move a key of x to the current node and find the location of a new key
	for(int j = numElements -1; j >= i; j--){
		keys[j+1] = keys[j]; //move all the keys and values one space ahead
		values[j+1] = values[j];
	}
	//To end the split operation we need to copy the middle key/value of x to this node and increment numElements of this node
	keys[i] = x->keys[size - 1];
	values[i] = x->values[size - 1];
	numElements++;
}

//
//Delete the item from a leaf node.
//
bool node::deleteFromLeaf(int index){
	//First we have to move all the keys and values after the index position one place backward
	for(int i = index + 1; i < numElements; i++){
		keys[i - 1] = keys[i];
		values[i - 1] = values[i];
	}
	numElements = numElements - 1; //decrement numElements
	return true;
}

//
//Delete the item from a node which is not leaf.
//
bool node::deleteFromNotLeaf(int index){
	string key = keys[index];
	//int nearHalf = (size + 1) / 2;

	//first we will see if the child that precedes the key in children[index] has at least nearHalf keys
	if(children[index]->numElements >= size){
		string beforeKey = getBeforeKey(index); //if it does then we will find its predecessor in the subtree
		string beforeValue = getBeforeValue(index);
		keys[index] = beforeKey; //replace key with before key
		values[index] = beforeValue; //replace value with before value
		return children[index]->delete_key(beforeKey); //Recursively delete before in children[index]
	}

	//if the child children[index] did not pass the earlier if statement then we will attempt it with children[index +1], and use after method instead of before
	else if(children[index + 1]->numElements >= size){
		string afterKey = getAfterKey(index); //if it does then we will find its successor in the subtree
		string afterValue = getAfterValue(index);
		keys[index] = afterKey; //replace key with the after key
		values[index] = afterValue; //replace value with the after value
		return children[index + 1]->delete_key(afterKey); //Recursively delete after in children[index]
	}

	//Else if both children[index] and children[index + 1] are too small, then we will merge them into children[index]
	else{
		merge(index);
		return children[index]->delete_key(key); //recursively deletes k from children[index]
	}
	return false; //reached the end without deleting anything
}

//
//Get the front key of index.
//
string node::getBeforeKey(int index){

	//Keep going down the right most node until a leaf has been reached
	node *current = children[index];
	while(!current->leaf){
		current = current->children[current->numElements];
	}
	//return the last key of the current leaf
	return current->keys[current->numElements-1];

}

//
//Get the front value of index.
//
string node::getBeforeValue(int index){

	//Keep going down the right most node until a leaf has been reached
	node *current = children[index];
	while(!current->leaf){
		current = current->children[current->numElements];
	}
	//return the last value of the current leaf
	return current->values[current->numElements-1];

}

//
//Get the after key of index.
//
string node::getAfterKey(int index){
	//Keep going down the left most node until a leaf has been reached
	node *current = children[index + 1];
	while(!current->leaf){
		current = current->children[0];
	}
	//return the first key of the current leaf
	return current->keys[0];
}

//
//Get the after value of index.
//
string node::getAfterValue(int index){
	//Keep going down the left most node until a leaf has been reached
	node *current = children[index + 1];
	while(!current->leaf){
		current = current->children[0];
	}
	//return the first key of the current leaf
	return current->values[0];
}

//
//fill up the node in the index.
void node::fill(int index){
	//int nearHalf = (size + 1) / 2;
	//If the previous child (index - 1) has more than nearHalf -1 keys, get a key from that child
	if(index != 0 && children[index - 1]->numElements >= size){
		getFromPrev(index);

	}
	//If the next child (index + 1) has more than nearHalf -1 keys, get a key from that child
	else if(index != numElements && children[index + 1]->numElements >= size){
		getFromNext(index);

	}
	//else we need to merge children[index] with one of its siblings
	else{ //if the child is the last child, merge it with the sibling before, other wise merge it with its next sibling
		if(index != numElements){
			merge(index);
		}
		else{
			merge(index - 1);
		}
	}
	return; //incase of a debug error
}

//
//Get the item in the previous position.
//
void node::getFromPrev(int index){

	node *child = children[index];
	node *sibling = children[index - 1];

	//The last key from the child i.e. children[index - 1] goes up to the parent and key [index - 1]
	//the parent is inserted as the first key in the child, and the sibling loses one key and child gains a key

	//for loop to move all the keys and values in children[index] one ahead
	for(int i = child->numElements - 1; i>= 0; --i){
		child->keys[i + 1] = child->keys[i];
		child->values[i + 1] = child->values[i];
	}
	//if child node is not a leaf, increment all the child points by one
	if(!child->leaf){
		for(int i = child->numElements; i >= 0; --i){
			child->children[i+1] = child->children[i];
		}
	}

	child->keys[0] = keys[index - 1]; //setting childs first key to the current nodes key in index - 1 position
	child->values[0] = values[index - 1]; //similar to the above line except for values

	//Moving sibilings's last child as children[index] first child
	if(!child->leaf){
		child->children[0] = sibling->children[sibling->numElements];
	}

	//Last step will be to move the keys and values from the sibling to the parent and properly increment/decrement their respective numElements
	keys[index - 1] = sibling->keys[sibling->numElements - 1];
	values[index - 1] = sibling->values[sibling->numElements - 1];

	child->numElements += 1;
	sibling->numElements -= 1;

	return; //incase for Debug
}

//
//Get the item in the next position.
//
void node::getFromNext(int index){
	node *child = children[index];
	node *sibling = children[index + 1];

	//keys and values at the index position is inserted as the last key in children
	child->keys[(child->numElements)] = keys[index];
	child->values[(child->numElements)] = values[index];

	//Siblings first child is put in as the last child
	if(!child->leaf){
		child->children[(child->numElements) + 1] = sibling->children[0];
	}

	//Now we put the first key and values from sibling into the current nodes key and values at index position
	keys[index] = sibling->keys[0];
	values[index] = sibling->values[0];

	//We have to decrement all of the keys and values in sibling
	for(int i = 1; i < sibling->numElements; ++i){
		sibling->keys[i-1] = sibling->keys[i];
		sibling->values[i-1] = sibling->values[i];
	}
	//Last step will be to move the child points one step behind if sibling is a leaf and properly increment/decrement their respective numElements

	if(!sibling->leaf){
		for(int i=1; i <= sibling->numElements; ++i){
			sibling->children[i-1] = sibling->children[i];
		}
	}

	child->numElements += 1;
	sibling->numElements -= 1;

	return; //incase for Debug

}

//
//Merge two nodes into one node.
//
void node::merge(int index){
	node *child = children[index];
	node *sibling = children[index + 1];

	//int nearHalf = (size + 1) / 2;

	//Getting a keys and values from the current node and inserting into nearhalf -1 index position of child
	child->keys[size - 1] = keys[index];
	child->values[size - 1] = values[index];

	//Copying the keys and values from Sibling to child
	for(int i = 0; i < sibling->numElements; ++i){
		child->keys[i + size] = sibling->keys[i];
		child->values[i + size] = sibling->values[i];
	}

	//Copying the child points from the sibling to the child
	if(!child->leaf){
		for(int i = 0; i <= sibling->numElements; ++i){
			child->children[i + size] = sibling->children[i];
		}
	}

	//Move the keys and values from the index in the current node one step before to fill the gap created from moving the keys and values to the child
	for(int i = index + 1; i < numElements; ++i){
		keys[i-1] = keys[i];
		values[i-1] = values[i];
	}

	//Move the child points after index + 1 in the current node one step before
	for(int i = index + 2; i <= numElements; ++i){
		children[i-1] = children[i];
	}

	//Properly decrementing/incrementing numElements in the current node and child
	child->numElements += sibling->numElements + 1;
	numElements--;

	//perform some memory freeing since we no longer need sibling
	delete(sibling);
	return; //incase for debug

}

//
//Find the key which equals to parameter.
//
int node::findKey(string key){
	int index = 0; //this will be the return index

	//While loop to calculate the index position
	while(index < numElements && keys[index] < key){
		++index; //increment index by one
	}
	/*
	if(index == size){
		index = index - 1; //this is in case it does not find it and goes up to numElements
	}
	*/
	return index; //returns the index position
}




