#pragma once
enum class RBTreeColor {
	Red,
	Black
};

template<class T>
class RBTreeNode {
public:
	T val;
	RBTreeColor color;
	RBTreeNode* left, * right, * father;
	RBTreeNode() :color(RBTreeColor::Black) {};
	RBTreeNode(T val, RBTreeNode* father, RBTreeColor color) :
		val(val), father(father), color(color), left(nullptr), right(nullptr) {
		if (father != nullptr) {
			if (father->val > this->val) {
				father->left = this;
			}
			else {
				father->right = this;
			}
		}
	};
};

template<class T>
class RBTree {
private:
	size_t _size;
private:
	/*红黑树插入辅助函数*/
	RBTreeNode<T>* help_find(const T& val) {
		RBTreeNode<T>* node = root;
		RBTreeNode<T>* father = nullptr;
		while (node) {
			if (!node) {
				return nullptr;
			}
			if (node->val < val) {
				father = node;
				node = node->right;
			}
			else if (node->val > val) {
				father = node;
				node = node->left;
			}
			else {
				return node;//相等返回相等的节点
			}
		}
		return father; //找不到返回father
	}

	void solve_double_red(RBTreeNode<T>* node) {
		if (node == root) { //如果递归至root，直接将root置为黑色，返回
			node->color = RBTreeColor::Black;
			return;
		}
		RBTreeNode<T>* fnode = node->father;
		if (fnode->color == RBTreeColor::Red) { //如果父节点为红色，则进行双红修正
			RBTreeNode<T>* ffnode = fnode->father;  //若fnode为红节点，则必然有父节点ffnode
			RBTreeNode<T>* unode = ffnode->left == fnode ? ffnode->right : ffnode->left; //叔节点
			if (unode == nullptr || unode->color == RBTreeColor::Black) { //如果叔节点为黑色，进行RR1修正
				solveRR1(node, fnode, ffnode);
			}
			else {  //如果叔节点为红色，进行RR2修正
				solveRR2(node, fnode, ffnode);
			}
		}
	}

	void solveRR1(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* ffnode) {
		RBTreeNode<T>* fffnode = ffnode->father;
		ffnode->color = RBTreeColor::Red;
		RBTreeNode<T>* tnode = fnode;
		if (ffnode->left == fnode) {
			if (fnode->right == node) {
				tnode = node;
				rotate_l(tnode, fnode, ffnode);
			}
			rotate_r(tnode, ffnode, fffnode);
		}
		else {
			if (fnode->left == node) {
				tnode = node;
				rotate_r(tnode, fnode, ffnode);
			}
			rotate_l(tnode, ffnode, fffnode);
		}
		tnode->color = RBTreeColor::Black;
	}
	
	void solveRR2(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* ffnode) {
		ffnode->color = RBTreeColor::Red;
		ffnode->left->color = RBTreeColor::Black;
		ffnode->right->color = RBTreeColor::Black;
		solve_double_red(ffnode);
	}

	/*红黑树删除辅助函数*/
	RBTreeNode<T>* get_post_node(RBTreeNode<T>* node) {
		RBTreeNode<T>* rnode = node->right;
		RBTreeNode<T>* tnode = nullptr;
		while (rnode) {
			tnode = rnode;
			rnode = rnode->left;
		}
		return tnode;
	}

	void help_delete_node(RBTreeNode<T>* node, RBTreeNode<T>* childnode) {
		node->left = childnode->left; //node获得孩子的后继,q且后继的父节点更新为node
		if (childnode->left != nullptr) {
			childnode->left->father = node;
		}
		node->right = childnode->right;
		if (childnode->right != nullptr) {
			childnode->right->father = node;
		}
		node->val = childnode->val;  //node获得孩子的后继
		delete childnode;
		childnode = nullptr;
		node->color = RBTreeColor::Black;
	}

	void delete_node(RBTreeNode<T>* node) { //用于可直接删除的节点，且该节点有1个孩子节点或没有孩子
		if (node->left == nullptr && node->right == nullptr) {  //如果2个孩子都为空，直接删除
			if (node->father != nullptr) {
				if (node->father->left == node) {
					node->father->left = nullptr;
				}
				else {
					node->father->right = nullptr;
				}
			}
			delete node;
			node = nullptr;
		}
		else if (node->left == nullptr) {
			help_delete_node(node, node->right);
		}
		else {
			help_delete_node(node, node->left);
		}
	}

	bool have_red_child(RBTreeNode<T>* node) {
		return node->left != nullptr && node->left->color == RBTreeColor::Red || 
			node->right != nullptr && node->right->color == RBTreeColor::Red;
	}

