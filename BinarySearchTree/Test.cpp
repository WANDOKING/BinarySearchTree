#include <iostream>
#include <cassert>
#include <string>
#include <random>

#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "BstTester.h"

enum EOperation
{
	Delete,
	IsContain,
	// ~ Insert
};

int main(void)
{
	// RedBlackTree
	if (true)
	{
		srand(2468);

		RedBlackTree<int>* bst = new RedBlackTree<int>();
		BstTester<int>* tester = new SetTester<int>();

		long long testCount = 0;
		while (true)
		{
			++testCount;

			bool retBst;
			bool retTester;

			int operation = rand() % 5;
			int randomNumber = rand();

			std::string strOperation;
			if (operation == EOperation::Delete)
			{
				// Delete()
				retBst = bst->Delete(randomNumber);
				retTester = tester->Delete(randomNumber);
				strOperation = "Delete()";
			}
			else if (operation == EOperation::IsContain)
			{
				// IsContain()
				retBst = bst->IsContain(randomNumber);
				retTester = tester->IsContain(randomNumber);
				strOperation = "IsContain()";
			}
			else
			{
				// Insert()
				retBst = bst->Insert(randomNumber);
				retTester = tester->Insert(randomNumber);
				strOperation = "Insert()";
			}

			// 1. return value 비교
			assert(retBst == retTester);

			// 2. size 비교
			int bstSize = bst->GetSize();
			int testerSize = tester->GetSize();
			assert(bstSize == testerSize);

			// 3. 레드-블랙 트리 조건 체크
			assert(bst->IsValidTree());

			// 4. 최소 깊이, 최대 깊이 확인
			int minDepth = bst->GetMinDepth();
			int maxDepth = bst->GetMaxDepth();
			assert(minDepth * 2 >= maxDepth);

			// 5. 중위 순회 결과 비교
			std::string strTester = tester->GetInorderString();
			std::string strBst = bst->GetInorderString();
			assert(strTester == strBst);

			// 로그
			printf("Test = %10lld | OP = %14s | num = %8d | | Size = %8d | BlackDepth = %6d | minDepth = %6d | maxDepth = %6d\n", 
				testCount, strOperation.c_str(), randomNumber, bst->GetSize(), bst->GetBlackDepth(), minDepth, maxDepth);
		}

		delete bst;
		delete tester;
	}

	// BinarySearchTree
	if (false)
	{
		srand(9642);

		BinarySearchTree<int>* bst = new BinarySearchTree<int>();
		BstTester<int>* tester = new SetTester<int>();

		long long testCount = 0;
		while (true)
		{
			++testCount;
			
			bool retBst;
			bool retTester;

			int operation = rand() % 5;
			int randomNumber = rand();

			std::string strOperation;
			if (operation == EOperation::Delete)
			{
				// Delete()
				retBst = bst->Delete(randomNumber);
				retTester = tester->Delete(randomNumber);
				strOperation = "Delete()";
			}
			else if (operation == EOperation::IsContain)
			{
				// IsContain()
				retBst = bst->IsContain(randomNumber);
				retTester = tester->IsContain(randomNumber);
				strOperation = "IsContain()";
			}
			else
			{
				// Insert()
				retBst = bst->Insert(randomNumber);
				retTester = tester->Insert(randomNumber);
				strOperation = "Insert()";
			}

			// 1. return value 비교
			assert(retBst == retTester);

			// 2. size 비교
			int bstSize = bst->GetSize();
			int testerSize = tester->GetSize();
			assert(bstSize == testerSize);

			// 3. 중위 순회 결과 비교
			std::string strTester = tester->GetInorderString();
			std::string strBst = bst->GetInorderString();
			assert(strTester == strBst);

			// 로그
			printf("testCount = %10lld | op = %15s | number = %12d | Size = %10d\n", testCount, strOperation.c_str(), randomNumber, bst->GetSize());
		}
	}

	_CrtDumpMemoryLeaks();
}