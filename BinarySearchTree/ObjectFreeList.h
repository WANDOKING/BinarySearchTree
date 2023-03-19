// version 1.3.1
#pragma once

#include "MemoryAllocator.h"

// 오브젝트풀의 안전 모드 사용 여부
// 안전 모드 사용 시 오브젝트 앞 뒤로 오브젝트 풀의 this 포인터를 붙인다.
// Free()로 전달받은 포인터의 앞 뒤를 조사하여 이 값이 오염되었는지 확인한다.
//#define _USE_OBJECT_POOL_SAFE_MODE

// 사용할 Allocator
#define MEMORY_ALLOC(size) OverflowCheckAllocator::Alloc(size)
#define MEMORY_FREE(ptr) OverflowCheckAllocator::Free(ptr)

template <typename T>
class ObjectFreeList
{
private:
	template <typename T>
	struct Node
	{
#ifdef _USE_OBJECT_POOL_SAFE_MODE
		ObjectFreeList<T>* safeblock{};
#endif
		T data{};
		Node<T>* next{};
	};
public:
	inline ObjectFreeList(bool bNeedPlacementNew = false)
		: mbNeedPlacementNew(bNeedPlacementNew)
	{ }

	ObjectFreeList(int capacity, bool bNeedPlacementNew = false)
		: mbNeedPlacementNew(bNeedPlacementNew)
		, mCapacity(capacity)
		, mSize(capacity)
	{
		Node<T>* newNode = nullptr;
		Node<T>* nextNode = nullptr;

		for (int i = 0; i < capacity; ++i)
		{
			newNode = (Node<T>*)(MEMORY_ALLOC(sizeof(Node<T>)));

			if (mbNeedPlacementNew == false)
			{
				new (&(newNode->data)) T();
			}

			newNode->next = nextNode;
			nextNode = newNode;
		}
		head = nextNode;
	}

	inline int GetCapacity()
	{
		return mCapacity;
	}

	inline int GetSize()
	{
		return mSize;
	}

	inline bool IsCallPlacementNew()
	{
		return mbNeedPlacementNew;
	}

	T* Alloc()
	{
		Node<T>* ret;

		if (mSize == 0)
		{
			ret = (Node<T>*)(MEMORY_ALLOC(sizeof(Node<T>)));

			if (mbNeedPlacementNew == false)
			{
				new (&(ret->data)) T();
			}

			mCapacity++;
		}
		else
		{
			ret = head;
			head = head->next;
			mSize--;
		}

		if (mbNeedPlacementNew)
		{
			new (&(ret->data)) T();
		}

#ifdef _USE_OBJECT_POOL_SAFE_MODE
		ret->safeblock = (ObjectFreeList<T>*)this;
		ret->next = (Node<T>*)this;
#endif

		return &(ret->data);
	}

	void Free(T* data)
	{
#ifdef _USE_OBJECT_POOL_SAFE_MODE
		Node<T>* node = (Node<T>*)((unsigned char*)data - sizeof(ObjectFreeList<T>*));

		// memory pollution detect
#pragma warning(push)
#pragma warning(disable: 6011)
		if ((ObjectFreeList*)(node->safeblock) != this || (ObjectFreeList*)(node->next) != this)
		{
			// intentional crash
			int* nullPointer = 0x00000000;
			*nullPointer = 0xDEADCA11;
		}
#pragma warning(pop)
#else
		Node<T>* node = (Node<T>*)(data);
#endif

		if (mbNeedPlacementNew)
		{
			data->~T();
		}

		node->next = head;
		head = node;
		mSize++;
	}
private:
	const bool mbNeedPlacementNew; // Alloc()/Free()호출 시, 생성자/소멸자를 호출 할 것인지에 대한 옵션
	Node<T>* head = nullptr;
	int mCapacity = 0;
	int mSize = 0;
};