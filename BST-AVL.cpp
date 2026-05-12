#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

struct Book {
	int id;
	string title;
	string author;
};

// node structure
struct Node {
	Book book;

	Node* left;
	Node* right;

	// constructor
	Node(Book b) {
		book = b;
		left = nullptr;
		right = nullptr;
	}
};

// BST SEARCH
Node* search(Node* root, int target, int& steps) {

	// value not found
	if (root == nullptr) return nullptr;

	steps++; // count that visited node 

	// value found
	if (root->book.id == target) return root;

	if (target > root->book.id) return search(root->right, target, steps); // search right subtree

	else return search(root->left, target, steps); // search left subtree
}

// BST INSERT
Node* insert(Node* root, Book value) {

	// tree/subtree is empty
	if (root == nullptr) return new Node(value);

	if (value.id > root->book.id) {
		root->right = insert(root->right, value); // go to right subtree & connect
	}

	else if (value.id < root->book.id) {
		root->left = insert(root->left, value); // go to left subtree & connect
	}

	return root;
}

// BST DELETE
Node* deleteFunc(Node* root, int target) { // we pass a node pointer, the value of any node of its children (not the node value itself)

	// target not found
	if (root == nullptr) return nullptr;

	// search left subtree
	if (target < root->book.id) root->left = deleteFunc(root->left, target);

	// search right subtree
	else if (target > root->book.id) root->right = deleteFunc(root->right, target);

	// if target found 
	else {   // this's implemented after the recursion during the searching for the target node, now the root represents the node to be really deleted

		// leaf with no child
		if (root->left == nullptr && root->right == nullptr) {

			delete root;
			return nullptr;
		}

		// node with one child
		if (root->left == nullptr && root->right != nullptr) {

			Node* temp = root->right;  // to have a copy from that child and its childen (so they aren't deleted/disconnected)

			delete root;  // delete the whole targetted node
			return temp;  // return the subtree starting from that child
		}

		if (root->left != nullptr && root->right == nullptr) {

			Node* temp = root->left;
			delete root;
			return temp;
		}

		// node with two children
		if (root->left != nullptr && root->right != nullptr) {

			Node* currNode = root->right;  // go to the right subtree

			while (currNode->left != nullptr) {  // keep moving to the smallest/left node value
				currNode = currNode->left;
			}

			root->book = currNode->book;  // replace the node value itself with the successor/small value

			root->right = deleteFunc(root->right, currNode->book.id);   // then delete the dublicated node/successor
		}
	}

	return root;
}

// BST INORDER
void inOrder(Node* root) {

	if (root == nullptr) return; // if tree/subtree is empty -> storp recursion

	inOrder(root->left);  // go to the left subtree & keep go deeply 

	cout << root->book.id << " " << root->book.title << " " << root->book.author << endl; // print the node (recursively bottom to top)

	inOrder(root->right); // after printing the node (go to its right and keep recustion)
}

// BST HEIGHT
int heightBST(Node* root) {

	if (root == nullptr) return 0;

	int leftHeight = heightBST(root->left);
	int rightHeight = heightBST(root->right);

	return 1 + max(leftHeight, rightHeight);
}

// AVL Tree Node structure
struct AVLNode
{
	Book book;
	AVLNode* left;
	AVLNode* right;
	int height;

	AVLNode(Book b) : book(b), left(nullptr), right(nullptr), height(1) {}
};

int height(AVLNode* node)
{
	return node ? node->height : 0;
}

int getBalance(AVLNode* node)
{
	return node ? height(node->left) - height(node->right) : 0; // Balance factor >> height of left subtree - height of right subtree
}

// Right rotation for LL imbalance
AVLNode* rightRotate(AVLNode* A)
{
	// Reading the nodes involved in the rotation
	AVLNode* B = A->left;  // B is the left child of A, which will become the new root of this subtree
	AVLNode* C = B->right; // C must stay fixed as the left child of B

	// Rotation process
	B->right = A; // B becomes the new root, so A becomes the right child of B
	A->left = C;  // C replaces B as the left child of A (the root)

	// Update heights after rotation
	A->height = 1 + max(height(A->left), height(A->right));
	B->height = 1 + max(height(B->left), height(B->right));

	return B;
}

// Left rotation for RR imbalance
AVLNode* leftRotate(AVLNode* A)
{
	// Reading the nodes involved in the rotation
	AVLNode* B = A->right; // B is the right child of A, which will become the new root of this subtree
	AVLNode* C = B->left;  // C must stay fixed as the right child of B

	// Rotation process
	B->left = A;  // B becomes the new root, so A becomes the left child of B
	A->right = C; // C replaces B as the right child of A (the root)

	// Update heights after rotation
	A->height = 1 + max(height(A->left), height(A->right));
	B->height = 1 + max(height(B->left), height(B->right));

	return B;
}

