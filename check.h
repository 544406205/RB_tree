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
		std::cout << "�ڵ� " << root->val << " ָ��ĸ��ڵ��쳣" << std::endl;
		return false;
	}
	_father_check(root->left);
	_father_check(root->right);
	return true;
}
bool father_check(Node* root) {
	bool tmark = _father_check(root);
	if (root->father != nullptr) {
		std::cout << "�ڵ� " << root->val << " ָ��ĸ��ڵ��쳣" << std::endl;
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
			std::cout << "�������������" << std::endl;
			return false;
		}
	}
	return true;
}
bool _ISRBTree(Node* root, int count, int BlackCount) {
	if (root == nullptr) {
		if (count != BlackCount) {
			std::cout << "error����ɫ������Ŀ�����" << std::endl;
			return false;
		}
		return true;
	}
	if (root->color == RBTreeColor::Red && root->father->color == RBTreeColor::Red) {
		std::cout << "error�����������ĺ�ɫ���" << std::endl;
		return false;
	}
	if (root->color == RBTreeColor::Black) {
		++count;
	}
	return _ISRBTree(root->left, count, BlackCount) && _ISRBTree(root->right, count, BlackCount);
}
bool ISRBTree(Node* root) {
	if (root == nullptr) {//�����Ǻ����
		return true;
	}
	if (root->color == RBTreeColor::Red) {
		std::cout << "error�������Ϊ��ɫ" << std::endl;
		return false;
	}
	//������·����Ϊ��ɫ�����Ŀ�Ĳο�ֵ
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
	if (Inorder(root)) { //���Ԫ���Ƿ�����
		std::cout << "����Ԫ������" << std::endl;
	}
	if (ISRBTree(root)) {//����Ƿ�������������
		std::cout << "�������������" << std::endl;
	}
	if (father_check(root)) {//��鸸�ڵ�ָ���Ƿ�����
		std::cout << "���нڵ�ָ�򸸽ڵ�����" << std::endl;
	}
}