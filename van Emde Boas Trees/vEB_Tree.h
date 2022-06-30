#pragma once
#include <iostream>
#include <cmath>

//van Emde Boas Tree
class vEB_Tree
{
private:
	int universe_size_power;            //Range of elements stored (universe size) is a power of 2, this variable represents the exponent: 
	                                   //universe_size = 2^universe_size_power.
	
	int min;                          //Minimum Stored Element.
	int max;                         //Maximum Stored Element


	vEB_Tree* summary;              //A van Emde Boas structure that can inform us of which clusters are empty and which are not.
	vEB_Tree** clusters;           //An array of cluster pointers, which are van Emde Boas structure that recursively store our elements. 

	//We can imagine that all elements are stored in the leaves (at the lowest clusters in our recursive structure).

	//------------------------------------------------------------------------------------------------------------------------
	
	//Utility Functions.
	int high(int) const;                         //Returns the index of the cluster storing the input integer.
	
	int low(int) const;                         //Returns the value of the input integer relative to its cluster.
	
	int index(int, int) const;                 //Given the index of a cluster and the value of an integer relative to that cluster,
	                                          //outputs the true value of the integer.

	//-------------------------------------------------------------------------------------------------------------------------

	//Recursive Functions.
	bool rec_isMember(vEB_Tree* , int);                //Checks whether a given element is stored in the vEB_Tree or not.

	int rec_successor(vEB_Tree*, int);                //Finds the successor of a given element within the vEB_Tree.

	int rec_predecessor(vEB_Tree*, int);             //Finds the predecessor of a given element withing the vEB_Tree.

	void rec_insert(vEB_Tree*, int);                //Inserts a new element into the vEB_Tree.

	void rec_delete(vEB_Tree*, int);               //Deletes an exisiting element from the vEB_Tree.

public:

	//Constructor and Destructor.
	vEB_Tree(int);
	~vEB_Tree();

	//-----------------------------------------------------------------------------------------------------------------------

	//Interface:

	//Getters for min and max.
	int getMin() const;
	int getMax() const;
	
	//These functions call their corresponding recursive versions above.
	bool isMember(int);
	int successor(int);
	int predecessor(int);
	void insert(int);
	void delete_element(int);

	//Prints the stored elements.
	void print();

};

