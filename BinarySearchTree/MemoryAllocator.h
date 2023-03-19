// version 1.3.0
#pragma once

#include <iostream>

#pragma region Default Allocator (�⺻ �Ҵ���)
// malloc()/free()�� ����ϴ� �⺻���� �޸� �Ҵ���
class DefaultAllocator
{
public:
	inline static void* Alloc(size_t size) { return malloc(size); }
	inline static void Free(void* address) { free(address); }
};
#pragma endregion

#pragma region Overflow Detector (�����÷ο� ���� �Ҵ���)
// VirtualAlloc()/VirtualFree()�� ����Ͽ� �Ҵ� ���� �ڿ� NOACCESS �������� ����
// �����÷ο� �߻� �� ũ���ð� �߻��Ѵ�
// 60K ���� ����� �����
class OverflowCheckAllocator
{
public:
	static void* Alloc(size_t size);
	static void Free(void* address);
};
#pragma endregion

#pragma region Underflow Detector (����÷ο� ���� �Ҵ���)
// VirtualAlloc()/VirtualFree()�� ����Ͽ� �Ҵ� ���� �տ� NOACCESS �������� ����
// ����÷ο� �߻� �� ũ���ð� �߻��Ѵ�
// 60K ���� ����� �����
class UnderflowCheckAllocator
{
public:
	static void* Alloc(size_t size);
	static void Free(void* address);
};
#pragma endregion

#pragma region Out-of-range Detector (�����÷ο�/����÷ο� ���� �Ҵ���)
// VirtualAlloc()/VirtualFree()�� ����Ͽ� �Ҵ� ���� ��/�ڿ� NOACCESS �������� ����
// �� ������ ������ ũ���� ����� ���� �� �¾� �������� ����� �Ҵ��� �� �����ϴ�.
// �׷��� ���� ����� �Ҵ��� ���, ����÷ο� üũ�� �ﰢ������ �̷������ ���Ѵ�.
// �ǵ������� ������ ���, �����÷ο�/����÷ο� �߻� �� ũ���ð� �߻��Ѵ�
// 56K ���� ����� �����
class OutOfRangeCheckAllocator
{
public:
	static void* Alloc(size_t size);
	static void Free(void* address);
};
#pragma endregion