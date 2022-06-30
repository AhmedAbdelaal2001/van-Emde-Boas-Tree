#include "vEB_Tree.h"
using namespace std;

int main() {

	int n;
	int choice;

	cout << "Enter the number of elements you wish to store in the van Emde Boas Tree: ";
	cin >> n;

	n = ceil(log(n) / log(2));
	vEB_Tree vEB(n);
	do {
		
		cout << "\n-----------------------------------------------------------------------------------------------------------------\n";
		cout << "Enter the action you wish to execute: " << endl;
		cout << "1. Print the stored elements. " << endl;
		cout << "2. Insert a new element. " << endl;
		cout << "3. Delete an existing element. " << endl;
		cout << "4. Check whether a certain element is stored or not." << endl;
		cout << "5. Find the successor of an element." << endl;
		cout << "6. Find the predecessor of an element. " << endl;
		cout << "7. Find the maximum stored element. " << endl;
		cout << "8. Find the minimum stored element. " << endl;
		cout << "9. Exit" << endl;
		
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			cout << "Printing the contents of the tree: " << endl;
			vEB.print();
			break;
		}
		case 2: {
			cout << "Enter the element you wish to insert: " << endl;
			cin >> n;
			vEB.insert(n);
			break;
		}
		case 3: {
			cout << "Enter the element you wish to delete: " << endl;
			cin >> n;
			vEB.delete_element(n);
			break;
		}
		case 4: {
			cout << "Enter the element you wish to query on: " << endl;
			cin >> n;
			if (vEB.isMember(n)) cout << "Element is stored." << endl;
			else cout << "Element is not stored." << endl;
			break;
		}
		case 5: {
			cout << "Enter the element you wish to find the successor of: " << endl;
			cin >> n;
			int successor = vEB.successor(n);
			if (successor == -1) cout << "Element " << n << " has no successor." << endl;
			else cout << "The successor of element " << n << " is: " << successor << endl;
			break;
		}
		case 6: {
			cout << "Enter the element you wish to find the predecessor of: " << endl;
			cin >> n;
			int predecessor = vEB.predecessor(n);
			if (predecessor == -1) cout << "Element " << n << " has no predecessor." << endl;
			else cout << "The predecessor of element " << n << " is: " << predecessor << endl;
			break;
		}
		case 7: {
			int max = vEB.getMax();
			if (max == -1) cout << "The tree is empty." << endl;
			else cout << "Maximum element is: " << max << endl;
			break;
		}
		case 8: {
			int min = vEB.getMin();
			if (min == -1) cout << "The tree is empty." << endl;
			else cout << "Minimum element is: " << min << endl;
			break;
		}
		case 9: {
			break;
		}
		default:
			cout << "Unrecognized Input. Please Try Again." << endl;
		}
	
	} while (choice != 9);

	return 0;
}