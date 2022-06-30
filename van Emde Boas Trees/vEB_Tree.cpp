#include "vEB_Tree.h"

vEB_Tree::vEB_Tree(int u_power) {

	universe_size_power = u_power;
	min = max = -1;

	if (u_power == 1) {                                 //Base Case: universe_size == 2.

		summary = nullptr;                             //No clusters/summary are stored in the base case.
		clusters = nullptr;

	}
	else {

		int clusters_num_power = ceil(u_power / 2);
		int cluster_size_power = floor(u_power / 2);

		summary = new vEB_Tree(clusters_num_power);                   //Create the summary, which is a vEB_Tree of size sqrt(universe_size).
		clusters = new vEB_Tree * [pow(2, clusters_num_power)];      
		for (int i = 0; i < pow(2, clusters_num_power); i++) {       //Create sqrt(universe_size) clusters, each of size sqrt(universe_size).

			clusters[i] = new vEB_Tree(cluster_size_power);

		}

	}

}

vEB_Tree::~vEB_Tree()                              //Deletes all dynamically allocated data.
{
	delete summary;

	if (universe_size_power == 1) {
		
		delete clusters;
		return;
	
	}

	for (int i = 0; i < pow(2, ceil(universe_size_power / 2)); i++) {
		
		delete clusters[i];
	
	}

	delete clusters;
}

//---------------------------------------------------------------------------------------------------------------------

//Utility Functions:

int vEB_Tree::high(int x) const {

	return x / (int)pow(2, floor(universe_size_power / 2));           //Returns the index of the cluster storing x.

}

int vEB_Tree::low(int x) const {

	return x % (int)pow(2, floor(universe_size_power));            //Returns the value of x relative to its cluster.

}

int vEB_Tree::index(int i, int j) const {

	return i * pow(2, floor(universe_size_power / 2)) + j;          //Reconstructs a value from its cluster index and relative position.

}

//-----------------------------------------------------------------------------------------------------------------------

//Recursive Functions (all run in O(log(log(n)))):

bool vEB_Tree::rec_isMember(vEB_Tree* vEB, int x)
{

	if (x == vEB->getMin() || x == vEB->getMax()) return true;            //Base Case.
	else if (vEB->universe_size_power == 1) return false;                //If the cluster has a size of 2, and x is not equal to max and min,
	                                                                    //then x is not stored in the vEB structure.

	vEB_Tree* targetCluster = vEB->clusters[vEB->high(x)];            
	return rec_isMember(targetCluster, targetCluster->low(x));	       //Recurse on the cluster that should contain x.

}

int vEB_Tree::rec_successor(vEB_Tree* vEB, int x)
{

	if (x < vEB->getMin()) return vEB->getMin();               //Base Cases.
	if (vEB->universe_size_power == 1) {

		if (x == 0 && vEB->getMax() == 1) return 1;
		else return -1;

	}

	int targetCluster_index = vEB->high(x);
	int target = 0;
	vEB_Tree* targetCluster = vEB->clusters[targetCluster_index];

	//If the cluster containing x has an element larger than x, then recurse on it. Otherwise, recurse on the summary to find the
	//next non-empty cluster.
	if (targetCluster->low(x) < targetCluster->getMax()) target = rec_successor(targetCluster, targetCluster->low(x));
	else {

		targetCluster_index = rec_successor(vEB->summary, targetCluster_index);
		
		//If we find no non-empty clusters, then no successor exists for x.
		if (targetCluster_index == -1) return -1;

		targetCluster = vEB->clusters[targetCluster_index];
		target = targetCluster->getMin();

	}

	return vEB->index(targetCluster_index, target);
}

