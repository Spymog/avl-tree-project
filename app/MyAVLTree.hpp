#ifndef __PROJ_FOUR_AVL_HPP
#define __PROJ_FOUR_AVL_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>
#include <iostream>

class ElementNotFoundException : public RuntimeException 
{
public:
	ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};


template<typename Key, typename Value>
class MyAVLTree
{
private:
	// fill in private member data here
	// If you need to declare private functions, do so here too.

	struct Node
	{
		Key key;
		Value value;
		Node* parent;
		Node* left;
		Node* right;
		unsigned int height;
		int balanceFactor;
	};

	Node* root;

	unsigned int keyCount;

	Node* getNode(const Key &k);
	const Node* getNode(const Key &k) const;

	void updateHeight(Node* n);
	void setChild(Node* p, std::string whichChild, Node* c);
	void replaceChild(Node* p, Node* currentChild, Node* c);
	int getBalance(Node* n);
	void rotateRight(Node* n);
	void rotateLeft(Node* n);
	void rebalance(Node* n);
	const std::vector<Key> inOrderKeys(Node* n) const; 
	const std::vector<Key> preOrderKeys(Node* n) const;
	const std::vector<Key> postOrderKeys(Node* n) const;
	void deleteTree(Node* root);



public:
	MyAVLTree();

	// In general, a copy constructor and assignment operator
	// are good things to have.
	// For this quarter, I am not requiring these. 
	//	MyAVLTree(const MyAVLTree & st);
	//	MyAVLTree & operator=(const MyAVLTree & st);


	// The destructor is, however, required. 
	~MyAVLTree();

	// size() returns the number of distinct keys in the tree.
	size_t size() const noexcept;

	// isEmpty() returns true if and only if the tree has no values in it. 
	bool isEmpty() const noexcept;

	// contains() returns true if and only if there
	//  is a (key, value) pair in the tree
	//	that has the given key as its key.
	bool contains(const Key & k) const noexcept; 

	// find returns the value associated with the given key
	// If !contains(k), this will throw an ElementNotFoundException
	// There needs to be a version for const and non-const MyAVLTrees.
	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	// Inserts the given key-value pair into 
	// the tree and performs the AVL re-balance
	// operation, as described in lecture. 
	// If the key already exists in the tree, 
	// you may do as you please (no test cases in
	// the grading script will deal with this situation)
	void insert(const Key & k, const Value & v);

	// in general, a "remove" function would be here
	// It's a little trickier with an AVL tree
	// and I am not requiring it for this quarter's ICS 46.
	// You should still read about the remove operation
	// in your textbook. 

	// The following three functions all return
	// the set of keys in the tree as a standard vector.
	// Each returns them in a different order.
	std::vector<Key> inOrder() const;
	std::vector<Key> preOrder() const;
	std::vector<Key> postOrder() const;

	const Key lChild(const Key& k) const;
	const Key rChild(const Key& k) const;

};


template<typename Key, typename Value>
MyAVLTree<Key,Value>::MyAVLTree()
	: root{nullptr}, keyCount{0}
{

}

template<typename Key, typename Value>
MyAVLTree<Key,Value>::~MyAVLTree()
{
	// delete root;
	deleteTree(root);
}

