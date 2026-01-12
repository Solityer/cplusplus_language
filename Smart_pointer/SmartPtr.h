#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>

using namespace std;

// 通用基础 SmartPtr (用于演示最基本的RAII)
template<class T>
class SmartPtr
{
public:
	// RAII构造函数：获取资源所有权
	SmartPtr(T* ptr)
		:_ptr(ptr)
	{}

	~SmartPtr()
	{
		cout << "~SmartPtr()->" << _ptr << endl;
		if (_ptr)
			delete _ptr;
	}

	// 像指针一样
	T& operator*()
	{
		return *_ptr;
	}

	T* operator->()
	{
		return _ptr;
	}

private:
	T* _ptr;
};

namespace pzh
{
	// C++98 auto_ptr(已弃用)
	// 管理权转移，最后一个拷贝对象管理资源，被拷贝对象都被置空
	template<class T>
	class auto_ptr
	{
	public:
		// RAII构造函数
		auto_ptr(T* ptr)
			:_ptr(ptr)
		{}

		~auto_ptr()
		{
			if (_ptr)
			{
				cout << "delete->" << _ptr << endl;
				delete _ptr; // 释放内存
				_ptr = nullptr; // 置空指针
			}
		}

		// 拷贝构造函数（管理权转移）
		// ap2(ap1) 后，ap1失去管理权，ap2获得管理权
		auto_ptr(auto_ptr<T>& ap)  // 非const引用，因为要修改源对象
			:_ptr(ap._ptr)         // 转移指针所有权
		{
			ap._ptr = nullptr;     // 源对象指针置空
		}

		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

	private:
		T* _ptr;  // 管理的原始指针
	};


	// C++11 unique_ptr (独占所有权智能指针)
	template<class T>
	class unique_ptr
	{
	public:
		// RAII
		unique_ptr(T* ptr)
			:_ptr(ptr)
		{}

		~unique_ptr()
		{
			if (_ptr)
			{
				cout << "delete->" << _ptr << endl;
				delete _ptr;
			}
		}

		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

		// C++11方式：删除拷贝构造函数（防拷贝）
		unique_ptr(const unique_ptr<T>& up) = delete;
		// 删除赋值操作符（防赋值）
		unique_ptr<T>& operator=(const unique_ptr<T>& up) = delete;

	private:
		// C++98 写法：
		// 1、只声明不实现
		// 2、限定为私有
		//unique_ptr(const unique_ptr<T>& up);
		//unique_ptr<T>& operator=(const unique_ptr<T>& up);
	private:
		T* _ptr;
	};

	// C++11 shared_ptr (共享所有权智能指针，包含定制删除器版本)
	template<class T>
	class shared_ptr
	{
	public:
		shared_ptr(T* ptr = nullptr)
			:_ptr(ptr)
			,_pcount(new int(1))  // 分配引用计数，初始化为1
		{}

		// 支持定制删除器的构造函数
		template<class D> // D为删除器类型
		shared_ptr(T *ptr, D del) // del是删除器对象
			:_ptr(ptr)
			,_pcount(new int(1))
			,_del(del) // 初始化删除器
		{}

		// 释放资源逻辑（减少引用计数，必要时释放资源）
		void release()
		{
			if (--(*_pcount) == 0)       // 减少引用计数并检查是否为0
			{
				// 调用定制删除器进行释放
				_del(_ptr);              // 使用删除器释放资源
				delete _pcount;          // 释放引用计数内存
			}
		}

		~shared_ptr()
		{
			release();
		}

		// 拷贝构造
		shared_ptr(const shared_ptr<T>& sp)
			:_ptr(sp._ptr)
			,_pcount(sp._pcount)
			,_del(sp._del) // 拷贝删除器
		{
			++(*_pcount);
		}

		// 赋值重载 sp1 = sp3
		shared_ptr<T>& operator=(const shared_ptr<T>& sp)
		{
			if (_ptr != sp._ptr)  // 避免自我赋值
			{
				release();// 释放当前资源
				// 接管新资源
				_ptr = sp._ptr;
				_pcount = sp._pcount;
				_del = sp._del;
				++(*_pcount);
			}
			return *this;
		}

		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

		// 获取引用计数
		int use_count() const
		{
			return *_pcount;
		}

		// 获取原始指针
		T* get() const
		{
			return _ptr;
		}

	private:
		T* _ptr;
		int* _pcount;

		// 默认删除器，使用lambda表达式（function包装器可存储任意可调用对象）
		function<void(T*)> _del = [](T* ptr)
		{
			// cout << "default delete->" << ptr << endl;
			delete ptr;  // 默认使用delete释放
		};
	};

	// weak_ptr (弱引用指针，解决循环引用问题)
	// 不增加引用计数，不拥有资源所有权
	template<class T>
	class weak_ptr
	{
	public:
		weak_ptr()
			:_ptr(nullptr)
		{}

		// 从shared_ptr构造
		weak_ptr(const shared_ptr<T>& sp)
			:_ptr(sp.get())  // 获取原始指针但不增加引用计数
		{}

		weak_ptr<T>& operator=(const shared_ptr<T>& sp)
		{
			_ptr = sp.get(); // 获取原始指针但不增加引用计数
			return *this;
		}

		T& operator*()
		{
			return *_ptr;
		}

		T* operator->()
		{
			return _ptr;
		}

	private:
		T* _ptr;
	};
}