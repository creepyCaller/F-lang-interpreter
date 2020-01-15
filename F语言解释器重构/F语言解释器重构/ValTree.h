#pragma once
typedef double ET;
typedef enum { Red, Black }NColor;
typedef struct RedBlackTree
{
	char name[256];
	ET data;
	struct RedBlackTree *left, *right, *p;
	NColor color;
} TN;

class ValTree
{
protected:
	TN * root; //树的根节点指针
	TN *nil; //哨兵结点指针
	TN * createNode(ET e, char *name);
	void leftRotate(TN *x);
	void rightRotate(TN *x);
	void InsertFixer(TN *z);
	void setRoot(TN *t);
	void DeleteTree(TN *t);
public:
	ValTree();
	~ValTree();
	TN *getRoot();
	bool Serch(char *name);
	bool Serch(char *name, ET *ret);
	double getVal(char * name);
	ET ChangeSerch(char *name, double increment);
	void Insert(ET e, char *name);
	void DeleteTree();
	void Order(TN * t);
};