template<typename Key, typename Value>
size_t MyAVLTree<Key, Value>::size() const noexcept
{
	return keyCount;
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::isEmpty() const noexcept
{
	return root == nullptr;
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::contains(const Key &k) const noexcept
{
	return getNode(k) != nullptr;
}

template<typename Key, typename Value>
Value & MyAVLTree<Key, Value>::find(const Key & k)
{
	if (!contains(k))
	{
		throw ElementNotFoundException{"No node with matching key found"};
	}
	return getNode(k)->value;
}

template<typename Key, typename Value>
const Value & MyAVLTree<Key, Value>::find(const Key & k) const
{
	if (!contains(k))
	{
		throw ElementNotFoundException{"No node with matching key found"};
	}
	return getNode(k)->value;
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::insert(const Key & k, const Value & v)
{
	if (contains(k))
	{
		return;
	}

	keyCount++;
	if (root == nullptr)
	{
		root = new Node{k, v, nullptr, nullptr, nullptr, 0, 0};
	}
	else
	{
		Node* currentNode = root;
		Node* newNode = new Node{k, v, nullptr, nullptr, nullptr, 0, 0};
		while (currentNode != nullptr)
		{
			if (k < currentNode->key)
			{
				if (currentNode->left == nullptr)
				{
					newNode->parent = currentNode;
					currentNode->left = newNode;

					currentNode = nullptr;
				}
				else
				{
					currentNode = currentNode->left;
				}
			}
			else
			{
				if (currentNode->right == nullptr)
				{
					newNode->parent = currentNode;
					currentNode->right = newNode;

					currentNode = nullptr;

				}
				else
				{
					currentNode = currentNode->right;
				}
			}
		}

		currentNode = newNode->parent;
		while (currentNode != nullptr)
		{
			rebalance(currentNode);
			currentNode = currentNode->parent;
		}

	}
}




template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::inOrder() const
{
	// std::vector<Key> foo;
	return inOrderKeys(root); 
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::preOrder() const
{
	// std::vector<Key> foo;
	return preOrderKeys(root); 
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::postOrder() const
{
	// std::vector<Key> foo;
	return postOrderKeys(root); 
}

template<typename Key, typename Value>
typename MyAVLTree<Key, Value>::Node* MyAVLTree<Key, Value>::getNode(const Key &k)
{
	Node* currentNode = root;

	while (currentNode != nullptr)
	{
		if (!(k < currentNode->key) && !(currentNode->key < k))
		{
			return currentNode;
		}
		else if (k < currentNode->key)
		{
			currentNode = currentNode->left;
		}
		else
		{
			currentNode = currentNode->right;
		}
	}

	return nullptr;
}

template<typename Key, typename Value>
const typename MyAVLTree<Key, Value>::Node* MyAVLTree<Key, Value>::getNode(const Key &k) const
{
	Node* currentNode = root;

	while (currentNode != nullptr)
	{
		if (!(k < currentNode->key) && !(currentNode->key < k))
		{
			return currentNode;
		}
		else if (k < currentNode->key)
		{
			currentNode = currentNode->left;
		}
		else
		{
			currentNode = currentNode->right;
		}
	}

	return nullptr;
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::updateHeight(MyAVLTree<Key, Value>::Node* n)
{
	int leftHeight = -1;
	if (n->left != nullptr)
	{
		// updateHeight(n->left);
		leftHeight = n->left->height;
	}
	int rightHeight = -1;
	if (n->right != nullptr)
	{
		// updateHeight(n->right);
		rightHeight = n->right->height;
	}
	if (leftHeight < rightHeight)
	{
		n->height = rightHeight + 1;
	}
	else
	{
		n->height = leftHeight + 1;
	}
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::setChild(MyAVLTree<Key, Value>::Node* p, std::string whichChild, Node* c)
{
	if (whichChild != "left" && whichChild != "right")
	{
		return;
	}

	if (whichChild == "left")
	{
		p->left = c;
	}
	else
	{
		p->right = c;
	}
	if (c != nullptr)
	{
		c->parent = p;
	}
	
	updateHeight(p);
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::replaceChild(MyAVLTree<Key, Value>::Node* p, Node* currentChild, Node* c)
{	
	if (p->left == currentChild)
	{
		setChild(p, "left", c);
	}
	else if (p->right == currentChild)
	{
		setChild(p, "right", c);
	}
}

template<typename Key, typename Value>
int MyAVLTree<Key, Value>::getBalance(MyAVLTree<Key, Value>::Node* n)
{
	int leftHeight = -1;
	if (n->left != nullptr)
	{
		leftHeight = n->left->height;
	}
		
   	int rightHeight = -1;
   	if (n->right != nullptr)
	{
      	rightHeight = n->right->height;
	}
	return leftHeight - rightHeight;
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rotateRight(MyAVLTree<Key, Value>::Node* n)
{
	Node* leftRightChild = n->left->right;
	if (n->parent != nullptr)
	{
		replaceChild(n->parent, n, n->left);
	}
	else
	{
		root = n->left;
		root->parent = nullptr;
	}
	setChild(n->left, "right", n);
	setChild(n, "left", leftRightChild);
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rotateLeft(MyAVLTree<Key, Value>::Node* n)
{
	Node* rightLeftChild = n->right->left;
	if (n->parent != nullptr)
	{
		replaceChild(n->parent, n, n->right);
	}
	else
	{
		root = n->right;
		root->parent = nullptr;
	}
	setChild(n->right, "left", n);
	setChild(n, "right", rightLeftChild);
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rebalance(MyAVLTree<Key, Value>::Node* n)
{
	updateHeight(n);

	if (getBalance(n) == -2)
	{
		if (getBalance(n->right) == 1)
		{
			rotateRight(n->right);	
		}
		rotateLeft(n);
	}
	else if (getBalance(n) == 2)
	{
		if (getBalance(n->left) == -1)
		{
			rotateLeft(n->left);
		}
		rotateRight(n);
	}	
}

template<typename Key, typename Value>
const std::vector<Key> MyAVLTree<Key, Value>::inOrderKeys(MyAVLTree<Key, Value>::Node* n) const
{
	std::vector<Key> keys;

	if (n == nullptr)
	{
		return keys;
	}

	std::vector<Key> leftKeys = inOrderKeys(n->left);
	keys.insert(keys.end(), leftKeys.begin(), leftKeys.end());

	keys.push_back(n->key);

	std::vector<Key> rightKeys = inOrderKeys(n->right);
	keys.insert(keys.end(), rightKeys.begin(), rightKeys.end());

	return keys;
}

template<typename Key, typename Value>
const std::vector<Key> MyAVLTree<Key, Value>::preOrderKeys(MyAVLTree<Key, Value>::Node* n) const
{
	std::vector<Key> keys;

	if (n == nullptr)
	{
		return keys;
	}

	keys.push_back(n->key);

	std::vector<Key> leftKeys = preOrderKeys(n->left);
	keys.insert(keys.end(), leftKeys.begin(), leftKeys.end());

	std::vector<Key> rightKeys = preOrderKeys(n->right);
	keys.insert(keys.end(), rightKeys.begin(), rightKeys.end());

	return keys;
}

template<typename Key, typename Value>
const std::vector<Key> MyAVLTree<Key, Value>::postOrderKeys(MyAVLTree<Key, Value>::Node* n) const
{
	std::vector<Key> keys;

	if (n == nullptr)
	{
		return keys;
	}

	std::vector<Key> leftKeys = postOrderKeys(n->left);
	keys.insert(keys.end(), leftKeys.begin(), leftKeys.end());

	std::vector<Key> rightKeys = postOrderKeys(n->right);
	keys.insert(keys.end(), rightKeys.begin(), rightKeys.end());

	keys.push_back(n->key);

	return keys;
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::deleteTree(MyAVLTree<Key, Value>::Node* root)
{
	if (root == nullptr)
	{
		return;
	}

	deleteTree(root->left);
	deleteTree(root->right);

	delete root;

	root = nullptr;
}

template<typename Key, typename Value>
const Key MyAVLTree<Key, Value>::lChild(const Key& k) const
{	
	if (getNode(k) != nullptr && getNode(k)->left != nullptr)
	{
		return getNode(k)->left->key;
	}
	else
	{
		Key k = Key{};
		return k;
	}
}

template<typename Key, typename Value>
const Key MyAVLTree<Key, Value>::rChild(const Key& k) const
{	
	if (getNode(k) != nullptr && getNode(k)->right != nullptr)
	{
		return getNode(k)->right->key;
	}
	else
	{
		Key k = Key{};
		return k;
	}
	
}

#endif 