#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct Book {
	int id;
	string title;
	string author;
};

// ======================================================
// BST SECTION
// ======================================================

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

	if (target > root->book.id)
		return search(root->right, target, steps); // search right subtree

	else
		return search(root->left, target, steps); // search left subtree
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
	if (target < root->book.id)
		root->left = deleteFunc(root->left, target);

	// search right subtree
	else if (target > root->book.id)
		root->right = deleteFunc(root->right, target);

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

	if (root == nullptr) return; // if tree/subtree is empty -> stop recursion

	inOrder(root->left);  // go to the left subtree & keep go deeply 

	cout << "ID: " << root->book.id
		<< ", Title: " << root->book.title
		<< ", Author: " << root->book.author << endl;

	inOrder(root->right); // after printing the node (go to its right and keep recursion)
}

// BST HEIGHT
int heightBST(Node* root) {

	if (root == nullptr) return 0;

	int leftHeight = heightBST(root->left);
	int rightHeight = heightBST(root->right);

	return 1 + max(leftHeight, rightHeight);
}

// BST RANGE SEARCH
void findInRangeBST(Node* root, int low, int high) {

	if (root == nullptr) return;

	if (root->book.id > low) {
		findInRangeBST(root->left, low, high);
	}

	if (root->book.id >= low && root->book.id <= high) {

		cout << "ID: " << root->book.id
			<< ", Title: " << root->book.title
			<< ", Author: " << root->book.author << endl;
	}

	if (root->book.id < high) {
		findInRangeBST(root->right, low, high);
	}
}

// BST CLOSEST BOOK
void findClosestBST(Node* root, int target, Node*& closest) {

	if (root == nullptr) return;

	if (abs(root->book.id - target) < abs(closest->book.id - target)) {
		closest = root;
	}

	if (target < root->book.id) {
		findClosestBST(root->left, target, closest);
	}
	else if (target > root->book.id) {
		findClosestBST(root->right, target, closest);
	}
}


// ======================================================
// AVL SECTION
// ======================================================

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
	AVLNode* B = A->left;  // B is the left child of A
	AVLNode* C = B->right; // C is the right child of B

	// First, perform a left rotation on B to bring C up
	B->right = C->left;
	C->left = B;

	// Then, perform a right rotation on A to bring C up
	A->left = C->right;
	C->right = A;

	// Update heights after rotations
	A->height = 1 + max(height(A->left), height(A->right));
	B->height = 1 + max(height(B->left), height(B->right));
	C->height = 1 + max(height(C->left), height(C->right));

	return C;
}

// RL Rotation for RL imbalance
AVLNode* rightLeftRotate(AVLNode* A)
{
	// Reading the nodes involved in the rotation
	AVLNode* B = A->right;
	AVLNode* C = B->left;

	// First, perform a right rotation on B to bring C up
	B->left = C->right;
	C->right = B;

	// Then, perform a left rotation on A to bring C up
	A->right = C->left;
	C->left = A;

	// Update heights after rotations
	A->height = 1 + max(height(A->left), height(A->right));
	B->height = 1 + max(height(B->left), height(B->right));
	C->height = 1 + max(height(C->left), height(C->right));

	return C;
}

