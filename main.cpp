#include"RB_Tree.h"
#include<vector>
#include<iostream>
#include"check.h"

using namespace std;

int main() {
	RBTree<type> tree;
	
	vector<type> tmp1{ 0,1,2,3,4,5,6,7,8,9 };
	for (int i = 0; i < tmp1.size(); ++i) {
		tree.insert(tmp1[i]);
	}
	tree.erase(0);
	tree_check(tree.root);

	vector<type> tmp2{ 2,6,15,10,9,12,18,20  };
	for (int i = 0; i < tmp2.size(); ++i) {
		tree.insert(tmp2[i]);
		cout << tmp2[i] << " :" << std::endl;
		tree_check(tree.root);

	}

	tree.erase(2);
	tree_check(tree.root);
	tree.erase(15);
	tree_check(tree.root);
	tree.erase(6); 
	tree_check(tree.root);
	return 0;
}