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
	/*��������븨������*/
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
				return node;//��ȷ�����ȵĽڵ�
			}
		}
		return father; //�Ҳ�������father
	}

	void solve_double_red(RBTreeNode<T>* node) {
		if (node == root) { //����ݹ���root��ֱ�ӽ�root��Ϊ��ɫ������
			node->color = RBTreeColor::Black;
			return;
		}
		RBTreeNode<T>* fnode = node->father;
		if (fnode->color == RBTreeColor::Red) { //������ڵ�Ϊ��ɫ�������˫������
			RBTreeNode<T>* ffnode = fnode->father;  //��fnodeΪ��ڵ㣬���Ȼ�и��ڵ�ffnode
			RBTreeNode<T>* unode = ffnode->left == fnode ? ffnode->right : ffnode->left; //��ڵ�
			if (unode == nullptr || unode->color == RBTreeColor::Black) { //�����ڵ�Ϊ��ɫ������RR1����
				solveRR1(node, fnode, ffnode);
			}
			else {  //�����ڵ�Ϊ��ɫ������RR2����
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

	/*�����ɾ����������*/
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
		node->left = childnode->left; //node��ú��ӵĺ��,q�Һ�̵ĸ��ڵ����Ϊnode
		if (childnode->left != nullptr) {
			childnode->left->father = node;
		}
		node->right = childnode->right;
		if (childnode->right != nullptr) {
			childnode->right->father = node;
		}
		node->val = childnode->val;  //node��ú��ӵĺ��
		delete childnode;
		childnode = nullptr;
		node->color = RBTreeColor::Black;
	}

	void delete_node(RBTreeNode<T>* node) { //���ڿ�ֱ��ɾ���Ľڵ㣬�Ҹýڵ���1�����ӽڵ��û�к���
		if (node->left == nullptr && node->right == nullptr) {  //���2�����Ӷ�Ϊ�գ�ֱ��ɾ��
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
		if (bnode->color == RBTreeColor::Black) { //�ֵܽڵ�Ϊ��ɫ
			if (have_red_child(bnode)) {  //����ֵܽڵ�������1���캢�ӣ����BB1
				solveBB1(node, fnode, bnode);
			}
			else {  //�ֵܽڵ�û�к캢�ӣ����BB2
				if (fnode->color == RBTreeColor::Red) {  //���ڵ�Ϊ��ɫ
					solveBB2R(fnode, bnode);
				}
				else {  //���ڵ�Ϊ��ɫ
					solveBB2B(fnode, bnode);
				}
			}
		}
		else {  //�ֵ�Ϊ��ɫ����Ϊ��
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

	/*�����������������*/
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
		fnode->left = node->right; //�ӹ��Һ���
		if (node->right != nullptr) {  //�Һ��Ӹ��¸���
			node->right->father = fnode;
		}
		node->right = fnode;
		update_father(node, fnode, ffnode);
	}

	void rotate_l(RBTreeNode<T>* node, RBTreeNode<T>* fnode, RBTreeNode<T>* ffnode) {
		fnode->right = node->left;	//node�ĸ��׽ӹ�node������
		if (node->left != nullptr) {  //���Ӹ��¸���
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
		if (!root) { //���rootΪ�գ��򸳸����ڵ㣬��Ϊ��ɫ
			root = new RBTreeNode<T>(val, nullptr, RBTreeColor::Black);
			++_size;
			return;
		}
		RBTreeNode<T>* fnode = help_find(val);
		if (fnode != nullptr && fnode->val == val) { //���ظ�Ԫ�أ�������
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
		RBTreeNode<T>* pnode = node; //Ҫɾ���Ľڵ㣬����Ϊ�䱾������Ϊ��̽ڵ㣬��Ҫ�жϺ��ӵ�����
		if (node->left != nullptr && node->right != nullptr) { //�����2�����ӽڵ㣬��ͺ�̽���ֵ
			pnode = get_post_node(node);
			node->val = pnode->val;
		}
		if (pnode->color == RBTreeColor::Black){
			if (!have_red_child(pnode) && pnode != root) {  //����ڽڵ��к캢�ӻ���Ϊ���ڵ�,����
				solve_double_black(pnode);
			}
		}
		delete_node(pnode); //��ʱ�Ѿ�û��˫��ȱ��/pnodeΪ��ڵ�/pnodeΪ���ڵ�/pnodeΪ�ڽڵ㵫�к캢��
		--_size;
	}

	size_t size() const {
		return size;
	}
	
};

