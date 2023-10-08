// version 1.0.0
#pragma once

#include <cassert>

template <typename T>
class BinarySearchTree final
{
	struct Node
	{
		T		Data{};
		Node*	Left = nullptr;
		Node*	Right = nullptr;
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

	inline int	GetSize() const { return mSize; }

	inline bool	IsContain(T data) const { return isContainRecursive(data, mRoot); }

	bool		Insert(T data)
	{
		bool ret;

		if (mRoot == nullptr)
		{
			Node* newNode = new Node;
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

	bool		Delete(T data)
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

private:

	bool	insertRecursive(T data, Node* const root)
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
				Node* newNode = new Node;
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
				Node* newNode = new Node;
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

	bool	isContainRecursive(T data, const Node* const rootOrNull) const
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

	bool	deleteRecursive(T data, Node* const root)
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

	// �ڽ��� �ϳ� ������ ��� ����, null �Ǵ� �� ��ġ�� ���ο� ��� �ּҸ� ��ȯ
	Node*	deleteNodeWithLessThanOneChild(Node* const node)
	{
		assert(node != nullptr);

		Node* toDelete = node;
		Node* ret;
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


		delete toDelete;

		return ret;
	}

	// ��Ʈ ��尡 �ִ��� �ƴ� Ʈ���� ���� �ִ� ��带 �����ϰ� �� ���� ��ȯ
	T		deleteMaxNodeRecursive(Node* const rootRightIsNotNull)
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

	// ��� ��� ���� (���� ��ȸ)
	void	clearRecursive(Node* const root)
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

		delete root;
	}

#pragma region BST ������ �Լ���
public:
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
	void		getInorderStringRecursive(const Node* root, std::string& result) const
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
#pragma endregion

private:
	Node*	mRoot = nullptr;
	int		mSize = 0;
};