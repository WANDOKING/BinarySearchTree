#include <iostream>
#include <cassert>
#include <string>
#include <random>

#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "BstTester.h"

enum EOperation
{
	Delete = 0,
	IsContain = 1,
	// Insert
};

int main(void)
{
	{
		srand(1024);

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
			if (operation == EOperation::IsContain)
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