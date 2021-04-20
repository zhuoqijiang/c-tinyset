#pragma once
#include"iterator.h"
#include"iterator_traits.h"
#include"rb_tree_node.h"
#include"simple_alloc.h"
#include"_default_alloc_template.h"
#include"_malloc_alloc_template.h"
#include<queue>
#include<utility>
void _rb_tree_rotate_left(_rb_tree_node_base* x, _rb_tree_node_base* (&root))
{
	_rb_tree_node_base* y = x->right;
	x->right = y->left;
	if (y->left != nullptr)
		y->left->parent = x;
	y->parent = x->parent;
	if (x == root)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}
void _rb_tree_rotate_right(_rb_tree_node_base* x, _rb_tree_node_base* (&root))
{
	_rb_tree_node_base* y = x->left;
	x->left = y->right;
	if (y->right != nullptr)
		y->right->parent = x;
	y->parent = x->parent;
	if (x == root)
		root = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}
void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base* (&root))
{
	x->color = _rb_tree_red;
	while (x != root && x->parent->color == _rb_tree_red) {
		if (x->parent == x->parent->parent->left){
			_rb_tree_node_base* y = x->parent->parent->right;
			if (y != nullptr && y->color == _rb_tree_red) {
				x->parent->color = _rb_tree_black;
				y->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->right) {
					x = x->parent;
					_rb_tree_rotate_left(x, root);
				}
				x->parent->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				_rb_tree_rotate_right(x->parent->parent, root);
			}
		}
		else {
			_rb_tree_node_base* y = x->parent->parent->left;
			if (y != nullptr && y->color == _rb_tree_red) {
				x->parent->color = _rb_tree_black;
				y->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					_rb_tree_rotate_right(x, root);
				}
				x->parent->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				_rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = _rb_tree_black;
}
void _rb_tree_rebalance_remove(_rb_tree_node_base* x,_rb_tree_node_base* (&root))
{
	while (x != root && x->color ==_rb_tree_black)
	{
		if (x == x->parent->left)
		{
			_rb_tree_node_base* w = x->parent->right;

			if (w->color ==_rb_tree_red)
			{
				w->color = _rb_tree_black;
				x->parent->color = _rb_tree_red;
				_rb_tree_rotate_left(x->parent,root);
				w = x->parent->right;
			}
			if (w->left->color == _rb_tree_black && w->right->color == _rb_tree_black)
			{
				w->color = _rb_tree_red;
				x = x->parent;
			}
			else
			{
				if (w->right->color == _rb_tree_black)
				{
					w->color =_rb_tree_red;
					w->left->color = _rb_tree_black;
					_rb_tree_rotate_right(w,root);
					w = x->parent->right;
				}
				w->color = w->parent->color;
				w->parent->color = _rb_tree_black;
				w->right->color = _rb_tree_black;
				_rb_tree_rotate_left(x->parent,root);
				x = root;

			}
		}
		else
		{
			_rb_tree_node_base* w = x->parent->left;
			if (w->color == _rb_tree_red)
			{
				w->parent->color = _rb_tree_red;
				w->color = _rb_tree_black;
				_rb_tree_rotate_right(x->parent,root);
				w = x->parent->left;
			}
			if (w->right->color == _rb_tree_black)
			{
				w->color = _rb_tree_red;
				x = x->parent;
			}
			else
			{
				if (w->left->color == _rb_tree_black)
				{
					w->right->color = _rb_tree_black;
					w->color = _rb_tree_red;
					_rb_tree_rotate_left(w,root);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = _rb_tree_black;
				w->left->color = _rb_tree_black;
				_rb_tree_rotate_right(x->parent,root);
				x = root;
			}
		}
	}
	root->color = _rb_tree_black;
}
typedef _default_alloc_template alloc;
template<class T>
struct less {
	bool operator()(const T& a, const T& b)
	{
		return a < b;
	}
};
template<class T>
struct more {
	bool operator()(const T& a, const T& b)
	{
		return a > b;
	}
};
template<class Value, class Compare=less<Value>, class Alloc = alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef _rb_tree_node_base* base_ptr;
	typedef _rb_tree_node<Value> rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc> rb_tree_allocator;
	typedef simple_alloc<Value, Alloc> value_allocator;
	typedef _rb_tree_color_type color_type;
public:
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _rb_tree_node<value_type>* link_type;
	typedef typename _rb_tree_iterator<value_type,value_type&,value_type*>::iterator iterator;
	typedef typename _rb_tree_iterator<value_type,const value_type&,const value_type*>::const_iterator const_iterator;
	typedef typename _rb_tree_reverse_iterator<value_type, value_type&, value_type*>::reverse_iterator  reverse_iterator;
	typedef typename _rb_tree_reverse_iterator<value_type, const value_type&, const value_type*>::const_reverse_iterator const_reverse_iterator;
protected:
	Compare value_compare() { return compare; }
	link_type get_node() { return rb_tree_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_allocator::deallocate(p); }
	link_type create_node(const value_type& x)
	{
		link_type tmp = get_node();
		value_allocator::construct(&tmp->value, x);
		return tmp;
	}
	link_type clone_node(link_type x)
	{
		link_type tmp = create_node(x->value);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}
	void destroy_node(link_type x)
	{
		rb_tree_allocator::destroy(&(x->value));
		put_node(x);
	}
	link_type header;
public:
	
protected:
	size_type node_count;
	Compare compare;
	link_type& root() { return (link_type&)header->parent; }
	link_type& leftmost()const { return (link_type&)header->left; }
	link_type& rightmost()const{ return (link_type&)header->right; }
	static link_type& left(link_type x) { return (link_type&)x->left; }
	static link_type& right(link_type x) { return (link_type&)x->right; }
	static link_type& parent(link_type x) { return (link_type&)x->parent; }
	static reference value(link_type x) { return x->value; }
	static color_type color(link_type x) { return (color_type&)x->color; }
	static link_type minimum(link_type x) { return (link_type)_rb_tree_node_base::minimum(x); }
	static link_type maximun(link_type x) { return (link_type)_rb_tree_node_base::maximum(x); }
public:
	rb_tree<Value, Compare, Alloc>& operator=(const rb_tree<Value, Compare, Alloc>& x);
private:
	static bool estimateless(const rb_tree&, const rb_tree&);
	static bool estimate_equal(const rb_tree&, const rb_tree&);
	void transplant(base_ptr a, base_ptr b)
	{
		if (a->parent == header)
			header->parent = b;
		else if (a == a->parent->left)
			a->parent->left = b;
		else
			a->parent->right = b;
		a->parent = b->parent;
	}
	iterator _insert(link_type x_, link_type y_, const value_type& r);
	link_type _copy(link_type x,link_type y);
	void traverse(base_ptr root,std::queue<value_type> &a)
	{
		if (root != NULL) {
			traverse(root->left,a);
			a.push(((link_type)root)->value);
			traverse(root->right,a);
		}
	}
	void traverse(base_ptr root, std::queue<link_type>& a)
	{
		if (root != NULL) {
			traverse(root->left, a);
			a.push(((link_type)root));
			traverse(root->right, a);
		}
	}
	void _erase(base_ptr x);
	void init()
	{
		header = get_node();
		header->color = _rb_tree_red;
		root() = nullptr;
		leftmost() = header;
		rightmost() = header;
		header->value = 0;
	}
public:
	iterator insert_unique(const value_type& v);
	template<class Inputiterator>
	void insert_unique(Inputiterator first, Inputiterator last);
	rb_tree(const Compare& comp = Compare()) :node_count(0), compare(comp)
	{
		init();
	}
	explicit rb_tree(const rb_tree<Value, Compare, Alloc>& x)
	{
		init();
		*this = x;
	}
	~rb_tree()
	{
		clear();
		put_node(header);
	}
public:
	template<class K, class C, class A>
	friend bool operator<(const rb_tree<K, C, A>& x, const rb_tree<K, C, A>& y)
	{
		return rb_tree<K, C, A>::estimateless(x, y);
	}
	template<class K, class C, class A>
	friend bool operator==(const rb_tree<K, C, A>& x, const rb_tree<K, C, A>& y)
	{
		return rb_tree<K, C, A>::estimate_equal(x, y);
	}
	Compare get_cmp()const { return compare; }
	iterator begin()const { return leftmost(); }
	iterator end()const { return header; }
	reverse_iterator rbegin()const { return header; }
	reverse_iterator rend()const { return rightmost(); }
	bool empty() { return node_count == 0; }
	size_type size()const { return node_count; }
	iterator find(const value_type& x);
	void swap(rb_tree<Value, Compare, Alloc>& x)
	{
		link_type tmp(header);
		header = x.header;
		x.header = tmp;
	}
	size_type count(const value_type& x) {
		if (find(x) != nullptr)
			return 1;
		return 0;
	}
	void clear();
	iterator upper_bound(const value_type& x)
	{
		iterator it = find(x);
		it++;
		return it;
	}
	iterator lower_bound(const value_type& x)
	{
		iterator it = find(x);
		return it;
	}
	std::pair<iterator,iterator> equal_range(const value_type& x)
	{
		iterator first = lower_bound(x);
		iterator second = upper_bound(x);
		std::pair<iterator, iterator> tmp;
		tmp.first = first;
		tmp.second = second;
		return tmp;
	}
	void erase(const value_type& x) {
		base_ptr p = (base_ptr)root();
		while (p != nullptr) {
			if (((link_type)p)->value == x)
				break;
			else if (value_compare()(x, ((link_type)p)->value))
				p = p->left;
			else if (value_compare()(((link_type)p)->value, x))
				p = p->right;
		}
		if (p == header->left) {
			if (p->right != nullptr) {
				header->left = p->right;
			}
			else
				header->left = p->parent;
		}
		if (p == header->right) {
			if (p->left != nullptr) {
				header->right = p->left;
			}
			else
				header->right = p->parent;
		}
		if (p != nullptr) {
			_erase(p);
		}

	}
	template<class Inputiterator>
	void erase(Inputiterator first, Inputiterator last);
};
template<class Value, class Compare, class Alloc>
bool rb_tree<Value, Compare, Alloc>::estimate_equal(const rb_tree<Value, Compare, Alloc>& x, const rb_tree<Value, Compare, Alloc>& y)
{
	iterator it1 = x.begin();
	iterator it2 = y.begin();
	while (it1 != x.end() && it2 != y.end()) {
		if (*it1 == *it2) {
			it1++;
			it2++;
			continue;
		}
		return false;
	}
	if (it1 != x.end() || it2 != y.end())
		return false;
	return true;
}
template<class Value, class Compare, class Alloc>
bool rb_tree<Value, Compare, Alloc>::estimateless(const rb_tree<Value, Compare, Alloc>& x, const rb_tree<Value, Compare, Alloc>& y)
{
	iterator it1 = x.begin();
	iterator it2 = y.begin();
	while (it1 != x.end() && it2 != y.end()) {
		if (*it1 == *it2) {
			it1++;
			it2++;
			continue;
		}
		return *it1 < *it2;
	}
	if (it2 == y.end())
		return false;
	return true;
}
template<class Value,class Compare,class Alloc>
void rb_tree<Value, Compare, Alloc>::clear()
{
	std::queue<link_type> a;
	traverse(header->parent, a);
	while (!a.empty()) {
		link_type front = a.front();
		value_allocator::destroy(&front->value);
		rb_tree_allocator::deallocate(front);
		a.pop();
	}
	header->parent = nullptr;
	header->left =header;
	header->right =header;
}
template<class Value, class Compare, class Alloc>
inline typename rb_tree<Value, Compare, Alloc>::link_type rb_tree<Value, Compare, Alloc>::_copy(link_type x, link_type y)
{
	x->value = y->value;
}
template<class Value, class Compare, class Alloc>
rb_tree<Value, Compare, Alloc>& rb_tree<Value, Compare, Alloc>::operator=(const rb_tree<Value, Compare, Alloc>& x)
{
	std::queue<int> a;
	traverse(x.header->parent,a);
	while (!a.empty()) {
		insert_unique(a.front());
		a.pop();
	}
	return *this;
}
template<class Value, class Compare, class Alloc>
typename rb_tree<Value, Compare, Alloc>::iterator
rb_tree<Value, Compare, Alloc>::find(const value_type& x)
{
	base_ptr p= (base_ptr)root();
	while (p != nullptr) {
		if (((link_type)p)->value == x)
			return iterator((link_type)p);
		else if (value_compare()(x, ((link_type)p)->value))
			p = p->left;
		else if (value_compare()(((link_type)p)->value, x))
			p = p->right;
	}
	return nullptr;
}
template<class Value,class Compare,class Alloc>
typename rb_tree<Value,Compare,Alloc>::iterator
rb_tree<Value, Compare, Alloc>::_insert(link_type x_, link_type y_, const value_type& r)
{
	link_type x = x_;
	link_type y =y_;
	link_type z;
	z = create_node(r);
		if (y != header) {
			if (value_compare()(z->value, y->value))
				left(y) = z;
			else
				right(y) = z;
		}
		else if (y == header) {
			parent(y) = z;
			root() = z;
			rightmost() = z;
			leftmost() = z;
		}
		if (value_compare()(z->value, leftmost()->value))
			leftmost() = z;
		if (value_compare()(rightmost()->value, z->value))
			rightmost() = z;
	parent(z) = y;
	left(z) =nullptr;
	right(z) = nullptr;
	_rb_tree_rebalance(z, header->parent);
	node_count++;
	return iterator(z);
}
template<class Value, class Compare, class Alloc>
void rb_tree<Value, Compare, Alloc>::_erase(base_ptr t)
{
	//rb_tree<Value, Compare, Alloc> tmp;
	base_ptr x = nullptr;
	base_ptr y = t;
	color_type color_y = y->color;
	if (t->left == nullptr && t->right == nullptr) {
		if (t->parent == header)
			header->parent = nullptr;
		else if (t == t->parent->left)
			t->parent->left = nullptr;
		else
			t->parent->right = nullptr;
	}
	else if (t->left == nullptr) {
		x = t->right;
		transplant(t, t->right);
	}
	else if (t->right == nullptr) {
		x = t->left;
		transplant(t, t->left);
	}
	else {
		y = _rb_tree_node_base::minimum(t->right);
		color_y = y->color;
		x = y->right;
		if (t == y->parent)
			x->parent = y;
		else {
			transplant(y, y->right);
			y->right = t->right;
			y->right->parent = y;
		}
		transplant(t, y);
		y->left = t->left;
		t->left->parent = y;
		y->color = t->color;
	}
	if (color_y == _rb_tree_black)
		_rb_tree_rebalance_remove(x, header->parent);
}
template<class Value,class Compare,class Alloc>
typename rb_tree<Value, Compare, Alloc>::iterator
 rb_tree<Value, Compare, Alloc>::insert_unique(const value_type& v)
{
	if (find(v) != nullptr)
		return find(v);
	link_type x= root();
	link_type y = header;
	while (x != nullptr) {
		y = x;
		if (value_compare()(value(x), v))
			x = right(x);
		else
			x = left(x);
	}
	return _insert(x, y, v);
}
template<class Value, class Compare, class Alloc>
template<class Inputiterator>
void rb_tree<Value, Compare, Alloc>::insert_unique(Inputiterator first, Inputiterator last)
{
	Inputiterator it = first;
	while (it!= last) {
		insert_unique(*it);
		it++;
	}
}
template<class Value, class Compare, class Alloc>
template<class Inputiterator>
void rb_tree<Value, Compare, Alloc>::erase(Inputiterator first, Inputiterator last)
{
	Inputiterator it = first;
	while (it != last) {
		erase(*it);
		it++;
	}
}