	void solve_double_black(RBTreeNode<T>* node) {
		RBTreeNode<T>* fnode = node->father;
		RBTreeNode<T>* bnode = fnode->left == node ? fnode->right : fnode->left;
		if (bnode->color == RBTreeColor::Black) { //兄弟节点为黑色
			if (have_red_child(bnode)) {  //如果兄弟节点至少有1个红孩子，情况BB1
				solveBB1(node, fnode, bnode);
			}
			else {  //兄弟节点没有红孩子，情况BB2
				if (fnode->color == RBTreeColor::Red) {  //父节点为红色
					solveBB2R(fnode, bnode);
				}
				else {  //父节点为黑色
					solveBB2B(fnode, bnode);
				}
			}
		}
		else {  //兄弟为红色，则父为黑
			solveBB3(node, fnode, bnode);
		}
	}

	void solveBB1(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* bnode) {
		RBTreeNode<T>* tnode = bnode;
		RBTreeNode<T>* ffnode = fnode->father;
		if (fnode->left == bnode) {
			if (bnode->right != nullptr && bnode->right->color == RBTreeColor::Red) {
				tnode = bnode->right;
				rotate_l(tnode, bnode, fnode);
			}
			rotate_r(tnode, fnode, ffnode);
			if (bnode->left != nullptr) {
				bnode->left->color = RBTreeColor::Black;
			}
		}
		else {
			if (bnode->left != nullptr && bnode->left->color == RBTreeColor::Red) {
				tnode = bnode->left;
				rotate_r(tnode, bnode, fnode);
			}
			rotate_l(tnode, fnode, ffnode);
			if (bnode->right != nullptr) {
				bnode->right->color = RBTreeColor::Black;
			}
		}
		tnode->color = fnode->color;
		fnode->color = RBTreeColor::Black;
	}

	void solveBB2R(RBTreeNode<T>* fnode, RBTreeNode<T>* bnode) {
		fnode->color = RBTreeColor::Black;
		bnode->color = RBTreeColor::Red;
	}

	void solveBB2B(RBTreeNode<T>* fnode, RBTreeNode<T>* bnode) {
		bnode->color = RBTreeColor::Red;
		if (fnode != root) {
			solve_double_black(fnode);
		}
	}

	void solveBB3(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* bnode) {
		RBTreeNode<T>* ffnode = fnode->father;
		if (bnode == fnode->left) {
			rotate_r(bnode, fnode, ffnode);
		}
		else {
			rotate_l(bnode, fnode, ffnode);
		}
		fnode->color = RBTreeColor::Red;
		bnode->color = RBTreeColor::Black;
		solve_double_black(node);
	}

	/*红黑树调整辅助函数*/
	void update_father(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* ffnode) {
		fnode->father = node;
		if (ffnode == nullptr) {
			root = node;
		}
		else {
			if (ffnode->left == fnode) {
				ffnode->left = node;
			}
			else {
				ffnode->right = node;
			}
		}
		node->father = ffnode;
	}

	void rotate_r(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* ffnode) {
		fnode->left = node->right; //接管右孩子
		if (node->right != nullptr) {  //右孩子更新父亲
			node->right->father = fnode;
		}
		node->right = fnode;
		update_father(node, fnode, ffnode);
	}

	void rotate_l(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* ffnode) {
		fnode->right = node->left;	//node的父亲接管node的左孩子
		if (node->left != nullptr) {  //左孩子更新父亲
			node->left->father = fnode;
		}
		node->left = fnode;
		update_father(node, fnode, ffnode);
	}
	
public:
	RBTreeNode<T>* root;

	RBTree() :root(nullptr), _size(0) {};
		
	RBTreeNode<T>* find(T val) {
		RBTreeNode<T>* node = help_find(val);
		if (node == nullptr || node->val != val) { 
			return nullptr;
		}
		return node;
	}

	void insert(const T& val) {
		if (!root) { //如果root为空，则赋给父节点，且为黑色
			root = new RBTreeNode<T>(val, nullptr, RBTreeColor::Black);
			++_size;
			return;
		}
		RBTreeNode<T>* fnode = help_find(val);
		if (fnode != nullptr && fnode->val == val) { //有重复元素，不插入
			return;
		}
		RBTreeNode<T>* node = new RBTreeNode<T>(val, fnode, RBTreeColor::Red);
		solve_double_red(node);
		++_size;
	}

	void erase(const T& val) {
		RBTreeNode<T>* node = find(val);
		if (node == nullptr) {
			return;
		}
		RBTreeNode<T>* pnode = node; //要删除的节点，可能为其本身，可能为后继节点，需要判断孩子的数量
		if (node->left != nullptr && node->right != nullptr) { //如果有2个孩子节点，则和后继交换值
			pnode = get_post_node(node);
			node->val = pnode->val;
		}
		if (pnode->color == RBTreeColor::Black){
			if (!have_red_child(pnode) && pnode != root) {  //如果黑节点有红孩子或者为根节点,跳出
				solve_double_black(pnode);
			}
		}
		delete_node(pnode); //此时已经没有双黑缺陷/pnode为红节点/pnode为根节点/pnode为黑节点但有红孩子
		--_size;
	}

	size_t size() const {
		return size;
	}
	
};

