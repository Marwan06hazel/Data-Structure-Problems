#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

// Book structure to hold book information
struct Book {
	int id;
	string title;
	string author;
};

// node structure
struct BSTNode {
	Book book;

	BSTNode* left;
	BSTNode* right;

	// constructor
	BSTNode(Book b) {
		book = b;
		left = nullptr;
		right = nullptr;
	}
};

// BST Functions Implementaion

BSTNode* bst_search(BSTNode* root, int target, int& steps) {

	// value not found
	if (root == nullptr) return nullptr;

	steps++; // count that visited node 

	// value found
	if (root->book.id == target) return root;

	if (target > root->book.id) return bst_search(root->right, target, steps); // search right subtree

	else return bst_search(root->left, target, steps); // search left subtree
}

BSTNode* bst_insert(BSTNode* root, Book value) {

	// tree/subtree is empty
	if (root == nullptr) return new BSTNode(value);

	if (value.id > root->book.id) {
		root->right = bst_insert(root->right, value); // go to right subtree & connect
	}

	else if (value.id < root->book.id) {
		root->left = bst_insert(root->left, value); // go to left subtree & connect
	}

	return root;
}

BSTNode* bst_deleteFunc(BSTNode* root, int target) { // we pass a node pointer, the value of any node of its children (not the node value itself)

	// target not found
	if (root == nullptr) return nullptr;

	// search left subtree
	if (target < root->book.id) root->left = bst_deleteFunc(root->left, target);

	// search right subtree
	else if (target > root->book.id) root->right = bst_deleteFunc(root->right, target);

	// if target found 
	else {   // this's implemented after the recursion during the searching for the target node, now the root represents the node to be really deleted

		// leaf with no child
		if (root->left == nullptr && root->right == nullptr) {

			delete root;
			return nullptr;
		}

		// node with one child
		if (root->left == nullptr && root->right != nullptr) {

			BSTNode* temp = root->right;  // to have a copy from that child and its childen (so they aren't deleted/disconnected)

			delete root;  // delete the whole targetted node
			return temp;  // return the subtree starting from that child
		}

		if (root->left != nullptr && root->right == nullptr) {

			BSTNode* temp = root->left;
			delete root;
			return temp;
		}

		// node with two children
		if (root->left != nullptr && root->right != nullptr) {

			BSTNode* currNode = root->right;  // go to the right subtree

			while (currNode->left != nullptr) {  // keep moving to the smallest/left node value
				currNode = currNode->left;
			}

			root->book = currNode->book;  // replace the node value itself with the successor/small value

			root->right = bst_deleteFunc(root->right, currNode->book.id);   // then delete the dublicated node/successor
		}
	}

	return root;
}

void bst_inOrder(BSTNode* root) {

	if (root == nullptr) return; // if tree/subtree is empty -> stop recursion

	bst_inOrder(root->left);  // go to the left subtree & keep go deeply 

	cout << "ID: " << root->book.id << ", Title: " << root->book.title << ", Author: " << root->book.author << endl; // print the node (recursively bottom to top)

	bst_inOrder(root->right); // after printing the node (go to its right and keep recursion)
}

int bst_height(BSTNode* root) {

	if (root == nullptr) return 0;

	int leftHeight = bst_height(root->left);
	int rightHeight = bst_height(root->right);

	return 1 + max(leftHeight, rightHeight);
}

// Find books in ID range
void bst_findInRange(BSTNode* node, int low, int high) {
	if (!node)
		return;

	if (node->book.id > low) {                    // If the book ID is greater than the low value, search in the left subtree
		bst_findInRange(node->left, low, high);   // Search in the left subtree
	}

	if (node->book.id >= low && node->book.id <= high) {                                                                         // If the book ID is within the range, display it
		cout << "ID: " << node->book.id << ", Title: " << node->book.title << ", Author: " << node->book.author << endl;         // Display the book information
	}

	if (node->book.id < high) {                   // If the book ID is less than the high value, search in the right subtree
		bst_findInRange(node->right, low, high);  // Search in the right subtree
	}
}

