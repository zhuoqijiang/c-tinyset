#pragma once
#include"_malloc_alloc_template.h"
#include"_default_alloc_template.h"
#include<algorithm>
template<typename T,class Alloc=_default_alloc_template>
struct simple_alloc
{
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	static pointer allocate(size_type n)
	{
		if (!n)
			return nullptr;
		return (pointer)Alloc::allocate(n*sizeof(value_type));
	}
	static pointer allocate()
	{
		return (pointer)Alloc::allocate(sizeof(value_type));
	}
	static pointer reallocate(void *p,size_type old_size,size_type new_size)
	{
		if (new_size == 0)
			return nullptr;
		return (pointer)Alloc::reallocate(p, old_size*sizeof(value_type), new_size*sizeof(value_type));
	}
	static void deallocate(pointer p, size_type n)
	{
		Alloc::deallocate(p,n*sizeof(value_type));
	}
	static void deallocate(pointer p)
	{
		Alloc::deallocate(p,sizeof(value_type));
	}
	static void construct(pointer p, const value_type& value)
	{
		new(p) value_type(value);
	}
	static void destroy(pointer p)
	{
		p->~value_type();
	}
	static pointer address(reference x)
	{
		return (pointer)&x;
	}
	static const_pointer const_address(reference x)
	{
		return static_cast<const_pointer>(&x);
	}
};