// LR Rotation for LR imbalance
AVLNode* leftRightRotate(AVLNode* A)
{
	// Reading the nodes involved in the rotation
	AVLNode* B = A->left;  // B is the left child of A, which will become the new root of this subtree
	AVLNode* C = B->right; // C is the right child of B, which will become the left child of A after the first rotation

	// First, perform a left rotation on B to bring C up
	B->right = C->left; // C's left child becomes B's right child
	C->left = B;        // C becomes the new root of this subtree, with B as its left child

	// Then, perform a right rotation on A to bring C up
	A->left = C->right; // C's right child becomes A's left child
	C->right = A;       // C becomes the new root of this subtree, with A as its right child

	// Update heights after rotations
	A->height = 1 + max(height(A->left), height(A->right));
	B->height = 1 + max(height(B->left), height(B->right));
	C->height = 1 + max(height(C->left), height(C->right));

	return C; // C is the new root of this subtree after the rotations
}

// RL Rotation for RL imbalance
AVLNode* rightLeftRotate(AVLNode* A)
{
	// Reading the nodes involved in the rotation
	AVLNode* B = A->right; // B is the right child of A,
	AVLNode* C = B->left;  // C is the left child of B, which will become the right child of A after the first rotation

	// First, perform a right rotation on B to bring C up
	B->left = C->right; // C's right child becomes B's left child
	C->right = B;       // C becomes the new root of this subtree, with B as its right child

	// Then, perform a left rotation on A to bring C up
	A->right = C->left; // C's left child becomes A's right child
	C->left = A;        // C becomes the new root of this subtree, with A

	// Update heights after rotations
	A->height = 1 + max(height(A->left), height(A->right));
	B->height = 1 + max(height(B->left), height(B->right));
	C->height = 1 + max(height(C->left), height(C->right));

	return C; // C is the new root of this subtree after the rotations
}

// Insert a book into the AVL tree and balance it
AVLNode* insertAVL(AVLNode* node, Book book)
{
	if (!node)
		return new AVLNode(book); // If the node is null, create a new node with the book

	if (book.id < node->book.id)
	{
		node->left = insertAVL(node->left, book);
	}
	else if (book.id > node->book.id)
	{
		node->right = insertAVL(node->right, book);
	}
	else
	{
		return node; // Duplicate IDs are not allowed, return the existing node
	}

	// Update height of this ancestor node
	node->height = 1 + max(height(node->left), height(node->right));

	// Get the balance factor to check whether this node became unbalanced
	int balance = getBalance(node);

	// LL Case
	if (balance > 1 && book.id < node->left->book.id)
	{
		return rightRotate(node);
	}

	// RR Case
	if (balance < -1 && book.id > node->right->book.id)
	{
		return leftRotate(node);
	}

	// LR Case
	if (balance > 1 && book.id > node->left->book.id)
	{
		return leftRightRotate(node);
	}

	// RL Case
	if (balance < -1 && book.id < node->right->book.id)
	{
		return rightLeftRotate(node);
	}

	return node;
}

// Node with two children: Get the inorder successor (smallest in the right subtree)
AVLNode* minValueNode(AVLNode* node)
{
	AVLNode* current = node;

	while (current->left)
	{
		current = current->left;
	}

	return current;
}

// Delete a book from the AVL tree and balance it
AVLNode* deleteNode(AVLNode* node, int id)
{
	if (!node)
		return node;

	if (id < node->book.id)
	{
		node->left = deleteNode(node->left, id);
	}
	else if (id > node->book.id)
	{
		node->right = deleteNode(node->right, id);
	}
	else
	{
		if (!node->left || !node->right)
		{
			AVLNode* temp = node->left ? node->left : node->right;

			if (!temp)
			{
				temp = node;
				node = nullptr;
			}
			else
			{
				*node = *temp;
			}

			delete temp;
		}
		else
		{
			AVLNode* temp = minValueNode(node->right);

			node->book = temp->book;

			node->right = deleteNode(node->right, temp->book.id);
		}
	}

	if (!node)
		return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	// LL Case
	if (balance > 1 && getBalance(node->left) >= 0)
	{
		return rightRotate(node);
	}

	// LR Case
	if (balance > 1 && getBalance(node->left) < 0)
	{
		return leftRightRotate(node);
	}

	// RR Case
	if (balance < -1 && getBalance(node->right) <= 0)
	{
		return leftRotate(node);
	}

	// RL Case
	if (balance < -1 && getBalance(node->right) > 0)
	{
		return rightLeftRotate(node);
	}

	return node;
}

// Search for a book by ID
AVLNode* searchAVL(AVLNode* node, int id, int& steps)
{
	if (!node)
		return nullptr;

	steps++;

	if (node->book.id == id)
		return node;

	if (id < node->book.id)
		return searchAVL(node->left, id, steps);
	else
		return searchAVL(node->right, id, steps);
}

// Inorder traversal to display books in sorted order
void inorder(AVLNode* node)
{
	if (node)
	{
		inorder(node->left);

		cout << "ID: " << node->book.id
			<< ", Title: " << node->book.title
			<< ", Author: " << node->book.author << endl;

		inorder(node->right);
	}
}
