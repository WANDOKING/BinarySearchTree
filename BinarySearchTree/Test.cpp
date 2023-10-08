#include <iostream>
#include <cassert>
#include <string>
#include <random>
#include <conio.h>

#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "BstTester.h"

#include "Profiler.h"

enum EOperation
{
	Delete,
	IsContain,
	// ~ Insert
};

void TestRedBlackTree(unsigned int randomSeed)
{
	srand(randomSeed);

	time_t timer = time(nullptr);
	struct tm startTime;

	localtime_s(&startTime, &timer);

	RedBlackTree<int>* bst = new RedBlackTree<int>();
	BstTester<int>* tester = new SetTester<int>();

	long long testCount = 0;

	while (true)
	{
		if (_kbhit())
		{
			int input = _getch();
			if (input == 's' || input == 'S' || input == 'q' || input == 'Q')
			{
				time_t nowTimer = time(nullptr);
				struct tm nowTime;

				localtime_s(&nowTime, &nowTimer);

				printf("\n[Stop]\n");
				printf("- Test Started : %04d-%02d-%02d %02d:%02d:%02d\n", startTime.tm_year + 1900, startTime.tm_mon + 1, startTime.tm_mday, startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
				printf("- Now          : %04d-%02d-%02d %02d:%02d:%02d\n", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

				if (input == 's' || input == 'S')
				{
					system("pause");
				}
				else
				{
					break;
				}
			}
		}

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

void TestBinarySearchTree(unsigned int randomSeed)
{
	srand(randomSeed);

	time_t timer = time(nullptr);
	struct tm startTime;

	localtime_s(&startTime, &timer);

	BinarySearchTree<int>* bst = new BinarySearchTree<int>();
	BstTester<int>* tester = new SetTester<int>();

	long long testCount = 0;
	while (true)
	{
		if (_kbhit())
		{
			int input = _getch();
			if (input == 's' || input == 'S' || input == 'q' || input == 'Q')
			{
				time_t nowTimer = time(nullptr);
				struct tm nowTime;

				localtime_s(&nowTime, &nowTimer);

				printf("\n[Stop]\n");
				printf("- Test Started : %04d-%02d-%02d %02d:%02d:%02d\n", startTime.tm_year + 1900, startTime.tm_mon + 1, startTime.tm_mday, startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
				printf("- Now          : %04d-%02d-%02d %02d:%02d:%02d\n", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

				if (input == 's' || input == 'S')
				{
					system("pause");
				}
				else
				{
					break;
				}
			}
		}

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

	delete bst;
	delete tester;
}

void PerformanceTest(unsigned int randomSeed, int operationCount)
{
#pragma region Red Black Tree

	srand(randomSeed);

	RedBlackTree<int> redBlackTree;

	PROFILE_BEGIN(L"RBTree_Insert");
	for (int i = 0; i < operationCount; ++i)
	{
		redBlackTree.Insert(rand());
	}
	PROFILE_END(L"RBTree_Insert");
	wprintf(L"RBTree_Insert Complete\n");

	PROFILE_BEGIN(L"RBTree_IsContatin");
	for (int i = 0; i < operationCount; ++i)
	{
		redBlackTree.IsContain(rand());
	}
	PROFILE_END(L"RBTree_IsContatin");
	wprintf(L"RBTree_IsContatin Complete\n");

	PROFILE_BEGIN(L"RBTree_Delete");
	for (int i = 0; i < operationCount; ++i)
	{
		redBlackTree.Delete(rand());
	}
	PROFILE_END(L"RBTree_Delete");
	wprintf(L"RBTree_Delete Complete\n");

#pragma endregion

#pragma region Binary Search Tree

	srand(randomSeed);

	BinarySearchTree<int> binarySearchTree;

	PROFILE_BEGIN(L"BST_Insert");
	for (int i = 0; i < operationCount; ++i)
	{
		binarySearchTree.Insert(rand());
	}
	PROFILE_END(L"BST_Insert");
	wprintf(L"BST_Insert Complete\n");

	PROFILE_BEGIN(L"BST_IsContatin");
	for (int i = 0; i < operationCount; ++i)
	{
		binarySearchTree.IsContain(rand());
	}
	PROFILE_END(L"BST_IsContatin");
	wprintf(L"BST_IsContatin Complete\n");

	PROFILE_BEGIN(L"BST_Delete");
	for (int i = 0; i < operationCount; ++i)
	{
		binarySearchTree.Delete(rand());
	}
	PROFILE_END(L"BST_Delete");
	wprintf(L"BST_Delete Complete\n");

#pragma endregion

#pragma region std::set

	srand(randomSeed);

	SetTester<int> stdSet;

	PROFILE_BEGIN(L"stdSet_Insert");
	for (int i = 0; i < operationCount; ++i)
	{
		stdSet.Insert(rand());
	}
	PROFILE_END(L"stdSet_Insert");
	wprintf(L"stdSet_Insert Complete\n");

	PROFILE_BEGIN(L"stdSet_IsContatin");
	for (int i = 0; i < operationCount; ++i)
	{
		stdSet.IsContain(rand());
	}
	PROFILE_END(L"stdSet_IsContatin");
	wprintf(L"stdSet_IsContatin Complete\n");

	PROFILE_BEGIN(L"stdSet_Delete");
	for (int i = 0; i < operationCount; ++i)
	{
		stdSet.Delete(rand());
	}
	PROFILE_END(L"stdSet_Delete");
	wprintf(L"stdSet_Delete Complete\n");

#pragma endregion
}

int main(void)
{
	time_t timer = time(nullptr);
	struct tm startTime;

	localtime_s(&startTime, &timer);

	int performanceTestCount = 0;

	while (true)
	{
		if (_kbhit())
		{
			int input = _getch();
			if (input == 's' || input == 'S' || input == 'q' || input == 'Q')
			{
				time_t nowTimer = time(nullptr);
				struct tm nowTime;

				localtime_s(&nowTime, &nowTimer);

				printf("\n[Stop]\n");
				printf("- Test Started : %04d-%02d-%02d %02d:%02d:%02d\n", startTime.tm_year + 1900, startTime.tm_mon + 1, startTime.tm_mday, startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
				printf("- Now          : %04d-%02d-%02d %02d:%02d:%02d\n", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

				if (input == 's' || input == 'S')
				{
					system("pause");
				}
				else
				{
					break;
				}
			}
		}

		PerformanceTest(performanceTestCount++, 10'000'000);
		wprintf(L"performanceTestCount = %d\n", performanceTestCount);
	}

	PROFILE_SAVE(L"Profile_Result.txt");

	_CrtDumpMemoryLeaks();
}