// version 1.3.0
#pragma once

#include <iostream>

#pragma region Default Allocator (기본 할당자)
// malloc()/free()를 사용하는 기본적인 메모리 할당자
class DefaultAllocator
{
public:
	inline static void* Alloc(size_t size) { return malloc(size); }
	inline static void Free(void* address) { free(address); }
};
#pragma endregion

#pragma region Overflow Detector (오버플로우 감지 할당자)
// VirtualAlloc()/VirtualFree()을 사용하여 할당 영역 뒤에 NOACCESS 페이지를 붙임
// 오버플로우 발생 시 크래시가 발생한다
// 60K 이하 사이즈만 기능함
class OverflowCheckAllocator
{
public:
	static void* Alloc(size_t size);
	static void Free(void* address);
};
#pragma endregion

#pragma region Underflow Detector (언더플로우 감지 할당자)
// VirtualAlloc()/VirtualFree()을 사용하여 할당 영역 앞에 NOACCESS 페이지를 붙임
// 언더플로우 발생 시 크래시가 발생한다
// 60K 이하 사이즈만 기능함
class UnderflowCheckAllocator
{
public:
	static void* Alloc(size_t size);
	static void Free(void* address);
};
#pragma endregion

#pragma region Out-of-range Detector (오버플로우/언더플로우 감지 할당자)
// VirtualAlloc()/VirtualFree()을 사용하여 할당 영역 앞/뒤에 NOACCESS 페이지를 붙임
// 이 버전은 페이지 크기의 배수로 거의 딱 맞아 떨어지는 사이즈를 할당할 때 유용하다.
// 그렇지 않은 사이즈를 할당할 경우, 언더플로우 체크는 즉각적으로 이루어지지 못한다.
// 의도적으로 동작할 경우, 오버플로우/언더플로우 발생 시 크래시가 발생한다
// 56K 이하 사이즈만 기능함
class OutOfRangeCheckAllocator
{
public:
	static void* Alloc(size_t size);
	static void Free(void* address);
};
#pragma endregion