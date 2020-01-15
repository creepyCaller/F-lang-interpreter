#include "stdafx.h"
#include "ValTree.h"


TN * ValTree::createNode(ET e, char * name)
{
	TN *temp = nil;
	temp = new TN;
	if (temp == nullptr)
	{
		puts("申请给变量的内存失败");
		return nil;
	}
	temp->color = Red;
	temp->data = e;
	strcpy_s(temp->name, name);
	temp->left = nil;
	temp->right = nil;
	temp->p = nil;
	return temp;
}

void ValTree::leftRotate(TN * x)
{
	TN *y = x->right;
	x->right = y->left;
	if (y->left != nil)
		y->left->p = x;
	y->p = x->p;
	if (x->p == nil)
	{
		setRoot(y);
	}
	else if (x == x->p->left)
	{
		x->p->left = y;
	}
	else
	{
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
}

void ValTree::rightRotate(TN * x)
{
	TN *y = x->left;
	x->left = y->right;
	if (y->right != nil)
		y->right->p = x;
	y->p = x->p;
	if (x->p == nil)
	{
		setRoot(y);
	}
	else if (x == x->p->right)
	{
		x->p->right = y;
	}
	else
	{
		x->p->left = y;
	}
	y->right = x;
	x->p = y;
}

void ValTree::InsertFixer(TN * z)
{
	TN *t;
	while (z->p->color == Red)
	{
		if (z->p == z->p->p->left)
		{
			t = z->p->p->right;
			if (t->color == Red)
			{
				z->p->color = Black;
				t->color = Black;
				z->p->p->color = Red;
				z = z->p->p;
			}
			else
			{
				if (z == z->p->right)
				{
					z = z->p;
					leftRotate(z);
				}
				z->p->color = Black;
				z->p->p->color = Red;
				rightRotate(z->p->p);
			}
		}
		else
		{
			t = z->p->p->left;
			if (t->color == Red)
			{
				z->p->color = Black;
				t->color = Black;
				z->p->p->color = Red;
				z = z->p->p;
			}
			else
			{
				if (z == z->p->left)
				{
					z = z->p;
					rightRotate(z);
				}
				z->p->color = Black;
				z->p->p->color = Red;
				leftRotate(z->p->p);
			}
		}
	}
	root->color = Black;
	return;
}

void ValTree::setRoot(TN * t)
{
	root = t;
	return;
}

void ValTree::DeleteTree(TN * t)
{
	if (t != nil)
	{
		DeleteTree(t->left);
		DeleteTree(t->right);
		delete t;
	}
	return;
}

ValTree::ValTree()
{
	nil = new TN;
	nil->p = nullptr;
	nil->color = Black;
	nil->data = -1;
	nil->left = nullptr;
	nil->right = nullptr;
	nil->name[0] = 0;
	root = nil;
}


ValTree::~ValTree()
{
	DeleteTree(root);
}

TN * ValTree::getRoot()
{
	return this->root;
}

bool ValTree::Serch(char * name)
{
	TN *temp = root;
	while (temp != nil)
	{
		if (strcmp(name, temp->name) < 0)
		{
			temp = temp->left;
		}
		else if (strcmp(name, temp->name) > 0)
		{
			temp = temp->right;
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool ValTree::Serch(char * name, ET * ret)
{
	TN *temp = root;
	while (temp != nil)
	{
		if (strcmp(name, temp->name) < 0)
		{
			temp = temp->left;
		}
		else if (strcmp(name, temp->name) > 0)
		{
			temp = temp->right;
		}
		else
		{
			*ret = temp->data;
			return true;
		}
	}
	return false;
}

double ValTree::getVal(char * name)
{
	TN *temp = root;
	while (temp != nil)
	{
		if (strcmp(name, temp->name) < 0)
		{
			temp = temp->left;
		}
		else if (strcmp(name, temp->name) > 0)
		{
			temp = temp->right;
		}
		else
		{
			return temp->data;
		}
	}
	return 0;
}

ET ValTree::ChangeSerch(char * name, double increment)
{
	TN *temp = root;
	while (temp != nil)
	{
		if (strcmp(name, temp->name) < 0)
		{
			temp = temp->left;
		}
		else if (strcmp(name, temp->name) > 0)
		{
			temp = temp->right;
		}
		else
		{
			temp->data = temp->data + increment;
			return temp->data;
		}
	}
	return -1;
}

void ValTree::Insert(ET e, char * name)
{
	TN *temp = root, *temp1 = nil, *z = nil;
	int t = 0;
	z = createNode(e, name);
	while (temp != nil)
	{
		temp1 = temp;
		t = strcmp(z->name, temp->name);
		if (t < 0)
		{
			temp = temp->left;
		}
		else if( t > 0)
		{
			temp = temp->right;
		}
		else if (!t)
		{
			temp->data = z->data;
			return;
		}
	}
	z->p = temp1;
	if (temp1 == nil)
	{
		setRoot(z);
	}
	else if (strcmp(z->name, temp1->name) < 0)
	{
		temp1->left = z;
	}
	else
	{
		temp1->right = z;
	}
	InsertFixer(z);
	return;
}

void ValTree::DeleteTree()
{
	DeleteTree(root);
	setRoot(nil);
	return;
}

void ValTree::Order(TN * t)
{
	if (t != nil)
	{
		Order(t->left);
		printf("%s = %.2lf\n", t->name, t->data);
		Order(t->right);
	}
	return;
}