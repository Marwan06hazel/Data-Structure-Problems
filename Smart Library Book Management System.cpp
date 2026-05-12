#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Book structure to hold book information
struct Book
{
    int id;
    string title;
    string author;
};

// AVL Tree Node structure
struct AVLNode
{
    Book book;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Book b) : book(b), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree class for managing books
class AVLTree
{
private:
    AVLNode *root;

    int height(AVLNode *node)
    {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode *node)
    {
        return node ? height(node->left) - height(node->right) : 0; // Balance factor >> height of left subtree - height of right subtree
    }

    // Right rotation for LL imbalance
    AVLNode *rightRotate(AVLNode *A)
    {
        // Reading the nodes involved in the rotation
        AVLNode *B = A->left;  // B is the left child of A, which will become the new root of this subtree
        AVLNode *C = B->right; // C must stay fixed as the left child of B

        // Rotation process
        B->right = A; // B becomes the new root, so A becomes the right child of B
        A->left = C;  // C replaces B as the left child of A (the root)

        // Update heights after rotation
        A->height = 1 + max(height(A->left), height(A->right));
        B->height = 1 + max(height(B->left), height(B->right));

        return B;
    }

    // Left rotation for RR imbalance
    AVLNode *leftRotate(AVLNode *A)
    {
        // Reading the nodes involved in the rotation
        AVLNode *B = A->right; // B is the right child of A, which will become the new root of this subtree
        AVLNode *C = B->left;  // C must stay fixed as the right child of B

        // Rotation process
        B->left = A;  // B becomes the new root, so A becomes the left child of B
        A->right = C; // C replaces B as the right child of A (the root)

        // Update heights after rotation
        A->height = 1 + max(height(A->left), height(A->right));
        B->height = 1 + max(height(B->left), height(B->right));

        return B;
    }

