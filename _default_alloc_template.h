#pragma once
#include<stdlib.h>
#include"_malloc_alloc_template.h"
enum{_ALIGN=8};
enum{_MAX_BYTES=128};
enum{_NFREELIST=_MAX_BYTES/_ALIGN};
class _default_alloc_template
{
public:
	static void* allocate(size_t n);
	static void* reallocate(void* p,size_t old_size,size_t new_size);
	static void deallocate(void* p,size_t n);
	union obj {
		obj* free_list_link;
		char client_data[1];
	};
private:
	static size_t round_up(size_t n)
	{
		return (n + _ALIGN - 1) & ~(_ALIGN - 1);
	}
	static size_t free_list_index(size_t n)
	{
		return (n + (_ALIGN - 1)) / (_ALIGN - 1);
	}
	static void* refill(size_t n);
	static void* chunk_alloc(int &nobjs,size_t n);
private:
	static obj* free_list[_NFREELIST];
	static char* start_free;
	static char* end_free;
	static size_t heap_size;
};
char* _default_alloc_template::start_free = nullptr;
char* _default_alloc_template::end_free = nullptr;
size_t _default_alloc_template::heap_size = 0;
typename _default_alloc_template::obj*
_default_alloc_template::free_list[_NFREELIST] = {nullptr,nullptr ,nullptr ,nullptr,
nullptr,nullptr ,nullptr ,nullptr,nullptr,nullptr ,nullptr ,nullptr,nullptr,nullptr ,nullptr ,nullptr};
void* _default_alloc_template::allocate(size_t n)
{
	obj** my_free_list=nullptr;
	obj* result = nullptr;
	if (n > 128)
		return _malloc_alloc_template::allocate(n);
	my_free_list = free_list + free_list_index(n);
	result = *my_free_list;
	if (result == nullptr) {
		void* tmp = refill(round_up(n));
		return tmp;
	}
	*my_free_list = result->free_list_link;
	return result;
}
void* _default_alloc_template::reallocate(void* p,size_t old_size,size_t new_size)
{
	if (p)
		deallocate(p, old_size);
	void* result = allocate(new_size);
	return result;
}
void _default_alloc_template::deallocate(void* p,size_t n)
{
	obj** my_free_list = nullptr;
	if (n > static_cast<size_t>(_MAX_BYTES)) {
		_malloc_alloc_template::deallocate(p,n);
		return;
	}
	obj* result = static_cast<obj*>(p);
	my_free_list = free_list + free_list_index(n);
	result->free_list_link = *my_free_list;
	*my_free_list = result;
}
void* _default_alloc_template::refill(size_t n)
{
	int nobjs = 20;
	char* chunk =(char*)(chunk_alloc(nobjs,n ));
	obj** my_free_list = nullptr;
	obj* result, * current_obj, * next_obj;
	if (nobjs == 1)
		return chunk;
	my_free_list = free_list + free_list_index(n);
	result = (obj*)chunk;
	next_obj = (obj*)(chunk+n);
	*my_free_list = next_obj;
	for (int i = 1;; i++) {
		current_obj = next_obj;
		next_obj = (obj*)((char*)current_obj + n);
		//std::cout << i<<std::endl;
		if (nobjs-1==i) {
			current_obj->free_list_link = nullptr;
			break;
		}
		else
			current_obj->free_list_link = next_obj;
	}
	return result;
}
void* _default_alloc_template::chunk_alloc(int& nobjs, size_t n)
{
	char* result;
	size_t need_bytes = n * nobjs;
	size_t left_bytes = end_free - start_free;
	if (need_bytes <= left_bytes) {
		result =start_free;
		start_free += need_bytes;
		return result;
	}
	else if (left_bytes >= n) {
		nobjs = left_bytes / n;
		need_bytes = nobjs * n;
		result = start_free;
		start_free+= need_bytes;
		return result;
	}
	else {
		if (left_bytes > 0) {
			obj** my_free_list = free_list + free_list_index(left_bytes);
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		size_t get_bytes = 2 * need_bytes + round_up(heap_size >> 4);
		start_free=(char*)malloc(get_bytes);
		if (!start_free) {
			obj** my_free_list=nullptr;
			obj* tmp=nullptr;
			for (size_t i = n; i <= _MAX_BYTES; i += _ALIGN) {
				my_free_list = free_list + free_list_index(i);
				tmp = *my_free_list;
				if (tmp) {
					start_free =(char*)tmp;
					end_free = start_free + i;
					return chunk_alloc(nobjs, n);
				}
			}
			end_free = nullptr;
			start_free = static_cast<char*>(_malloc_alloc_template::allocate(get_bytes));
		}
		end_free = start_free+get_bytes;
		heap_size += get_bytes;
		return chunk_alloc(nobjs, n);
	}
}