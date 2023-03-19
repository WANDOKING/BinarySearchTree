// version 1.3.0
#include "MemoryAllocator.h"

#include <cassert>
#include <Windows.h>

enum
{
	PAGE_SIZE = 4096,
};

#pragma region 페이지 크기가 4KB인지 확인
#pragma warning(push)
#pragma warning(disable: 6011)
bool check_page_size()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	if (systemInfo.dwPageSize != PAGE_SIZE)
	{
		int* nullPointer = (int*)0x00000000;
		*nullPointer = 0xDEADDEAD;
	}

	return true;
}
bool g_checkForPageSize = check_page_size();
#pragma warning(pop)
#pragma endregion

#pragma region OverflowCheckAllocator
void* OverflowCheckAllocator::Alloc(size_t size)
{
	assert(size > 0);
	assert(size <= PAGE_SIZE * 16);

	const size_t NEED_PAGE = (size - 1) / PAGE_SIZE + 2;

	void* userAddress = VirtualAlloc(nullptr, PAGE_SIZE * NEED_PAGE, MEM_COMMIT, PAGE_READWRITE);
	void* noaccessAddress = VirtualAlloc((char*)userAddress + PAGE_SIZE * (NEED_PAGE - 1), PAGE_SIZE, MEM_COMMIT, PAGE_NOACCESS);
	return (void*)((char*)noaccessAddress - size);
}

void OverflowCheckAllocator::Free(void* address)
{
	void* addressToFree = (void*)((unsigned long long)address & 0xFFFF'FFFF'FFFF'0000);
	BOOL retVirtualFree = VirtualFree(addressToFree, 0, MEM_RELEASE);
	assert(retVirtualFree != 0);
}
#pragma endregion

#pragma region UnderflowCheckAllocator
void* UnderflowCheckAllocator::Alloc(size_t size)
{
	assert(size > 0);
	assert(size <= PAGE_SIZE * 16); // 64K 이하 사이즈만 기능함

	const size_t NEED_PAGE = (size - 1) / PAGE_SIZE + 2;

	void* baseAddress = VirtualAlloc(nullptr, PAGE_SIZE * NEED_PAGE, MEM_COMMIT, PAGE_READWRITE);
	void* ret = (void*)((char*)baseAddress + PAGE_SIZE);

	VirtualAlloc(baseAddress, PAGE_SIZE, MEM_COMMIT, PAGE_NOACCESS);
	return ret;
}

void UnderflowCheckAllocator::Free(void* address)
{
	void* addressToFree = (void*)((unsigned long long)address & 0xFFFF'FFFF'FFFF'0000);
	BOOL retVirtualFree = VirtualFree(addressToFree, 0, MEM_RELEASE);
	assert(retVirtualFree != 0);
}
#pragma endregion

#pragma region OutOfRangeCheckAllocator
void* OutOfRangeCheckAllocator::Alloc(size_t size)
{
	assert(size > 0);
	assert(size <= PAGE_SIZE * 16); // 64K 이하 사이즈만 기능함

	const size_t NEED_PAGE = (size - 1) / PAGE_SIZE + 3;

	void* baseAddress = VirtualAlloc(nullptr, PAGE_SIZE * NEED_PAGE, MEM_COMMIT, PAGE_READWRITE);

	void* underflowCheckPage = VirtualAlloc(baseAddress, PAGE_SIZE, MEM_COMMIT, PAGE_NOACCESS);
	void* overflowCheckPage = VirtualAlloc((char*)baseAddress + PAGE_SIZE * (NEED_PAGE - 1), PAGE_SIZE, MEM_COMMIT, PAGE_NOACCESS);

	void* ret = (void*)((char*)overflowCheckPage - size);

	return ret;
}

void OutOfRangeCheckAllocator::Free(void* address)
{
	void* addressToFree = (void*)((unsigned long long)address & 0xFFFF'FFFF'FFFF'0000);
	BOOL retVirtualFree = VirtualFree(addressToFree, 0, MEM_RELEASE);
	assert(retVirtualFree != 0);
}
#pragma endregion