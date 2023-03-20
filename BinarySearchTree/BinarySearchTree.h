#pragma once

#include "ObjectFreeList.h"
#include <cassert>

template <typename T>
class BinarySearchTree
{
private:
	template <typename T>
	struct Node
	{
		T Data{};
		Node<T>* Left = nullptr;
		Node<T>* Right = nullptr;
	};

public:
	BinarySearchTree() = default;

	~BinarySearchTree()
	{
		if (mRoot != nullptr)
		{
			clearRecursive(mRoot);
		}
	}

	inline int GetSize() const { return mSize; }

	bool IsContain(T data) const
	{
		return isContainRecursive(data, mRoot);
	}

	bool Insert(T data)
	{
		bool ret;

		if (mRoot == nullptr)
		{
			Node<T>* newNode = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
			newNode->Data = data;
			newNode->Left = nullptr;
			newNode->Right = nullptr;

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

	bool Delete(T data)
	{
		if (mRoot == nullptr)
		{
			return false;
		}

		bool ret;

		if (mRoot->Data == data)
		{
			if (mRoot->Left != nullptr && mRoot->Right != nullptr)
			{
				if (mRoot->Left->Right == nullptr)
				{
					mRoot->Data = mRoot->Left->Data;
					mRoot->Left = deleteNodeWithLessThanOneChild(mRoot->Left);
				}
				else
				{
					mRoot->Data = deleteMaxNodeRecursive(mRoot->Left);
				}
			}
			else
			{
				mRoot = deleteNodeWithLessThanOneChild(mRoot);
			}

			ret = true;
		}
		else
		{
			ret = deleteRecursive(data, mRoot);
		}

		if (ret == true)
		{
			mSize--;
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

		if (root->Left != nullptr)
		{
			getInorderStringRecursive(root->Left, result);
		}

		result += "[";
		result += std::to_string(root->Data);
		result += "] ";

		if (root->Right != nullptr)
		{
			getInorderStringRecursive(root->Right, result);
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
			if (root->Left == nullptr)
			{
				Node<T>* newNode = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
				newNode->Data = data;
				newNode->Left = nullptr;
				newNode->Right = nullptr;

				root->Left = newNode;
			}
			else
			{
				return insertRecursive(data, root->Left);
			}
		}
		else
		{
			if (root->Right == nullptr)
			{
				Node<T>* newNode = (Node<T>*)MEMORY_ALLOC(sizeof(Node<T>));
				newNode->Data = data;
				newNode->Left = nullptr;
				newNode->Right = nullptr;

				root->Right = newNode;
			}
			else
			{
				return insertRecursive(data, root->Right);
			}
		}

		return true;
	}

	bool isContainRecursive(T data, const Node<T>* rootOrNull) const
	{
		if (rootOrNull == nullptr)
		{
			return false;
		}
		else if (data == rootOrNull->Data)
		{
			return true;
		}
		else if (data < rootOrNull->Data)
		{
			return isContainRecursive(data, rootOrNull->Left);
		}
		else
		{
			return isContainRecursive(data, rootOrNull->Right);
		}
	}

	bool deleteRecursive(T data, Node<T>* root)
	{
		assert(data != root->Data);

		if (data < root->Data && root->Left == nullptr)
		{
			return false;
		}

		if (root->Data < data && root->Right == nullptr)
		{
			return false;
		}

		if (data < root->Data)
		{
			if (root->Left->Data == data)
			{
				if (root->Left->Left != nullptr && root->Left->Right != nullptr)
				{
					if (root->Left->Left->Right == nullptr)
					{
						root->Left->Data = root->Left->Left->Data;
						root->Left->Left = deleteNodeWithLessThanOneChild(root->Left->Left);
					}
					else
					{
						root->Left->Data = deleteMaxNodeRecursive(root->Left->Left);
					}
				}
				else
				{
					root->Left = deleteNodeWithLessThanOneChild(root->Left);
				}
			}
			else
			{
				return deleteRecursive(data, root->Left);
			}
		}
		else
		{
			if (root->Right->Data == data)
			{
				if (root->Right->Left != nullptr && root->Right->Right != nullptr)
				{
					if (root->Right->Left->Right == nullptr)
					{
						root->Right->Data = root->Right->Left->Data;
						root->Right->Left = deleteNodeWithLessThanOneChild(root->Right->Left);
					}
					else
					{
						root->Right->Data = deleteMaxNodeRecursive(root->Right->Left);
					}
				}
				else
				{
					root->Right = deleteNodeWithLessThanOneChild(root->Right);
				}
			}
			else
			{
				return deleteRecursive(data, root->Right);
			}
		}

		return true;
	}

	// 자식이 하나 이하인 노드 삭제, null 또는 그 위치의 새로운 노드 주소를 반환
	Node<T>* deleteNodeWithLessThanOneChild(Node<T>* node)
	{
		assert(node != nullptr);

		Node<T>* toDelete = node;
		Node<T>* ret;
		if (node->Left == nullptr && node->Right == nullptr)
		{
			ret = nullptr;
		}
		else if (node->Left != nullptr)
		{
			ret = node->Left;
		}
		else
		{
			ret = node->Right;
		}

		MEMORY_FREE(toDelete);

		return ret;
	}

	// 루트 노드가 최댓값이 아닌 트리에 대해 최댓값 노드를 삭제하고 그 값을 반환
	T deleteMaxNodeRecursive(Node<T>* rootRightIsNotNull)
	{
		assert(rootRightIsNotNull != nullptr);
		assert(rootRightIsNotNull->Right != nullptr);

		if (rootRightIsNotNull->Right->Right == nullptr)
		{
			T ret = rootRightIsNotNull->Right->Data;
			rootRightIsNotNull->Right = deleteNodeWithLessThanOneChild(rootRightIsNotNull->Right);
			return ret;
		}

		return deleteMaxNodeRecursive(rootRightIsNotNull->Right);
	}

	// 모든 노드 삭제 (후위 순회)
	void clearRecursive(Node<T>* root)
	{
		assert(root != nullptr);

		if (root->Left != nullptr)
		{
			clearRecursive(root->Left);
		}

		if (root->Right != nullptr)
		{
			clearRecursive(root->Right);
		}

		MEMORY_FREE(root);
	}
private:
	Node<T>* mRoot = nullptr;
	int mSize = 0;
};