    // LR Rotation for LR imbalance
    AVLNode *leftRightRotate(AVLNode *A)
    {
        // Reading the nodes involved in the rotation
        AVLNode *B = A->left;  // B is the left child of A, which will become the new root of this subtree
        AVLNode *C = B->right; // C is the right child of B, which will become the left child of A after the first rotation

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
    AVLNode *rightLeftRotate(AVLNode *A)
    {
        // Reading the nodes involved in the rotation
        AVLNode *B = A->right; // B is the right child of A,
        AVLNode *C = B->left;  // C is the left child of B, which will become the right child of A after the first rotation

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
    AVLNode *insert(AVLNode *node, Book book)
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
    AVLNode *minValueNode(AVLNode *node)
    {
        AVLNode *current = node;

        while (current->left)
        {
            current = current->left; // Move to the leftmost node
        }

        return current; // Return the leftmost node, which is the minimum value node
    }

    // Delete a book from the AVL tree and balance it
    AVLNode *deleteNode(AVLNode *node, int id)
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
                AVLNode *temp = node->left ? node->left : node->right; // Get the non-null child

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
                AVLNode *temp = minValueNode(node->right);

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
    AVLNode *search(AVLNode *node, int id)
    {
        if (!node || node->book.id == id)
        {
            return node; // If the node is null or the book ID matches, return the node
        }

        if (id < node->book.id)
        {
            return search(node->left, id); // Search in the left subtree
        }
        else
        {
            return search(node->right, id); // Search in the right subtree
        }
    }

    // Inorder traversal to display books in sorted order
    void inorder(AVLNode *node)
    {
        if (node)
        {
            inorder(node->left);                                                                                             // Traverse the left subtree
            cout << "ID: " << node->book.id << ", Title: " << node->book.title << ", Author: " << node->book.author << endl; // Display the book information
            inorder(node->right);                                                                                            // Traverse the right subtree
        }
    }

    // Find books in ID range
    void findInRange(AVLNode *node, int low, int high)
    {
        if (!node)
            return;

        if (node->book.id > low)
        {                                       // If the book ID is greater than the low value, search in the left subtree
            findInRange(node->left, low, high); // Search in the left subtree
        }

        if (node->book.id >= low && node->book.id <= high)
        {                                                                                                                    // If the book ID is within the range, display it
            cout << "ID: " << node->book.id << ", Title: " << node->book.title << ", Author: " << node->book.author << endl; // Display the book information
        }

        if (node->book.id < high)
        {                                        // If the book ID is less than the high value, search in the right subtree
            findInRange(node->right, low, high); // Search in the right subtree
        }
    }

    // Find closest book ID
    void findClosest(AVLNode *node, int target, AVLNode *&closest)
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
        AVLNode *result = search(root, id);
        if (result)
            cout << "Found: ID " << result->book.id << ", Title: " << result->book.title << ", Author: " << result->book.author << endl;
        else
            cout << "Book with ID " << id << " not found." << endl;
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

        AVLNode *closest = root; // Start with the root as the closest
        findClosest(root, target, closest);
        cout << "Closest book to ID " << target << ": ID " << closest->book.id << ", Title: " << closest->book.title << ", Author: " << closest->book.author << endl;
    }
};

int main()
{
    AVLTree tree;

    // Sample books
    vector<Book> books = {
        {142, "The Great Gatsby", "F. Scott Fitzgerald"},
        {57, "To Kill a Mockingbird", "Harper Lee"},
        {318, "1984", "George Orwell"},
        {204, "Pride and Prejudice", "Jane Austen"},
        {89, "The Catcher in the Rye", "J.D. Salinger"},
        {275, "Brave New World", "Aldous Huxley"},
        {163, "The Hobbit", "J.R.R. Tolkien"},
        {31, "Fahrenheit 451", "Ray Bradbury"},
        {390, "The Lord of the Rings", "J.R.R. Tolkien"},
        {112, "Animal Farm", "George Orwell"},
        {23, "The Alchemist", "Paulo Coelho"},
        {445, "Don Quixote", "Miguel de Cervantes"},
        {178, "Moby Dick", "Herman Melville"},
        {67, "War and Peace", "Leo Tolstoy"},
        {512, "Crime and Punishment", "Fyodor Dostoevsky"},
        {234, "The Odyssey", "Homer"},
        {99, "Jane Eyre", "Charlotte Bronte"},
        {367, "Wuthering Heights", "Emily Bronte"},
        {145, "Great Expectations", "Charles Dickens"},
        {289, "The Count of Monte Cristo", "Alexandre Dumas"},
        {55, "Les Miserables", "Victor Hugo"},
        {423, "Anna Karenina", "Leo Tolstoy"},
        {188, "Dracula", "Bram Stoker"},
        {76, "Frankenstein", "Mary Shelley"},
        {333, "The Picture of Dorian Gray", "Oscar Wilde"},
        {211, "Ulysses", "James Joyce"},
        {456, "Lolita", "Vladimir Nabokov"},
        {134, "Of Mice and Men", "John Steinbeck"},
        {298, "The Grapes of Wrath", "John Steinbeck"},
        {171, "Catch-22", "Joseph Heller"},
    };

    // Insert all books
    cout << "--- Inserting Books ---" << endl;
    for (auto &b : books)
        tree.insert(b);

    // Inorder traversal (sorted by ID)
    cout << "\n--- Inorder Traversal (Sorted by ID) ---" << endl;
    tree.inorder();

    // Search for an existing book
    cout << "\n--- Search ID 163 ---" << endl;
    tree.search(163);

    // Search for a non-existing book
    cout << "\n--- Search ID 999 ---" << endl;
    tree.search(999);

    // Delete a book
    cout << "\n--- Delete ID 204 ---" << endl;
    tree.deleteBook(204);
    tree.inorder();

    // Tree height
    cout << "\n--- Tree Height ---" << endl;
    cout << "Height: " << tree.getHeight() << endl;

    // Find books within a range
    cout << "\n--- Find Books with ID in range [100, 300] ---" << endl;
    tree.findInRange(100, 300);

    // Find the closest book to a target ID
    cout << "\n--- Find Closest Book to ID 250 ---" << endl;
    tree.findClosest(250);

    return 0;
}