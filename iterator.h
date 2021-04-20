#pragma once
#include<iostream>
#include"rb_tree_node.h"
template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct iterator_public {
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
	virtual	~iterator_public(){}
};
struct _rb_tree_base_iterator{
	typedef _rb_tree_node_base::base_ptr base_ptr;
	base_ptr node;
	void increment();
	void decrement();
	virtual ~_rb_tree_base_iterator(){}
};
void _rb_tree_base_iterator::increment()
{
	if (node->right != nullptr) {
		node = node->right;
		while (node->left != nullptr)
			node = node->left;
	}
	else {
		base_ptr tmp = node->parent;
		while (tmp->right == node) {
			node = tmp;
			tmp = tmp->parent;
		}
		if (node->right != tmp)
			node = tmp;
	}
}
void _rb_tree_base_iterator::decrement()
{
	if (node->color == _rb_tree_red && node->parent->parent == node) {
		node = node->right;
	}
	else if (node->left != nullptr) {
		node = node->left;
		while (node->left != nullptr)
			node = node->right;
	}
	else {
		base_ptr tmp = node->parent;
		while (node == tmp->left) {
			node = tmp;
			tmp = tmp->parent;
		}
		node = tmp;
	}
}
template<class T,class Ref,class Ptr>
struct _rb_tree_iterator:public iterator_public<std::forward_iterator_tag,T>,public _rb_tree_base_iterator{
	
	typedef Ref reference;
	typedef Ptr pointer;
	typedef _rb_tree_node<T>* link_type;
	typedef _rb_tree_iterator<T,T&,T*> iterator;
	typedef _rb_tree_iterator<T,const T&,const T*> const_iterator;
	typedef _rb_tree_iterator<T,Ref,Ptr> self;
	typedef const reference const_reference;
	_rb_tree_iterator() = default;
	_rb_tree_iterator(link_type x) { node = x; }
	_rb_tree_iterator(const iterator& it) { node = it.node; }
	reference operator* () const{return link_type(node)->value;}
	pointer operator->() const { return &(*(*this));}
	self& operator++()
	{ 
		increment();
		return *this;
	}
	self operator++(int)
	{
		self temp = *this;
		increment();
		return temp;
	}
	self& operator--()
	{
		decrement();
		return *this;
	}
	self operator--(int)
	{
		self temp = *this;
		decrement();
		return temp;
	}
	const reference operator=(const iterator& it) { node = it.node; return node; }
	bool operator==(const self& a) { return  node == a.node;}
	bool operator!=(const self& a) { return node != a.node; }
};
template<class T, class Ref, class Ptr>
struct _rb_tree_reverse_iterator :public iterator_public<std::forward_iterator_tag, T>, public _rb_tree_base_iterator {

	typedef Ref reference;
	typedef Ptr pointer;
	typedef _rb_tree_node<T>* link_type;
	typedef _rb_tree_reverse_iterator<T,const T&,const T*> const_reverse_iterator;
	typedef _rb_tree_reverse_iterator<T,T&,T*> reverse_iterator;
	typedef _rb_tree_reverse_iterator<T,Ref, Ptr> self;
	reference operator* () const{ 
		_rb_tree_iterator<T> temp = node;
		return *(--temp);
	}
	pointer operator->() const { return &(*(*this)); }
	self& operator++() 
	{
		decrement();
		return *this;
	}
	self operator++(int)
	{
		self temp = *this;
		decrement();
		return temp;
	}
	self& operator--()
	{
		increment();
		return *this;
	}
	self operator--(int)
	{
		self temp = *this;
		increment();
		return temp;
	}
	const reference operator=(const reverse_iterator& it) { node = it.node; return node; }
	bool operator==(const self& a) { return link_type(node)->value == link_type(a.node)->value; }
	bool operator!=(const self& a) { return link_type(node)->value != link_type(a.node)->value; }
	_rb_tree_reverse_iterator() = default;
	explicit _rb_tree_reverse_iterator(link_type x) { node = x; }
	explicit _rb_tree_reverse_iterator(const reverse_iterator& it) { node = it.node; }
};