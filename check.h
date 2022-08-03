#include<vector>
#include<iostream>
#include"RB_Tree.h"

using type = int;
using Node = RBTreeNode<type>;
using namespace std;
Node* sroot;
bool _father_check(Node* root) {
	if (!root || root == sroot) return true;
	if (root->father == nullptr || root->father->left != root && root->father->right != root) {
		std::cout << "节点 " << root->val << " 指向的父节点异常" << std::endl;
		return false;
	}
	_father_check(root->left);
	_father_check(root->right);
	return true;
}
bool father_check(Node* root) {
	bool tmark = _father_check(root);
	if (root->father != nullptr) {
		std::cout << "节点 " << root->val << " 指向的父节点异常" << std::endl;
		return false;
	}
	return tmark;
	
}

void _Inorder(Node* root, std::vector<int>& check) {
	if (root == nullptr) return;
	_Inorder(root->left, check);
	check.emplace_back(root->val);
	std::cout << root->val << " ";
	_Inorder(root->right, check);
}
bool Inorder(Node* root) {
	std::vector<int> check;
	_Inorder(root, check);
	std::cout << std::endl;
	for (int i = 0; i < check.size() - 1; ++i) {
		if (check[i] >= check[i + 1]) {
			std::cout << "中序遍历非有序" << std::endl;
			return false;
		}
	}
	return true;
}
bool _ISRBTree(Node* root, int count, int BlackCount) {
	if (root == nullptr) {
		if (count != BlackCount) {
			std::cout << "error：黑色结点的数目不相等" << std::endl;
			return false;
		}
		return true;
	}
	if (root->color == RBTreeColor::Red && root->father->color == RBTreeColor::Red) {
		std::cout << "error：存在连续的红色结点" << std::endl;
		return false;
	}
	if (root->color == RBTreeColor::Black) {
		++count;
	}
	return _ISRBTree(root->left, count, BlackCount) && _ISRBTree(root->right, count, BlackCount);
}
bool ISRBTree(Node* root) {
	if (root == nullptr) {//空树是红黑树
		return true;
	}
	if (root->color == RBTreeColor::Red) {
		std::cout << "error：根结点为红色" << std::endl;
		return false;
	}
	//找最左路径作为黑色结点数目的参考值
	Node* cur = root;
	int BlackCount = 0;
	while (cur) {
		if (cur->color == RBTreeColor::Black)
			BlackCount++;
		cur = cur->left;
	}
	int count = 0;
	return _ISRBTree(root, count, BlackCount);
}
void tree_check(Node* root) {
	sroot = root;
	if (Inorder(root)) { //检查元素是否有序
		std::cout << "所有元素有序" << std::endl;
	}
	if (ISRBTree(root)) {//检查是否满足红黑树性质
		std::cout << "红黑树性质满足" << std::endl;
	}
	if (father_check(root)) {//检查父节点指向是否正常
		std::cout << "所有节点指向父节点正常" << std::endl;
	}
}