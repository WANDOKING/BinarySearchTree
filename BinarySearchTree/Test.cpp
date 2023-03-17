#include <iostream>
#include <cassert>
#include <string>
#include <random>

#include "BinarySearchTree.h"
#include "BstTester.h"

enum EOperation
{
	Delete = 0,
	Insert = 0,
};

int main(void)
{
	{
		srand(1700);

		BinarySearchTree<int>* bst = new BinarySearchTree<int>();
		BstTester<int>* tester = new SetTester<int>();

		long long testCount = 0;
		while (true)
		{
			++testCount;
			int randomNumber = rand();
			bool retBst;
			bool retTester;

			if (rand() % 3 == EOperation::Delete)
			{
				retBst = bst->Delete(randomNumber);
				retTester = tester->Delete(randomNumber);
			}
			else
			{
				retBst = bst->Insert(randomNumber);
				retTester = tester->Insert(randomNumber);
			}

			assert(retBst == retTester);
			assert(bst->GetSize() == tester->GetSize());
			std::string strTester = tester->GetInorderString();
			std::string strBst = bst->GetInorderString();
			assert(strTester == strBst);

			printf("testCount = %lld, Size = %d\n", testCount, bst->GetSize());
		}
	}

	_CrtDumpMemoryLeaks();
}