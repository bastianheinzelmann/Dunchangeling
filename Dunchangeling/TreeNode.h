#pragma once
#include <vector>
#include <string>

class TreeNode
{
public:
	TreeNode* parent;
	int key;
	int lock;
	std::vector<TreeNode*> children;

	~TreeNode()
	{
		for (int i = 0; i < children.size() && children[i]; i++)
		{
			delete children[i];
		}
	}

	TreeNode()
	{

	}

	std::string toString()
	{
		std::string s;
		//PrintTree()
	}

	void InsertNode();
	void DeleteSubTree();
};