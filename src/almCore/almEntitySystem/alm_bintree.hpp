#ifndef _ALM_BIN_TREE_HPP_
#define _ALM_BIN_TREE_HPP_

#include <string>

#define red true
#define black false

template <class Data>
class RBTree
{
public:
	struct Node
	{
		Data data;
		Node *left;
		Node *right;
		Node *parent;
		bool color : 1;
		Node(const Data &vdata) : data(vdata), left(nullptr), right(nullptr), parent(nullptr), color(red) {}
	};

public:
	RBTree();
	virtual ~RBTree();

	virtual void Push(const Data &data);
	virtual bool Contain(const Data &data) const;
	virtual void Delete(const Data &data);
	virtual void Print();

protected:
	void LeftRotate(Node *pivot);
	void RightRotate(Node *pivot);
	void ResetParent(Node *pivot, Node *root);
	void Rebalance(Node *node);
	Node *FindNode(const Data &value) const;

	virtual bool Compare(const Node *left, const Node *right);

	void ReleaseTree(Node *node);

	Node *m_root;
private:
	unsigned int m_size;
};



template<class Data>
inline RBTree<Data>::RBTree()
	: m_root(nullptr)
	, m_size(0)
{
}

template<class Data>
inline RBTree<Data>::~RBTree()
{
	if (m_root) ReleaseTree(m_root);
	m_root = nullptr;
}



template<class Data>
inline void RBTree<Data>::Push(const Data & data)
{
	++m_size;
	Node *node = new Node(data);
	if (!m_root)
	{
		m_root = node;
		m_root->color = black;
		return;
	}

	Node *head = m_root;
	while (true)
	{
		if (Compare(node, head))
		{
			if (!head->left)
			{
				head->left = node;
				break;
			}
			head = head->left;
		}
		else
		{
			if (!head->right)
			{
				head->right = node;
				break;
			}
			head = head->right;
		}
	}

	node->parent = head;
	Rebalance(node);
}

template<class Data>
inline bool RBTree<Data>::Contain(const Data & data) const
{
	return static_cast<bool>(FindNode(data));
}

template<class Data>
inline void RBTree<Data>::Delete(const Data & data)
{
	Node *node = FindNode(data);
	if (!node) return;

	auto replaceNodeWith = [](Node *& node, Node *replace)->void{
		if (node->parent->left == node)
			node->parent->left = replace;
		else if (node->parent->right == node)
			node->parent->right = replace;
	};

	if (!node->left && !node->right)
	{
		replaceNodeWith(node, nullptr);
		delete node;
		--m_size;
	}
	else
	{
		if (node->left && node->right) 
		{
			Node *replace = node->left;
			while (true)
			{
				if (replace->left && replace->left->data > replace->data) replace = replace->left;
				else if (replace->right && replace->right->data > replace->data) replace = replace->right;
				else break;
			}
			node->data = replace->data;
			node = replace;
		}

		Node *child = node->left ? node->left : node->right;
		if (child) child->parent = node->parent;
		replaceNodeWith(node, child);
		delete node;
		--m_size;
	} 
}

template<class Data>
inline void RBTree<Data>::Print()
{
}

template<class Data>
inline void RBTree<Data>::LeftRotate(Node * pivot)
{
	Node *root = pivot->parent;
	ResetParent(pivot, root);
	root->right = pivot->left;
	pivot->left = root;
	if (root->right) root->right->parent = root;
}

template<class Data>
inline void RBTree<Data>::RightRotate(Node * pivot)
{
	Node *root = pivot->parent;
	ResetParent(pivot, root);
	root->left = pivot->right;
	pivot->right = root;
	if (root->left) root->left->parent = root;
}

template<class Data>
inline void RBTree<Data>::ResetParent(Node * pivot, Node * root)
{
	if (Node *rootParent = root->parent)
	{
		if (rootParent->left && rootParent->left == root)
			rootParent->left = pivot;
		else if (rootParent->right && rootParent->right == root)
			rootParent->right = pivot;
	}
	else m_root = pivot;
	pivot->parent = root->parent;
	root->parent = pivot;
}

template<class Data>
inline void RBTree<Data>::Rebalance(Node * node)
{
	if (!node->parent) return;
	if (!node->parent->parent) return;

	Node *parent = node->parent;
	while (parent && parent->color == red)
	{
		if (!parent->parent) break;
		Node *grandpa = parent->parent;
		Node *uncle = (grandpa->left == parent) ? grandpa->right : grandpa->left;
		if (uncle && uncle->color == red)
		{
			grandpa->color = red;
			uncle->color = parent->color = black;
		}
		else if(parent->color == red)
		{
			if (grandpa->right == parent)
			{
				if (parent->left == node)
				{
					RightRotate(node);
					std::swap(node, parent);
				}
				LeftRotate(parent);
			}
			else
			{
				if (parent->right == node)
				{
					LeftRotate(node);
					std::swap(node, parent);
				}
				RightRotate(parent);
			}
			parent->color = black;
			grandpa->color = red;
		}
		node = grandpa;
		parent = node ? node->parent : nullptr;
	}
	m_root->color = black;
}

template<class Data>
inline typename RBTree<Data>::Node * RBTree<Data>::FindNode(const Data & value) const
{
	Node *head = m_root;
	while (head)
	{
		if (head->data == value) return head;
		head = (head->data < value) ? head->right : head->left;
	}
	return nullptr;
}

template<class Data>
inline void RBTree<Data>::ReleaseTree(Node * node)
{
	if (node->left) ReleaseTree(node->left);
	if (node->right) ReleaseTree(node->right);
	delete node;
	node = nullptr;
	--m_size;
}

template<class Data> inline bool RBTree<Data>::Compare(const Node *left, const Node *right)
{
	return left->data < right->data;
}
#endif // !_ALM_BIN_TREE_HPP_