// AVL INSERT
AVLNode* insert(AVLNode* node, Book book)
{
	if (!node)
		return new AVLNode(book);

	if (book.id < node->book.id)
	{
		node->left = insert(node->left, book);
	}
	else if (book.id > node->book.id)
	{
		node->right = insert(node->right, book);
	}
	else
	{
		return node;
	}

	// Update height of this ancestor node
	node->height = 1 + max(height(node->left), height(node->right));

	// Get the balance factor
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

// Node with two children: Get the inorder successor
AVLNode* minValueNode(AVLNode* node)
{
	AVLNode* current = node;

	while (current->left)
	{
		current = current->left;
	}

	return current;
}

// AVL DELETE
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
		// Node with only one child or no child
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

	// Update height
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

// AVL SEARCH
AVLNode* search(AVLNode* node, int id, int& steps)
{
	if (!node)
		return nullptr;

	steps++;

	if (node->book.id == id)
		return node;

	if (id < node->book.id)
		return search(node->left, id, steps);

	else
		return search(node->right, id, steps);
}

// AVL INORDER
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

// AVL RANGE SEARCH
void findInRange(AVLNode* node, int low, int high)
{
	if (!node)
		return;

	if (node->book.id > low)
	{
		findInRange(node->left, low, high);
	}

	if (node->book.id >= low && node->book.id <= high)
	{
		cout << "ID: " << node->book.id
			<< ", Title: " << node->book.title
			<< ", Author: " << node->book.author << endl;
	}

	if (node->book.id < high)
	{
		findInRange(node->right, low, high);
	}
}

// AVL CLOSEST BOOK
void findClosest(AVLNode* node, int target, AVLNode*& closest)
{
	if (!node)
		return;

	if (abs(node->book.id - target) < abs(closest->book.id - target))
	{
		closest = node;
	}

	if (node->book.id == target)
		return;

	if (target < node->book.id)
	{
		findClosest(node->left, target, closest);
	}
	else
	{
		findClosest(node->right, target, closest);
	}
}


// ======================================================
// MAIN FUNCTION
// ======================================================

int main() {

	// ======================================================
	// RANDOM INSERTION CASE
	// ======================================================

	cout << "=====================================================" << endl;
	cout << "RANDOM INSERTION CASE" << endl;
	cout << "=====================================================" << endl;

	// BST Root
	Node* bstRoot = nullptr;

	// AVL Root
	AVLNode* avlRoot = nullptr;

	// 20+ Random Books
	vector<Book> randomBooks = {
		{42, "Harry Potter", "J.K Rowling"},
		{7, "Atomic Habits", "James Clear"},
		{68, "Clean Code", "Robert Martin"},
		{15, "The Hobbit", "Tolkien"},
		{90, "1984", "George Orwell"},
		{3, "To Kill a Mockingbird", "Harper Lee"},
		{55, "Wuthering Heights", "Emily Bronte"},
		{23, "Brave New World", "Aldous Huxley"},
		{81, "The Road", "Cormac McCarthy"},
		{12, "Never Let Me Go", "Kazuo Ishiguro"},
		{37, "Atonement", "Ian McEwan"},
		{64, "Catch-22", "Joseph Heller"},
		{29, "Frankenstein", "Mary Shelley"},
		{99, "The Count of Monte Cristo", "Alexandre Dumas"},
		{5, "A Tale of Two Cities", "Charles Dickens"},
		{18, "The Rules of Life", "Richard Templar"},
		{73, "Pride and Prejudice", "Jane Austen"},
		{41, "War and Peace", "Leo Tolstoy"},
		{60, "Animal Farm", "George Orwell"},
		{8, "The Lord of the Rings", "J.R.R. Tolkien"}
	};

	// Insert into BST and AVL
	for (auto& b : randomBooks) {

		// BST insert
		bstRoot = insert(bstRoot, b);

		// AVL insert
		avlRoot = insert(avlRoot, b);
	}

	// ======================================================
	// BST TESTING
	// ======================================================

	cout << "\n---------------- BST Inorder Traversal ----------------\n" << endl;
	inOrder(bstRoot);

	// BST Search
	int bstSteps = 0;

	cout << "\n---------------- BST Search ----------------\n" << endl;

	Node* bstSearch = search(bstRoot, 73, bstSteps);

	if (bstSearch) {
		cout << "Book Found: " << bstSearch->book.title << endl;
	}
	else {
		cout << "Book not found" << endl;
	}

	cout << "BST Search Steps: " << bstSteps << endl;

	// BST Delete
	cout << "\n---------------- BST Delete ID 15 ----------------\n" << endl;

	bstRoot = deleteFunc(bstRoot, 15);

	inOrder(bstRoot);

	// BST Height
	cout << "\nBST Height: " << heightBST(bstRoot) << endl;

	// BST Range Search
	cout << "\n---------------- BST Books in Range [20 - 70] ----------------\n" << endl;

	findInRangeBST(bstRoot, 20, 70);

	// BST Closest Book
	cout << "\n---------------- BST Closest Book ----------------\n" << endl;

	Node* bstClosest = bstRoot;

	findClosestBST(bstRoot, 50, bstClosest);

	cout << "Closest Book To ID 50:" << endl;

	cout << "ID: " << bstClosest->book.id
		<< ", Title: " << bstClosest->book.title
		<< ", Author: " << bstClosest->book.author << endl;

	// ======================================================
	// AVL TESTING
	// ======================================================

	cout << "\n\n---------------- AVL Inorder Traversal ----------------\n" << endl;

	inorder(avlRoot);

	// AVL Search
	int avlSteps = 0;

	cout << "\n---------------- AVL Search ----------------\n" << endl;

	AVLNode* avlSearch = search(avlRoot, 73, avlSteps);

	if (avlSearch) {
		cout << "Book Found: " << avlSearch->book.title << endl;
	}
	else {
		cout << "Book not found" << endl;
	}

	cout << "AVL Search Steps: " << avlSteps << endl;

	// AVL Delete
	cout << "\n---------------- AVL Delete ID 15 ----------------\n" << endl;

	avlRoot = deleteNode(avlRoot, 15);

	inorder(avlRoot);

	// AVL Height
	cout << "\nAVL Height: " << height(avlRoot) << endl;

	// AVL RANGE SEARCH
	cout << "\n---------------- AVL Books in Range [20 - 70] ----------------\n" << endl;

	findInRange(avlRoot, 20, 70);

	// AVL CLOSEST BOOK ID
	cout << "\n---------------- AVL Closest Book ----------------\n" << endl;

	AVLNode* closest = avlRoot;

	findClosest(avlRoot, 50, closest);

	cout << "Closest Book To ID 50:" << endl;

	cout << "ID: " << closest->book.id
		<< ", Title: " << closest->book.title
		<< ", Author: " << closest->book.author << endl;

	// ======================================================
	// RANDOM INSERTION COMPARISON
	// ======================================================

	cout << "\n\n=====================================================" << endl;
	cout << "COMPARISON : RANDOM INSERTION" << endl;
	cout << "=====================================================" << endl;

	cout << "BST Height: " << heightBST(bstRoot) << endl;
	cout << "AVL Height: " << height(avlRoot) << endl;

	cout << "BST Search Steps: " << bstSteps << endl;
	cout << "AVL Search Steps: " << avlSteps << endl;

	// ======================================================
	// SORTED INSERTION CASE
	// ======================================================

	cout << "\n\n=====================================================" << endl;
	cout << "SORTED INSERTION CASE" << endl;
	cout << "=====================================================" << endl;

	// BST Root
	Node* sortedBST = nullptr;

	// AVL Root
	AVLNode* sortedAVL = nullptr;

	// Sorted Books
	vector<Book> sortedBooks = {
		{1, "Book1", "Author1"},
		{2, "Book2", "Author2"},
		{3, "Book3", "Author3"},
		{4, "Book4", "Author4"},
		{5, "Book5", "Author5"},
		{6, "Book6", "Author6"},
		{7, "Book7", "Author7"},
		{8, "Book8", "Author8"},
		{9, "Book9", "Author9"},
		{10, "Book10", "Author10"},
		{11, "Book11", "Author11"},
		{12, "Book12", "Author12"},
		{13, "Book13", "Author13"},
		{14, "Book14", "Author14"},
		{15, "Book15", "Author15"},
		{16, "Book16", "Author16"},
		{17, "Book17", "Author17"},
		{18, "Book18", "Author18"},
		{19, "Book19", "Author19"},
		{20, "Book20", "Author20"}
	};

	// Insert into BST and AVL
	for (auto& b : sortedBooks) {

		// BST insert
		sortedBST = insert(sortedBST, b);

		// AVL insert
		sortedAVL = insert(sortedAVL, b);
	}

	// ======================================================
	// SEARCH COMPARISON
	// ======================================================

	int bstSortedSteps = 0;
	int avlSortedSteps = 0;

	Node* bstSortedSearch = search(sortedBST, 20, bstSortedSteps);

	AVLNode* avlSortedSearch = search(sortedAVL, 20, avlSortedSteps);

	// ======================================================
	// HEIGHT + STEPS COMPARISON
	// ======================================================

	cout << "\n---------------- HEIGHT COMPARISON ----------------\n" << endl;

	cout << "BST Height (Sorted Insertions): "
		<< heightBST(sortedBST) << endl;

	cout << "AVL Height (Sorted Insertions): "
		<< height(sortedAVL) << endl;

	cout << "\n---------------- SEARCH STEPS COMPARISON ----------------\n" << endl;

	cout << "BST Search Steps: "
		<< bstSortedSteps << endl;

	cout << "AVL Search Steps: "
		<< avlSortedSteps << endl;

	// ======================================================
	// FINAL CONCLUSION
	// ======================================================

	cout << "\n\n=====================================================" << endl;
	cout << "FINAL OBSERVATION" << endl;
	cout << "=====================================================" << endl;

	cout << "BST becomes taller with sorted insertions because" << endl;
	cout << "it behaves like a linked list." << endl;

	cout << "\nAVL tree stays balanced using rotations," << endl;
	cout << "so searching becomes faster and requires fewer steps." << endl;

	return 0;
}
