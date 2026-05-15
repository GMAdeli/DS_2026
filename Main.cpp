#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

struct Book
{
	int isbn;
	float price;
};

struct TreeNode
{
	Book info;
	int balanceFactor;
	TreeNode* leftPointer;
	TreeNode* rightPointer;
};

int validate_isbn()
{
	int isbn;
	while (1)
	{
		printf("Enter ISBN: ");
		if (scanf("%d", &isbn) == 1 && isbn > 0)
		{
			break;
		}
		else
		{
			printf("Invalid input! ISBN must be a positive integer.\n");
			while (getchar() != '\n');
		}
	}
	return isbn;
}

float validate_price()
{
	float price;
	while (1)
	{
		printf("Enter price: ");
		if (scanf("%f", &price) == 1 && price > 0)
		{
			break;
		}
		else
		{
			printf("Invalid input! Price must be a positive number.\n");
			while (getchar() != '\n');
		}
	}
	return price;
}

Book read_book() {
	Book temp;
	temp.isbn = validate_isbn();
	temp.price = validate_price();
	return temp;
}

void print_book(Book b)
{
	printf("ISBN: %d, Price: %.2f\n", b.isbn, b.price);
}

TreeNode* create_node(Book b)
{
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	if (newNode == NULL)
	{
		printf("Memory allocation failed!\n");
		return NULL;
	}
	newNode->info = b;
	newNode->leftPointer = NULL;
	newNode->rightPointer = NULL;
	newNode->balanceFactor = 0;
	return newNode;
}

void print(TreeNode* root)
{
	if (root == NULL)
		return;

	print_book(root->info);
	print(root->leftPointer);
	print(root->rightPointer);
}

int tree_height(TreeNode* root)
{
	if (root == NULL)
		return 0;

	int leftHeight = tree_height(root->leftPointer);
	int rightHeight = tree_height(root->rightPointer);

	return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int balance_factor(TreeNode* root)
{
	if (root == NULL)
		return 0;

	return tree_height(root->leftPointer) - tree_height(root->rightPointer);
}

TreeNode* rotate_left(TreeNode* root)
{
	TreeNode* newRoot = root->rightPointer;
	root->rightPointer = newRoot->leftPointer;
	newRoot->leftPointer = root;

	root->balanceFactor = balance_factor(root);
	newRoot->balanceFactor = balance_factor(newRoot);

	return newRoot;
}

TreeNode* rotate_right(TreeNode* root)
{
	TreeNode* newRoot = root->leftPointer;
	root->leftPointer = newRoot->rightPointer;
	newRoot->rightPointer = root;

	root->balanceFactor = balance_factor(root);
	newRoot->balanceFactor = balance_factor(newRoot);

	return newRoot;
}

TreeNode* rotate_left_right(TreeNode* root)
{
	root->leftPointer = rotate_left(root->leftPointer);
	return rotate_right(root);
}

TreeNode* rotate_right_left(TreeNode* root)
{
	root->rightPointer = rotate_right(root->rightPointer);
	return rotate_left(root);
}

TreeNode* balance(TreeNode* root)
{
	if (root == NULL)
		return NULL;

	int bf = balance_factor(root);

	if (bf == 2)
	{
		if (balance_factor(root->leftPointer) >= 0)
			return rotate_right(root);
		else
			return rotate_left_right(root);
	}
	else if (bf == -2)
	{
		if (balance_factor(root->rightPointer) <= 0)
			return rotate_left(root);
		else
			return rotate_right_left(root);
	}

	return root;
}

TreeNode* insert_node(Book b, TreeNode* root)
{
	if (root == NULL)
		return create_node(b);

	if (b.isbn < root->info.isbn)
		root->leftPointer = insert_node(b, root->leftPointer);
	else if (b.isbn > root->info.isbn)
		root->rightPointer = insert_node(b, root->rightPointer);
	else
	{
		printf("ISBN %d already exists! Cannot insert duplicate.\n", b.isbn);
		return root;
	}

	root->balanceFactor = balance_factor(root);
	root = balance(root);

	return root;
}

void print_inorder(TreeNode* root)
{
	if (root == NULL)
		return;

	print_inorder(root->leftPointer);
	print_book(root->info);
	print_inorder(root->rightPointer);
}

TreeNode* delete_root_node(TreeNode* root)
{
	if (root == NULL)
		return NULL;

	if (root->leftPointer == NULL && root->rightPointer == NULL)
	{
		free(root);
		return NULL;
	}
	else if (root->leftPointer == NULL)
	{
		TreeNode* temp = root->rightPointer;
		free(root);
		return temp;
	}
	else if (root->rightPointer == NULL)
	{
		TreeNode* temp = root->leftPointer;
		free(root);
		return temp;
	}
	else
	{
		TreeNode* parent = root;
		TreeNode* successor = root->rightPointer;

		while (successor->leftPointer != NULL)
		{
			parent = successor;
			successor = successor->leftPointer;
		}

		root->info = successor->info;

		if (parent->leftPointer == successor)
			parent->leftPointer = successor->rightPointer;
		else
			parent->rightPointer = successor->rightPointer;

		free(successor);

		root->balanceFactor = balance_factor(root);
		return balance(root);
	}
}

TreeNode* delete_node(TreeNode* root, int isbn)
{
	if (root == NULL)
	{
		printf("ISBN %d not found in the tree.\n", isbn);
		return NULL;
	}

	if (isbn < root->info.isbn)
		root->leftPointer = delete_node(root->leftPointer, isbn);
	else if (isbn > root->info.isbn)
		root->rightPointer = delete_node(root->rightPointer, isbn);
	else
	{
		printf("Deleting ISBN %d...\n", isbn);
		root = delete_root_node(root);
	}

	if (root != NULL)
	{
		root->balanceFactor = balance_factor(root);
		root = balance(root);
	}

	return root;
}

int main()
{
	TreeNode* root = NULL;
	int choice, isbn;
	Book b;

	do
	{
		printf("\nMenu\n");
		printf("1. Add a book\n");
		printf("2. Display tree\n");
		printf("3. Delete a book by ISBN\n");
		printf("4. Exit\n");
		printf("Enter choice: ");
		if (scanf("%d", &choice) != 1)
		{
			printf("Invalid input!\n");
			while (getchar() != '\n');
			continue;
		}

		switch (choice)
		{
		case 1:
			b = read_book();
			root = insert_node(b, root);
			if (root != NULL)
				printf("Book inserted successfully!\n");
			break;

		case 2:
			if (root == NULL)
				printf("Tree is empty.\n");
			else
			{
				printf("\nTree contents:\n");
				print(root);
				printf("\nTree contents (Sorted by ISBN):\n");
				print_inorder(root);
			}
			break;

		case 3:
			if (root == NULL)
				printf("Tree is empty. Nothing to delete.\n");
			else
			{
				printf("Enter ISBN to delete: ");
				if (scanf("%d", &isbn) == 1 && isbn > 0)
					root = delete_node(root, isbn);
				else
				{
					printf("Invalid ISBN!\n");
					while (getchar() != '\n');
				}
			}
			break;

		case 4:
			printf("Exiting...\n");
			break;

		default:
			printf("Invalid choice!\n");
		}
	} while (choice != 4);

	return 0;
}