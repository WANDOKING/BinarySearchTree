#pragma once

#include <set>
#include <algorithm>

template <typename T>
class BstTester
{
public:
	virtual ~BstTester() = default;

	virtual int GetSize() const = 0;
	virtual bool IsContain(T data) const = 0;
	virtual bool Insert(T data) = 0;
	virtual bool Delete(T data) = 0;
	virtual std::string GetInorderString() const = 0;
};

template <typename T>
class SetTester : public BstTester<T>
{
public:

	int GetSize() const override { return (int)mSet.size(); }

	bool IsContain(T data) const override { return mSet.find(data) != mSet.end(); }

	bool Insert(T data) override
	{
		if (IsContain(data))
		{
			return false;
		}

		mSet.insert(data);
		return true;
	}

	bool Delete(T data) override
	{
		auto it = mSet.find(data);
		
		if (it != mSet.end())
		{
			mSet.erase(it);
			return true;
		}

		return false;
	}

	std::string GetInorderString() const override
	{
		std::string result = "";

		for (const T& element : mSet)
		{
			result += "[";
			result += std::to_string(element);
			result += "] ";
		}

		return result;
	}
private:
	std::set<T> mSet;
};