int vEB_Tree::rec_predecessor(vEB_Tree* vEB, int x)              //Symmetric to rec_successor.
{

	if (x > vEB->getMax()) return vEB->getMax();
	if (vEB->universe_size_power == 1) {

		if (x == 1 && vEB->getMin() == 0) return 0;
		else return -1;

	}

	int targetCluster_index = vEB->high(x);
	int target = 0;
	vEB_Tree* targetCluster = vEB->clusters[targetCluster_index];

	if (targetCluster->low(x) > targetCluster->getMin()) target = rec_predecessor(targetCluster, targetCluster->low(x));
	else {

		targetCluster_index = rec_predecessor(vEB->summary, targetCluster_index);

		if (targetCluster_index == -1) {
			//Extra Check, since the minimum is not stored recursively, it needs to be directly compared to x.
			if (x > vEB->getMin()) return vEB->getMin();      
			else return -1;
		}

		targetCluster = vEB->clusters[targetCluster_index];
		target = targetCluster->getMax();

	}

	return vEB->index(targetCluster_index, target);

}

void vEB_Tree::rec_insert(vEB_Tree* vEB, int x)
{

	if (vEB->getMin() == -1) {                 //Base Case: Empty Tree.
		vEB->min = x;
		vEB->max = x;
		return;
	}

	if (x < vEB->getMin()) {                //Minimum element is not stored recursively. If we insert an element smaller than the min,
		                                   //then swap them.

		int temp = vEB->getMin();
		vEB->min = x;
		x = temp;


	}

	if (vEB->universe_size_power > 1) {                         

		vEB_Tree* targetCluster = vEB->clusters[vEB->high(x)];

		if (targetCluster->getMin() == -1) rec_insert(vEB->summary, vEB->high(x));     //If the cluster was empty before insertion,
		                                                                              //update the summary.
		rec_insert(targetCluster, targetCluster->low(x));                   //Recurse on the cluster that should contain x.

	}

	if (x > vEB->getMax()) vEB->max = x;                     //Update max if needed.
}

void vEB_Tree::rec_delete(vEB_Tree* vEB, int x)
{

	if (vEB->getMax() == vEB->getMin()) {               //Base Case: only 1 elements was stored.
		vEB->min = -1;
		vEB->max = -1;
		return;
	}

	if (vEB->universe_size_power == 1) {              //Base Case: vEB structure has universe_size = 2, and contains 2 elements.
		vEB->min = x == 0 ? 1 : 0;
		vEB->max = vEB->getMin();
		return;
	}

	//Minimum is not stored recursively. If we delete it, then it should be replaced by the next largest element, and that element
	//should then be deleted from its cluster.
	if (x == vEB->getMin()) {
		int min_cluster_index = vEB->summary->getMin();
		x = vEB->clusters[min_cluster_index]->getMin();
		x = vEB->index(min_cluster_index, x);
		vEB->min = x;
	}

	vEB_Tree* targetCluster = vEB->clusters[vEB->high(x)];
	rec_delete(targetCluster, targetCluster->low(x));              //Recursively delete x from its cluster.
	if (targetCluster->getMin() == -1) rec_delete(vEB->summary, vEB->high(x));       //If the cluster is now empty, update the summary.

	if (x == vEB->getMax()) {

		int max_cluster_index = vEB->summary->getMax();
		if (max_cluster_index == -1) vEB->max = vEB->getMin();              //vEB_Tree only contains one element.
		else {
			targetCluster = vEB->clusters[max_cluster_index];
			x = targetCluster->getMax();                                   //Find the new largest element.
			x = vEB->index(max_cluster_index, x);
			vEB->max = x;                                                 //Update max.
		}

	}

}

//---------------------------------------------------------------------------------------------------------------------------

//Interface:

int vEB_Tree::getMin() const {

	return min;

}

int vEB_Tree::getMax() const {

	return max;

}

bool vEB_Tree::isMember(int x)
{
	return rec_isMember(this, x);
}

int vEB_Tree::successor(int x) {

	return rec_successor(this, x);

}

int vEB_Tree::predecessor(int x) {

	return rec_predecessor(this, x);

}

void vEB_Tree::insert(int x) {

	if (isMember(x)) return;

	rec_insert(this, x);

}

void vEB_Tree::delete_element(int x) {

	if (!isMember(x)) return;

	rec_delete(this, x);

}

void vEB_Tree::print() {

	std::cout << "[\t";

	for (int i = 0; i < pow(2, universe_size_power); i++) {

		if (isMember(i)) std::cout << i << '\t';

	}

	std::cout << ']' << std::endl;

}
