#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_NODE = 1000;
int array[MAX_NODE+1];
struct node
{
	int val;
	int level;
	node *fa, *left, *right;
};

void insertNode(node *root, node *s)
{
	if(s->val<=root->val)
	{
		if(root->left == NULL)
		{
			root->left = s;
			s->fa = root;
			s->level = root->level + 1;
			return ;
		}
		else
			insertNode(root->left, s);
	}
	else if(s->val>=root->val)
	{
		if(root->right == NULL)
		{
			root->right = s;
			s->fa = root;
			s->level = root->level + 1;
			return ;
		}
		else
			insertNode(root->right, s);
	}
}

node* create(int num)
{
	if(num <= 0)
		return NULL;
	node *root = (node*)(malloc(sizeof(node)));
	root->val = array[0];
	root->level = 1;
	root->fa = NULL;
	root->left = NULL;
	root->right = NULL;
	for(int i=1; i<num; ++i)
	{
		node *s = (node*)(malloc(sizeof(node)));
		s->val = array[i];
		s->left = NULL;
		s->right = NULL;
		insertNode(root, s);
		printf("num=%d level=%d\n", s->val, s->level);
	}
	return root;
}

void inorderTraversal(node *root)
{
	if(root->left != NULL)
		inorderTraversal(root->left);
	printf("%d ", root->val);
	if(root->right != NULL)
		inorderTraversal(root->right);
}

void preorderTraversal(node *root)
{
	if(root != NULL)
	{
		printf("%d ", root->val);
		preorderTraversal(root->left);
		preorderTraversal(root->right);
	}
}

void endorderTraversal(node *root)
{
	if(root != NULL)
	{
		endorderTraversal(root->left);
		endorderTraversal(root->right);
		printf("%d ", root->val);
	}
}

int main()
{
	int n;
	scanf("%d",&n);
	for(int i=0; i<n; ++i)
		scanf("%d",&array[i]);
	printf("Creating ...\n");
	node* root = create(n);
	printf("root=%d level=%d\n", root->val, root->level);
	printf("End Create!\n");
	printf("In Order Traversal ... \n");
	inorderTraversal(root);
	printf("\nPre Order Traversal ... \n");
	preorderTraversal(root);
	printf("\nEnd Order Traversal ... \n");
	endorderTraversal(root);
	printf("\nFinish Traversal!\n");
}