// BST: Find closest book ID
void bst_findClosest(BSTNode* node, int target, BSTNode*& closest) {
	if (!node)
		return;

	// Update closest if this node is better
	if (abs(node->book.id - target) < abs(closest->book.id - target)) {
		closest = node;
	}

	// If exact match, no need to go further
	if (node->book.id == target)
		return;

	if (target < node->book.id) {
		bst_findClosest(node->left, target, closest);
		// Also check right if it could have a closer value
		if (node->right && abs(node->right->book.id - target) < abs(closest->book.id - target))
			bst_findClosest(node->right, target, closest);
	}
	else {
		bst_findClosest(node->right, target, closest);
		// Also check left if it could have a closer value
		if (node->left && abs(node->left->book.id - target) < abs(closest->book.id - target))
			bst_findClosest(node->left, target, closest);
	}
}

// BST wrapper: Find the closest book to a target ID
void bst_findClosest(BSTNode* root, int target) {
	if (!root) {
		cout << "No books in the library." << endl;
		return;
	}

	BSTNode* closest = root; // Start with the root as the closest
	bst_findClosest(root, target, closest);
	cout << "Closest book to ID " << target << ": ID " << closest->book.id << ", Title: " << closest->book.title << ", Author: " << closest->book.author << endl;
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

// AVL Tree class for managing books
class AVLTree
{
private:
	AVLNode* root;

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
	AVLNode* insert(AVLNode* node, Book book)
	{
		if (!node)
			return new AVLNode(book); // If the node is null, create a new node with the book

		if (book.id < node->book.id)
		{ // If the book ID is less than the current node's book ID, go to the left subtree
			node->left = insert(node->left, book);
		}
		else if (book.id > node->book.id)
		{ // If the book ID is greater than the current node's book ID, go to the right subtree
			node->right = insert(node->right, book);
		}
		else
		{
			return node; // Duplicate IDs are not allowed, return the existing node
		}

		// Update height of this ancestor node
		node->height = 1 + max(height(node->left), height(node->right));

		// Get the balance factor to check whether this node became unbalanced
		int balance = getBalance(node);

		// If unbalanced, there are 4 cases

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

		return node; // Return the unchanged node pointer
	}

	// Node with two children: Get the inorder successor (smallest in the right subtree)
	AVLNode* minValueNode(AVLNode* node)
	{
		AVLNode* current = node;

		while (current->left)
		{
			current = current->left; // Move to the leftmost node
		}

		return current; // Return the leftmost node, which is the minimum value node
	}

	// Delete a book from the AVL tree and balance it
	AVLNode* deleteNode(AVLNode* node, int id)
	{
		if (!node)
			return node; // If the node is null, return it

		if (id < node->book.id)
		{ // If the book ID to be deleted is less than the current node's book ID, go to the left subtree
			node->left = deleteNode(node->left, id);
		}
		else if (id > node->book.id)
		{ // If the book ID to be deleted is greater than the current node's book ID, go to the right subtree
			node->right = deleteNode(node->right, id);
		}
		else
		{
			// Node with only one child or no child
			if (!node->left || !node->right)
			{
				AVLNode* temp = node->left ? node->left : node->right; // Get the non-null child

				if (!temp)
				{ // No child case
					temp = node;
					node = nullptr; // Set the current node to null
				}
				else
				{                  // One child case
					*node = *temp; // Copy the contents of the non-null child to this node
				}
				delete temp; // Free the memory of the old node
			}
			else
			{

				// Node with two children: Get the inorder successor (smallest in the right subtree)
				AVLNode* temp = minValueNode(node->right);

				// Copy the inorder successor's content to this node (Replace the current node's book with the successor's book)
				node->book = temp->book;

				// Delete the inorder successor (Delete the duplicate node in the right subtree)
				node->right = deleteNode(node->right, temp->book.id);
			}
		}

		if (!node)
			return node; // If the tree had only one node then return

		// Update height of this ancestor node
		node->height = 1 + max(height(node->left), height(node->right));

		// Get the balance factor to check whether this node became unbalanced
		int balance = getBalance(node);

		// If unbalanced, there are 4 cases

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

		return node; // Return the unchanged node pointer
	}

	// Search for a book by ID
	AVLNode* search(AVLNode* node, int id, int& steps)
	{
		if (!node || node->book.id == id)
		{
			if (node) steps++; // count that visited node (only if found)
			return node; // If the node is null or the book ID matches, return the node
		}

		steps++; // count that visited node

		if (id < node->book.id)
		{
			return search(node->left, id, steps); // Search in the left subtree
		}
		else
		{
			return search(node->right, id, steps); // Search in the right subtree
		}
	}

	// Inorder traversal to display books in sorted order
	void inorder(AVLNode* node)
	{
		if (node)
		{
			inorder(node->left);                                                                                              // Traverse the left subtree
			cout << "ID: " << node->book.id << ", Title: " << node->book.title << ", Author: " << node->book.author << endl; // Display the book information
			inorder(node->right);                                                                                             // Traverse the right subtree
		}
	}

	// Find books in ID range
	void findInRange(AVLNode* node, int low, int high)
	{
		if (!node)
			return;

		if (node->book.id > low)
		{                                       // If the book ID is greater than the low value, search in the left subtree
			findInRange(node->left, low, high); // Search in the left subtree
		}

		if (node->book.id >= low && node->book.id <= high)
		{                                                                                                                     // If the book ID is within the range, display it
			cout << "ID: " << node->book.id << ", Title: " << node->book.title << ", Author: " << node->book.author << endl; // Display the book information
		}

		if (node->book.id < high)
		{                                        // If the book ID is less than the high value, search in the right subtree
			findInRange(node->right, low, high); // Search in the right subtree
		}
	}

	// Find closest book ID
	void findClosest(AVLNode* node, int target, AVLNode*& closest)
	{
		if (!node)
			return;

		// Update closest if this node is better
		if (abs(node->book.id - target) < abs(closest->book.id - target))
		{
			closest = node;
		}

		// If exact match, no need to go further
		if (node->book.id == target)
			return;

		if (target < node->book.id)
		{
			findClosest(node->left, target, closest);
			// Also check right if it could have a closer value
			if (node->right && abs(node->right->book.id - target) < abs(closest->book.id - target))
				findClosest(node->right, target, closest);
		}
		else
		{
			findClosest(node->right, target, closest);
			// Also check left if it could have a closer value
			if (node->left && abs(node->left->book.id - target) < abs(closest->book.id - target))
				findClosest(node->left, target, closest);
		}
	}

public:
	AVLTree() : root(nullptr) {}

	void insert(Book b)
	{
		root = insert(root, b);
	}

	void deleteBook(int id)
	{
		root = deleteNode(root, id);
	}

	void search(int id)
	{
		int steps = 0;
		AVLNode* result = search(root, id, steps);
		if (result)
			cout << "Found: ID " << result->book.id << ", Title: " << result->book.title << ", Author: " << result->book.author << endl;
		else
			cout << "Book with ID " << id << " not found." << endl;
	}

	int searchSteps(int id)
	{
		int steps = 0;
		search(root, id, steps);
		return steps;
	}

	void inorder()
	{
		inorder(root);
	}

	int getHeight()
	{
		return height(root);
	}

	void findInRange(int low, int high)
	{
		cout << "Books with ID in range [" << low << ", " << high << "]:" << endl;
		findInRange(root, low, high);
	}

	void findClosest(int target)
	{
		if (!root)
		{
			cout << "No books in the library." << endl;
			return;
		}

		AVLNode* closest = root; // Start with the root as the closest
		findClosest(root, target, closest);
		cout << "Closest book to ID " << target << ": ID " << closest->book.id << ", Title: " << closest->book.title << ", Author: " << closest->book.author << endl;
	}
};


int main() {

	// CASE 1: Random IDs

	BSTNode* bstRandom = nullptr;
	AVLTree avlRandom;

	Book b1 = { 42, "Harry Potter", "J.K Rowling" };
	Book b2 = { 7, "Atomic Habits", "James Clear" };
	Book b3 = { 68, "Clean Code", "Robert Martin" };
	Book b4 = { 15, "The Hobbit", "Tolkien" };
	Book b5 = { 90, "1984", "George Orwell" };
	Book b6 = { 3, "To Kill a Mockingbird", "Harper Lee" };
	Book b7 = { 55, "Wuthering Heights", "Emily Brontë" };
	Book b8 = { 23, "Brave New World", "Aldous Huxley" };
	Book b9 = { 81, "The Road", "Cormac McCarthy" };
	Book b10 = { 12, "Never Let Me Go", "Kazuo Ishiguro" };
	Book b11 = { 37, "Atonement", "Ian McEwan" };
	Book b12 = { 64, "Catch-22", "Joseph Heller" };
	Book b13 = { 29, "Frankenstein", "Mary Shelley" };
	Book b14 = { 99, "The Count of Monte Cristo", "Alexandre Dumas" };
	Book b15 = { 5, "A Tale of Two Cities", "Charles Dickens" };
	Book b16 = { 18, "The Rules of Life", "Richard Templar" };
	Book b17 = { 73, "Pride and Prejudice", "Jane Austen" };
	Book b18 = { 41, "War and Peace", "Leo Tolstoy" };
	Book b19 = { 60, "Animal Farm", "George Orwell" };
	Book b20 = { 8, "The Lord of the Rings", "J.R.R. Tolkien" };

	// insert (BST)
	bstRandom = bst_insert(bstRandom, b1);
	bstRandom = bst_insert(bstRandom, b2);
	bstRandom = bst_insert(bstRandom, b3);
	bstRandom = bst_insert(bstRandom, b4);
	bstRandom = bst_insert(bstRandom, b5);
	bstRandom = bst_insert(bstRandom, b6);
	bstRandom = bst_insert(bstRandom, b7);
	bstRandom = bst_insert(bstRandom, b8);
	bstRandom = bst_insert(bstRandom, b9);
	bstRandom = bst_insert(bstRandom, b10);
	bstRandom = bst_insert(bstRandom, b11);
	bstRandom = bst_insert(bstRandom, b12);
	bstRandom = bst_insert(bstRandom, b13);
	bstRandom = bst_insert(bstRandom, b14);
	bstRandom = bst_insert(bstRandom, b15);
	bstRandom = bst_insert(bstRandom, b16);
	bstRandom = bst_insert(bstRandom, b17);
	bstRandom = bst_insert(bstRandom, b18);
	bstRandom = bst_insert(bstRandom, b19);
	bstRandom = bst_insert(bstRandom, b20);

	// insert (AVL)
	avlRandom.insert(b1);
	avlRandom.insert(b2);
	avlRandom.insert(b3);
	avlRandom.insert(b4);
	avlRandom.insert(b5);
	avlRandom.insert(b6);
	avlRandom.insert(b7);
	avlRandom.insert(b8);
	avlRandom.insert(b9);
	avlRandom.insert(b10);
	avlRandom.insert(b11);
	avlRandom.insert(b12);
	avlRandom.insert(b13);
	avlRandom.insert(b14);
	avlRandom.insert(b15);
	avlRandom.insert(b16);
	avlRandom.insert(b17);
	avlRandom.insert(b18);
	avlRandom.insert(b19);
	avlRandom.insert(b20);

	// BST: Random IDs 
	cout << "BST - All Books in Sorted Order (Random Insertion): " << endl << endl; 
	bst_inOrder(bstRandom);
	cout << endl;

	cout << "BST Search ID 64 (Random)" << endl;
	int bstStepsRandom = 0;
	BSTNode* bstResult = bst_search(bstRandom, 64, bstStepsRandom);
	if (bstResult) cout << "Found: " << bstResult->book.title << endl << endl;
	else cout << "That book isn't found" << endl << endl;

	cout << "BST Delete ID 23 (Random)" << endl;
	bstRandom = bst_deleteFunc(bstRandom, 23);
	bst_inOrder(bstRandom);
	cout << endl;

	cout << "BST Find in Range [20, 70] (Random) " << endl;
	cout << "Books with ID in range [20, 70]:" << endl;
	bst_findInRange(bstRandom, 20, 70);
	cout << endl;

	cout << "BST Find Closest to ID 50 (Random)" << endl;
	bst_findClosest(bstRandom, 50);
	cout << endl;

	// AVL: Random IDs 
	
	cout << "AVL - All Books in Sorted Order (Random Insertion):" << endl << endl;
	avlRandom.inorder();
	cout << endl;

	cout << "AVL Search ID 64 (Random)" << endl;
	avlRandom.search(64);
	cout << endl;

	cout << "AVL Delete ID 23 (Random)" << endl;
	avlRandom.deleteBook(23);
	avlRandom.inorder();
	cout << endl;

	cout << "AVL Find in Range [20, 70] (Random)" << endl;
	avlRandom.findInRange(20, 70);
	cout << endl;

	cout << "AVL Find Closest to ID 50 (Random)" << endl;
	avlRandom.findClosest(50);
	cout << endl;



	// CASE 2: Sorted IDs

	BSTNode* bstSorted = nullptr;
	AVLTree avlSorted;

	Book r1 = { 1, "Harry Potter", "J.K Rowling" };
	Book r2 = { 2, "Atomic Habits", "James Clear" };
	Book r3 = { 3, "Clean Code", "Robert Martin" };
	Book r4 = { 4, "The Hobbit", "Tolkien" };
	Book r5 = { 5, "1984", "George Orwell" };
	Book r6 = { 6, "To Kill a Mockingbird", "Harper Lee" };
	Book r7 = { 7, "Wuthering Heights", "Emily Brontë" };
	Book r8 = { 8, "Brave New World", "Aldous Huxley" };
	Book r9 = { 9, "The Road", "Cormac McCarthy" };
	Book r10 = { 10, "Never Let Me Go", "Kazuo Ishiguro" };
	Book r11 = { 11, "Atonement", "Ian McEwan" };
	Book r12 = { 12, "Catch-22", "Joseph Heller" };
	Book r13 = { 13, "Frankenstein", "Mary Shelley" };
	Book r14 = { 14, "The Count of Monte Cristo", "Alexandre Dumas" };
	Book r15 = { 15, "A Tale of Two Cities", "Charles Dickens" };
	Book r16 = { 16, "The Rules of Life", "Richard Templar" };
	Book r17 = { 17, "Pride and Prejudice", "Jane Austen" };
	Book r18 = { 18, "War and Peace", "Leo Tolstoy" };
	Book r19 = { 19, "Animal Farm", "George Orwell" };
	Book r20 = { 20, "The Lord of the Rings", "J.R.R. Tolkien" };

	// insert (BST)
	bstSorted = bst_insert(bstSorted, r1);
	bstSorted = bst_insert(bstSorted, r2);
	bstSorted = bst_insert(bstSorted, r3);
	bstSorted = bst_insert(bstSorted, r4);
	bstSorted = bst_insert(bstSorted, r5);
	bstSorted = bst_insert(bstSorted, r6);
	bstSorted = bst_insert(bstSorted, r7);
	bstSorted = bst_insert(bstSorted, r8);
	bstSorted = bst_insert(bstSorted, r9);
	bstSorted = bst_insert(bstSorted, r10);
	bstSorted = bst_insert(bstSorted, r11);
	bstSorted = bst_insert(bstSorted, r12);
	bstSorted = bst_insert(bstSorted, r13);
	bstSorted = bst_insert(bstSorted, r14);
	bstSorted = bst_insert(bstSorted, r15);
	bstSorted = bst_insert(bstSorted, r16);
	bstSorted = bst_insert(bstSorted, r17);
	bstSorted = bst_insert(bstSorted, r18);
	bstSorted = bst_insert(bstSorted, r19);
	bstSorted = bst_insert(bstSorted, r20);

	// insert (AVL)
	avlSorted.insert(r1);
	avlSorted.insert(r2);
	avlSorted.insert(r3);
	avlSorted.insert(r4);
	avlSorted.insert(r5);
	avlSorted.insert(r6);
	avlSorted.insert(r7);
	avlSorted.insert(r8);
	avlSorted.insert(r9);
	avlSorted.insert(r10);
	avlSorted.insert(r11);
	avlSorted.insert(r12);
	avlSorted.insert(r13);
	avlSorted.insert(r14);
	avlSorted.insert(r15);
	avlSorted.insert(r16);
	avlSorted.insert(r17);
	avlSorted.insert(r18);
	avlSorted.insert(r19);
	avlSorted.insert(r20);

	// BST: Sorted IDs
	cout << "BST - All Books in Sorted Order (Sorted Insertion):" << endl << endl;
	bst_inOrder(bstSorted);
	cout << endl;

	cout << "BST Search ID 12 (Sorted)" << endl;
	int bstStepsSorted = 0;
	BSTNode* bstResult2 = bst_search(bstSorted, 12, bstStepsSorted);
	if (bstResult2) cout << "Found: " << bstResult2->book.title << endl << endl;
	else cout << "That book isn't found" << endl << endl;

	cout << "BST Delete ID 10 (Sorted)" << endl;
	bstSorted = bst_deleteFunc(bstSorted, 10);
	bst_inOrder(bstSorted);
	cout << endl;

	cout << "BST Find in Range [5, 15] (Sorted)" << endl;
	cout << "Books with ID in range [5, 15]:" << endl;
	bst_findInRange(bstSorted, 5, 15);
	cout << endl;

	cout << "BST Find Closest to ID 13 (Sorted)" << endl;
	bst_findClosest(bstSorted, 13);
	cout << endl;

	// AVL: Sorted IDs 
	cout << "AVL - All Books in Sorted Order (Sorted Insertion):" << endl << endl;
	avlSorted.inorder();
	cout << endl;

	cout << "AVL Search ID 12 (Sorted)" << endl;
	avlSorted.search(12);
	cout << endl;

	cout << "AVL Delete ID 10 (Sorted)" << endl;
	avlSorted.deleteBook(10);
	avlSorted.inorder();
	cout << endl;

	cout << "AVL Find in Range [5, 15] (Sorted)" << endl;
	avlSorted.findInRange(5, 15);
	cout << endl;

	cout << "AVL Find Closest to ID 13 (Sorted)" << endl;
	avlSorted.findClosest(13);
	cout << endl;

	
	// COMPARISON: Height & Search Steps

	// search ID to use for step comparison (same ID searched in both cases)
	int searchId = 12;

	int bstRandomSteps = 0;
	bst_search(bstRandom, searchId, bstRandomSteps);

	int bstSortedSteps2 = 0;
	bst_search(bstSorted, searchId, bstSortedSteps2);

	cout << "\nRandom IDs:" << endl << endl;
	cout << "BST Height: " << bst_height(bstRandom) << endl;
	cout << "AVL Height: " << avlRandom.getHeight() << endl;
	cout << "Search Steps (BST): " << bstRandomSteps << endl;
	cout << "Search Steps (AVL): " << avlRandom.searchSteps(searchId) << endl;

	cout << "\nSorted IDs:" << endl << endl;
	cout << "BST Height: " << bst_height(bstSorted) << endl;
	cout << "AVL Height: " << avlSorted.getHeight() << endl;
	cout << "Search Steps (BST): " << bstSortedSteps2 << endl;
	cout << "Search Steps (AVL): " << avlSorted.searchSteps(searchId) << endl;

	return 0;
}
