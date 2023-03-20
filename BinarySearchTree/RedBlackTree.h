#pragma once

#include "ObjectFreeList.h"
#include <cassert>

template <typename T>
class RedBlackTree
{
private:
	enum class EColor : bool
	{
		Red = true,
		Black = false
	};
	template <typename T>
	struct Node
	{
		T Data{};
		Node<T>* Parent = nullptr;
		Node<T>* Left = nullptr;
		Node<T>* Right = nullptr;
		EColor Color = EColor::Black;
	};
public:
	RedBlackTree()
	{
		mNill = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
	}

	~RedBlackTree()
	{
		delete mNill;
	}

	inline int GetSize() const { return mSize; }

	bool IsContain(T data) const
	{
		if (mRoot == nullptr)
		{
			return false;
		}

		return isContainRecursive(data, mRoot);
	}

	bool Insert(T data)
	{
		bool ret = true;

		if (mRoot == nullptr)
		{
			Node<T>* newNode = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
			newNode->Data = data;
			newNode->Parent = nullptr;
			newNode->Left = mNill;
			newNode->Right = mNill;
			newNode->Color = EColor::Black;
			mRoot = newNode;

			ret = true;
		}
		else
		{
			ret = insertRecursive(data, mRoot);
		}

		if (ret == true)
		{
			mSize++;
		}

		return ret;
	}

	std::string GetInorderString() const
	{
		std::string result = "";

		if (mRoot != nullptr)
		{
			getInorderStringRecursive(mRoot, result);
		}

		return result;
	}
private:
	void getInorderStringRecursive(const Node<T>* root, std::string& result) const
	{
		assert(root != nullptr);

		if (root->Left != mNill)
		{
			getInorderStringRecursive(root->Left, result);
		}

		result += "[";
		result += std::to_string(root->Data);
		result += "] ";

		if (root->Right != mNill)
		{
			getInorderStringRecursive(root->Right, result);
		}
	}

	bool isContainRecursive(T data, const Node<T>* rootOrNill) const
	{
		assert(rootOrNill != nullptr);

		if (rootOrNill == mNill)
		{
			return false;
		}
		else if (data == rootOrNill->Data)
		{
			return true;
		}
		else if (data < rootOrNill->Data)
		{
			return isContainRecursive(data, rootOrNill->Left);
		}
		else
		{
			return isContainRecursive(data, rootOrNill->Right);
		}
	}

	bool insertRecursive(T data, Node<T>* root)
	{
		assert(root != nullptr);

		if (data == root->Data)
		{
			return false;
		}

		if (data < root->Data)
		{
			if (root->Left == mNill)
			{
				Node<T>* newNode = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
				newNode->Parent = root;
				newNode->Data = data;
				newNode->Left = mNill;
				newNode->Right = mNill;
				newNode->Color = EColor::Red;

				root->Left = newNode;
				balanceRecurisve(newNode);
				mRoot->Color = EColor::Black;
			}
			else
			{
				return insertRecursive(data, root->Left);
			}
		}
		else
		{
			if (root->Right == mNill)
			{
				Node<T>* newNode = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
				newNode->Parent = root;
				newNode->Data = data;
				newNode->Left = mNill;
				newNode->Right = mNill;
				newNode->Color = EColor::Red;

				root->Right = newNode;
				balanceRecurisve(newNode);
				mRoot->Color = EColor::Black;
			}
			else
			{
				return insertRecursive(data, root->Right);
			}
		}

		return true;
	}

	void balanceRecurisve(Node<T>* node)
	{
		assert(node != nullptr);

		if (node == mRoot)
		{
			return;
		}

		assert(node->Parent != nullptr);
		assert(node->Parent != mNill);

		if (node->Parent->Color == EColor::Black)
		{
			return;
		}

		Node<T>* parent = node->Parent;
		Node<T>* grand = parent->Parent;
		Node<T>* uncle;
		bool bMyParentIsLeftChildOfGrand;
		if (grand->Left == parent)
		{
			uncle = grand->Right;
			bMyParentIsLeftChildOfGrand = true;
		}
		else
		{
			uncle = grand->Left;
			bMyParentIsLeftChildOfGrand = false;
		}

		// 1. 부모도 레드, 삼촌도 레드라면 색을 뒤집고 재귀
		if (uncle->Color == EColor::Red)
		{
			parent->Color = EColor::Black;
			uncle->Color = EColor::Black;
			grand->Color = EColor::Red;
			balanceRecurisve(grand);
			return;
		}

		if (bMyParentIsLeftChildOfGrand)
		{
			if (parent->Right == node)
			{
				// 회전해서 3번 상황으로 맞춘다
				rotateLeft(parent);

				// 3번 상황 : grand를 기준으로 우회전
				node->Color = EColor::Black;
				parent->Color = EColor::Red;
				grand->Color = EColor::Red;
				rotateRight(grand);
				//balanceRecurisve(node);
			}
			else
			{
				// 3번 상황 : grand를 기준으로 우회전
				parent->Color = EColor::Black;
				grand->Color = EColor::Red;
				rotateRight(grand);
				//balanceRecurisve(parent);
			}
		}
		else
		{
			if (parent->Left == node)
			{
				// 회전해서 3번 상황으로 맞춘다
				rotateRight(parent);

				// 3번 상황 : grand를 기준으로 우회전
				node->Color = EColor::Black;
				parent->Color = EColor::Red;
				grand->Color = EColor::Red;
				rotateLeft(grand);
				//balanceRecurisve(node);
			}
			else
			{
				// 3번 상황 : grand를 기준으로 우회전
				parent->Color = EColor::Black;
				grand->Color = EColor::Red;
				rotateLeft(grand);
				//balanceRecurisve(parent);
			}
		}
	}

	void rotateRight(Node<T>* node)
	{
		assert(node->Left != mNill);

		Node<T>* oldParent = node->Parent;
		Node<T>* oldLeftChild = node->Left;
		Node<T>* oldLeftChildRight = node->Left->Right;

		if (node == mRoot)
		{
			mRoot = oldLeftChild;
		}
		else if (oldParent->Left == node)
		{
			oldParent->Left = oldLeftChild;
		}
		else
		{
			oldParent->Right = oldLeftChild;
		}
		oldLeftChild->Parent = oldParent;

		node->Parent = oldLeftChild;
		oldLeftChild->Right = node;
		oldLeftChildRight->Parent = node;
		node->Left = oldLeftChildRight;
	}

	void rotateLeft(Node<T>* node)
	{
		assert(node->Right != mNill);

		Node<T>* oldParent = node->Parent;
		Node<T>* oldRightChild = node->Right;
		Node<T>* oldRightChildleft = node->Right->Left;

		if (node == mRoot)
		{
			mRoot = oldRightChild;
		}
		else if (oldParent->Left == node)
		{
			oldParent->Left = oldRightChild;
		}
		else
		{
			oldParent->Right = oldRightChild;
		}
		oldRightChild->Parent = oldParent;

		node->Parent = oldRightChild;
		oldRightChild->Left = node;
		oldRightChildleft->Parent = node;
		node->Right = oldRightChildleft;
	}
private:
	Node<T>* mRoot = nullptr;
	Node<T>* mNill = nullptr;
	int mSize = 0;
};