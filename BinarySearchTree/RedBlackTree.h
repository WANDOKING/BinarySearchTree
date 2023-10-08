// version 1.0.0
#pragma once

#include <cassert>
#include <stack> // ���� �׽�Ʈ�� ���ؼ� ���

template <typename T>
class RedBlackTree final
{
    // ����� ����
    enum class EColor : bool
    {
        Red = true,
        Black = false
    };

    // ���
    struct Node
    {
        T       Data{};
        Node*   Parent = nullptr;
        Node*   Left = nullptr;
        Node*   Right = nullptr;
        EColor	Color = EColor::Black;
    };

public:
    RedBlackTree()
    {
        mNIL = new Node;
        mRoot = mNIL;
    }

    ~RedBlackTree()
    {
        if (mSize > 0)
        {
            clearRecursive(mRoot);
        }

        delete mNIL;
    }

    int			GetSize() const { return mSize; }

    bool		IsContain(T data) const
    {
        if (mSize == 0)
        {
            return false;
        }

        return isContainRecursive(data, mRoot);
    }

    bool		Insert(T data)
    {
        assert(mRoot != nullptr);

        bool ret = true;

        if (mSize == 0)
        {
            Node* newNode = new Node;
            newNode->Data = data;
            newNode->Parent = mNIL;
            newNode->Left = mNIL;
            newNode->Right = mNIL;
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

    bool		Delete(T data)
    {
        Node* retDeleteBST = deleteBST(data);

        if (retDeleteBST == nullptr)
        {
            return false;
        }

        if (mbIsRedLastDeleted == false)
        {
            balanceDeleteRecursive(retDeleteBST);
        }

        return true;
    }
private:

    bool		isContainRecursive(T data, const Node* rootOrNIL) const
    {
        assert(rootOrNIL != nullptr);

        if (rootOrNIL == mNIL)
        {
            return false;
        }
        else if (data == rootOrNIL->Data)
        {
            return true;
        }
        else if (data < rootOrNIL->Data)
        {
            return isContainRecursive(data, rootOrNIL->Left);
        }
        else
        {
            return isContainRecursive(data, rootOrNIL->Right);
        }
    }

    bool		insertRecursive(T data, Node* root)
    {
        assert(root != nullptr);
        assert(root != mNIL);

        if (data == root->Data)
        {
            return false;
        }

        if (data < root->Data)
        {
            if (root->Left == mNIL)
            {
                Node* newNode = new Node;
                newNode->Parent = root;
                newNode->Data = data;
                newNode->Left = mNIL;
                newNode->Right = mNIL;
                newNode->Color = EColor::Red;

                root->Left = newNode;
                balanceInsertRecursive(newNode);
                mRoot->Color = EColor::Black;
            }
            else
            {
                return insertRecursive(data, root->Left);
            }
        }
        else
        {
            if (root->Right == mNIL)
            {
                Node* newNode = new Node;
                newNode->Parent = root;
                newNode->Data = data;
                newNode->Left = mNIL;
                newNode->Right = mNIL;
                newNode->Color = EColor::Red;

                root->Right = newNode;
                balanceInsertRecursive(newNode);
                mRoot->Color = EColor::Black;
            }
            else
            {
                return insertRecursive(data, root->Right);
            }
        }

        return true;
    }

    // BST ��� ����, ������ �ڸ��� ��带 ��ȯ�Ѵ� -> �� ��带 �������� �뷱�� ����
    // ������ �����ߴٸ� nullptr�� ��ȯ
    Node* deleteBST(T data)
    {
        if (mSize == 0)
        {
            return nullptr;
        }

        Node* ret = deleteBstRecursive(data, mRoot);
        if (ret != nullptr)
        {
            mSize--;
        }

        return ret;
    }

    Node* deleteBstRecursive(T data, Node* root)
    {
        if (data == root->Data)
        {
            Node* ret;

            if (root->Left != mNIL && root->Right != mNIL)
            {
                Node* maxNode = getMaxNodeRecursive(root->Left);
                root->Data = maxNode->Data;
                ret = deleteNodeWithLessThanOneChild(maxNode);
            }
            else
            {
                root = deleteNodeWithLessThanOneChild(root);
                ret = root;
            }

            return ret;
        }
        else if (data < root->Data)
        {
            if (root->Left != mNIL)
            {
                return deleteBstRecursive(data, root->Left);
            }
        }
        else /*(root->Data < data)*/
        {
            if (root->Right != mNIL)
            {
                return deleteBstRecursive(data, root->Right);
            }
        }

        return nullptr;
    }

    // �ڽ��� �ϳ� ������ ��� ����, �� ��ġ�� ���ο� ��� �ּҸ� ��ȯ (NIL�� �� ����)
    Node* deleteNodeWithLessThanOneChild(Node* node)
    {
        assert(node != nullptr);
        assert(node != mNIL);

        Node* toDelete = node;
        Node* ret;

        if (node->Left == mNIL && node->Right == mNIL)
        {
            ret = mNIL;
        }
        else if (node->Left != mNIL)
        {
            assert(node->Right == mNIL);
            ret = node->Left;
        }
        else
        {
            assert(node->Left == mNIL);
            ret = node->Right;
        }

        ret->Parent = node->Parent;
        if (toDelete != mRoot)
        {
            if (node->Parent->Left == node)
            {
                node->Parent->Left = ret;
            }
            else
            {
                node->Parent->Right = ret;
            }
        }
        else
        {
            mRoot = ret;
        }

        if (toDelete->Color == EColor::Red)
        {
            mbIsRedLastDeleted = true;
        }
        else
        {
            mbIsRedLastDeleted = false;
        }

        delete toDelete;

        return ret;
    }

    // �ִ밪�� ���� ��带 ��ȯ
    Node* getMaxNodeRecursive(Node* root) const
    {
        assert(root != nullptr);
        assert(root != mNIL);

        if (root->Right == mNIL)
        {
            return root;
        }

        return getMaxNodeRecursive(root->Right);
    }

    // ��� ��� ���� (���� ��ȸ)
    void		clearRecursive(Node* root)
    {
        assert(root != mNIL);

        if (root->Left != mNIL)
        {
            clearRecursive(root->Left);
        }

        if (root->Right != mNIL)
        {
            clearRecursive(root->Right);
        }

        delete root;
    }
#pragma region Ʈ�� �뷱�� ����
private:
    // ������ ȸ��
    void rotateRight(Node* node)
    {
        assert(node->Left != nullptr);

        Node* oldParent = node->Parent;
        Node* oldLeftChild = node->Left;
        Node* oldLeftChildRight = node->Left->Right;

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

    // ���� ȸ��
    void rotateLeft(Node* node)
    {
        assert(node->Right != nullptr);

        Node* oldParent = node->Parent;
        Node* oldRightChild = node->Right;
        Node* oldRightChildleft = node->Right->Left;

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

    // ���� ���� �� �뷱��
    void balanceInsertRecursive(Node* node)
    {
        assert(node != nullptr);

        if (node == mRoot)
        {
            return;
        }

        assert(node->Parent != nullptr);
        assert(node->Parent != mNIL);

        if (node->Parent->Color == EColor::Black)
        {
            return;
        }

        Node* parent = node->Parent;
        Node* grand = parent->Parent;
        Node* uncle;
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

        // 1. �θ� ����, ���̵� ������ ���� ������ ���
        if (uncle->Color == EColor::Red)
        {
            parent->Color = EColor::Black;
            uncle->Color = EColor::Black;
            grand->Color = EColor::Red;
            balanceInsertRecursive(grand);
            return;
        }

        if (bMyParentIsLeftChildOfGrand)
        {
            if (parent->Right == node)
            {
                // ȸ���ؼ� 3�� ��Ȳ���� �����
                rotateLeft(parent);

                // 3�� ��Ȳ : grand�� �������� ��ȸ��
                node->Color = EColor::Black;
                parent->Color = EColor::Red;
                grand->Color = EColor::Red;
                rotateRight(grand);
                //balanceInsertRecursive(node);
            }
            else
            {
                // 3�� ��Ȳ : grand�� �������� ��ȸ��
                parent->Color = EColor::Black;
                grand->Color = EColor::Red;
                rotateRight(grand);
                //balanceInsertRecursive(parent);
            }
        }
        else
        {
            if (parent->Left == node)
            {
                // ȸ���ؼ� 3�� ��Ȳ���� �����
                rotateRight(parent);

                // 3�� ��Ȳ : grand�� �������� ��ȸ��
                node->Color = EColor::Black;
                parent->Color = EColor::Red;
                grand->Color = EColor::Red;
                rotateLeft(grand);
                //balanceInsertRecursive(node);
            }
            else
            {
                // 3�� ��Ȳ : grand�� �������� ��ȸ��
                parent->Color = EColor::Black;
                grand->Color = EColor::Red;
                rotateLeft(grand);
                //balanceInsertRecursive(parent);
            }
        }
    }

    // ���� ���� �� �뷱��
    void balanceDeleteRecursive(Node* node)
    {
        assert(node != nullptr);

        // 1. ���� ��尡 ������ ��� ������ �ٲٰ� ����
        if (node->Color == EColor::Red)
        {
            node->Color = EColor::Black;
            return;
        }

        // 1-#. ��Ʈ�� ��� ��
        if (node == mRoot)
        {
            return;
        }

        Node* parent = node->Parent;
        Node* sibling;
        bool bIAmLeftChild;
        if (parent->Left == node)
        {
            bIAmLeftChild = true;
            sibling = parent->Right;
        }
        else
        {
            bIAmLeftChild = false;
            sibling = parent->Left;
        }

        // 2. ���� ����� ������ ����
        if (sibling->Color == EColor::Red)
        {
            sibling->Color = EColor::Black;
            parent->Color = EColor::Red;

            if (bIAmLeftChild == true)
            {
                rotateLeft(parent);
            }
            else
            {
                rotateRight(parent);
            }

            balanceDeleteRecursive(node);
            return;
        }

        // 3. ������ ���� �ڽ��� ��
        if (sibling->Left->Color == EColor::Black && sibling->Right->Color == EColor::Black)
        {
            sibling->Color = EColor::Red;
            balanceDeleteRecursive(parent);
            return;
        }

        // 4. ������ ������ �ڽ��� ��
        if (bIAmLeftChild == true && sibling->Right->Color == EColor::Black)
        {
            sibling->Left->Color = EColor::Black;
            sibling->Color = EColor::Red;
            rotateRight(sibling);
            sibling = sibling->Parent;
        }
        else if (bIAmLeftChild == false && sibling->Left->Color == EColor::Black)
        {
            sibling->Right->Color = EColor::Black;
            sibling->Color = EColor::Red;
            rotateLeft(sibling);
            sibling = sibling->Parent;
        }

        // 5. �������� BEST ��Ȳ
        // ������ ��, ������ �ڽ��� ����
        if (bIAmLeftChild == true)
        {
            sibling->Color = parent->Color;
            parent->Color = EColor::Black;
            sibling->Right->Color = EColor::Black;
            rotateLeft(parent);
        }
        else
        {
            sibling->Color = parent->Color;
            parent->Color = EColor::Black;
            sibling->Left->Color = EColor::Black;
            rotateRight(parent);
        }
    }
#pragma endregion

#pragma region ���� �� Ʈ�� ������ �Լ���
public:
    bool		IsValidTree() const
    {
        // ����ִ� Ʈ���� ���� ó��
        if (mSize == 0)
        {
            if (mRoot == mNIL)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // 1. ��Ʈ ��尡 ������?
        if (mRoot->Color != EColor::Black)
        {
            return false;
        }

        // 2. ��Ʈ ����� �θ� NIL����?
        if (mRoot->Parent != mNIL)
        {
            return false;
        }

        // 3. ����-���� ��Ȳ�� �߻��ϴ���?
        // 4. ��� ��ο� ���� �� ���̰� ��������?
        // 5. BST�� �⺻ ������ �����ϴ���?
        // 6. �����Ͱ� ����� ����Ǿ�����?
        int treeBlackDepth = GetBlackDepth();
        return validCheckRecursive(mRoot, treeBlackDepth, 0);
    }

    int			GetMaxDepth() const
    {
        if (mSize == 0)
        {
            return 0;
        }

        std::stack<std::pair<Node*, int>> s;
        s.push(std::make_pair(mRoot, 1));
        int ret = 0;
        while (s.empty() == false)
        {
            Node* node = s.top().first;
            int depth = s.top().second;
            s.pop();

            if (depth > ret)
            {
                ret = depth;
            }

            if (node->Left != mNIL)
            {
                s.push(std::make_pair(node->Left, depth + 1));
            }

            if (node->Right != mNIL)
            {
                s.push(std::make_pair(node->Right, depth + 1));
            }
        }

        return ret;
    }

    int	GetMinDepth() const
    {
        if (mSize == 0)
        {
            return 0;
        }

        std::stack<std::pair<Node*, int>> s;
        s.push(std::make_pair(mRoot, 1));
        int minDepth = INT_MAX;
        while (s.empty() == false)
        {
            Node* node = s.top().first;
            int depth = s.top().second;
            s.pop();

            if (depth > minDepth)
            {
                continue;
            }

            if (node == mNIL)
            {
                minDepth = depth - 1;
            }

            s.push(std::make_pair(node->Left, depth + 1));
            s.push(std::make_pair(node->Right, depth + 1));
        }

        return minDepth;
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

    int	GetBlackDepth() const
    {
        if (mSize == 0)
        {
            return 0;
        }

        return getBlackDepthRecursive(mRoot, 0);
    }

private:

    bool validCheckRecursive(Node* node, const int treeBlackDepth, int currentBlackDepth) const
    {
        assert(node != nullptr);

        if (node == mNIL)
        {
            if (currentBlackDepth == treeBlackDepth)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        if (node != mRoot)
        {
            if (node->Parent == nullptr || node->Parent == mNIL)
            {
                return false;
            }

            if (node != node->Parent->Left && node != node->Parent->Right)
            {
                return false;
            }
        }

        // BST condition check
        // �� Ʈ���� �ߺ��� ��� ���� - �ߺ��� �߻��ص� false
        if (node->Left != mNIL && node->Left->Data >= node->Data)
        {
            return false;
        }
        else if (node->Right != mNIL && node->Right->Data <= node->Data)
        {
            return false;
        }

        // red-red check
        if (node->Color == EColor::Red && (node->Left->Color == EColor::Red || node->Right->Color == EColor::Red))
        {
            return false;
        }

        // black depth check
        if (node->Color == EColor::Black)
        {
            currentBlackDepth++;
        }

        bool bLeftIsValid = validCheckRecursive(node->Left, treeBlackDepth, currentBlackDepth);
        bool bRightIsValid = validCheckRecursive(node->Right, treeBlackDepth, currentBlackDepth);

        return bLeftIsValid && bRightIsValid;
    }

    int	getBlackDepthRecursive(Node* node, int blackDepth) const
    {
        assert(node != nullptr);

        if (node == mNIL)
        {
            return blackDepth;
        }

        if (node->Color == EColor::Black)
        {
            blackDepth++;
        }

        return getBlackDepthRecursive(node->Left, blackDepth);
    }

    void getInorderStringRecursive(const Node* root, std::string& result) const
    {
        assert(root != nullptr);

        if (root->Left != mNIL)
        {
            getInorderStringRecursive(root->Left, result);
        }

        result += "[";
        result += std::to_string(root->Data);
        result += "] ";

        if (root->Right != mNIL)
        {
            getInorderStringRecursive(root->Right, result);
        }
    }
#pragma endregion

private:
    Node* mRoot = nullptr;
    Node* mNIL = nullptr;
    int     mSize = 0;
    bool    mbIsRedLastDeleted = false; // ���� �뷱�� �� ����� ���� ����
};