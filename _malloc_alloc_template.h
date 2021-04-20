#pragma once
#include<malloc.h>
#include<stdlib.h>
void default_handle()
{
	exit(1);
}
class _malloc_alloc_template {
public:
	typedef void (*pointer_handle)();
	static void* allocate(size_t n);
	static void* reallocate(void* p, size_t old_size,size_t new_size);
	static void deallocate(void* p,size_t n);
	static pointer_handle set_new_handle(pointer_handle tmp);
private:
	static void* oom_malloc(size_t n);
	static void* oom_realloc(void* p, size_t n);
	static pointer_handle malloc_handle;
};
typename  _malloc_alloc_template::pointer_handle _malloc_alloc_template::malloc_handle =default_handle;
inline void* _malloc_alloc_template::allocate(size_t n)
{
	void* tmp = malloc(n);
	if (!tmp)
		tmp = oom_malloc(n);
	return tmp;
}
inline void* _malloc_alloc_template::reallocate(void* p, size_t old_size,size_t new_size)
{
	void* result= realloc(p, old_size);
	if (!result)
		result = oom_realloc(p, old_size);
	return result;
}
inline void  _malloc_alloc_template::deallocate(void* p,size_t n)
{
	free(p);
	p = nullptr;
}
typename _malloc_alloc_template::pointer_handle
inline _malloc_alloc_template::set_new_handle(pointer_handle new_)
{
	pointer_handle old = malloc_handle;
	malloc_handle = new_;
	return old;
}
void* _malloc_alloc_template::oom_malloc(size_t n)
{
	pointer_handle tmp = nullptr;
	void* result = nullptr;
	while (1) {
		tmp = malloc_handle;
		if (!tmp)
			exit(1);
		tmp();
		result = malloc(n);
		if (result)
			return result;
	}
}
void* _malloc_alloc_template::oom_realloc(void* p, size_t n)
{
	pointer_handle tmp = nullptr;
	void* result = nullptr;
	while (1) {
		tmp = malloc_handle;
		if (!tmp)
			exit(1);
		tmp();
		result = realloc(p, n);
		if (result)
			return result;
	}
}



