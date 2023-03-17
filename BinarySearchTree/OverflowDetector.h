#pragma once
#include <cassert>
#include <iostream>
#include <Windows.h>

#define PAGE_SIZE 4096

void* Malloc_OverflowCheck(size_t size)
{
	assert(size <= PAGE_SIZE * 16); // 64K 이하 사이즈만 기능함

	size_t needPage = size / PAGE_SIZE + 2;

	void* userAddress = VirtualAlloc(nullptr, PAGE_SIZE * needPage, MEM_COMMIT, PAGE_READWRITE);
	void* noaccessAddress = VirtualAlloc((char*)userAddress + PAGE_SIZE * (needPage - 1), PAGE_SIZE, MEM_COMMIT, PAGE_NOACCESS);
	void* ret = (char*)noaccessAddress - size;
	return ret;
}

void Free_OverflowCheck(void* address)
{
	unsigned long long mask = 0xFFFF'FFFF'FFFF'0000;
	void* addressToFree = (void*)((unsigned long long)address & mask);
	bool ret = VirtualFree(addressToFree, 0, MEM_RELEASE);
}