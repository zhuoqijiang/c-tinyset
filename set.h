#pragma once
#include"rb_tree.h"
template<class Key,class Compare=less<Key>,class Alloc=alloc>
class set {
public:
	typedef Key value_type;
	typedef Key key_type;
	typedef Compare value_compare;
	typedef Compare key_compare;
private:
	typedef rb_tree<value_type, Compare, Alloc> rb_tree;
	rb_tree t;
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef typename rb_tree::const_pointer pointer;
	typedef typename rb_tree::const_pointer const_pointer;
	typedef typename rb_tree::const_reference reference;
	typedef typename rb_tree::const_reference reference;
	typedef typename rb_tree::const_iterator iterator;
	typedef typename rb_tree::const_iterator const_iterator;
	typedef typename rb_tree::const_reverse_iterator reverse_iterator;
	typedef typename rb_tree::const_reverse_iterator const_reverse_iterator;
public:
	set():t(Compare()){}
	explicit set(const Compare& cmp):t(cmp){}
	template<class Inputiterator>
	set(Inputiterator first,Inputiterator last):t(Compare())
	{
		t.insert_unique(first, last);
	}
	template<class Inputiterator>
	set(Inputiterator first, Inputiterator last, const Compare& cmp) :t(cmp)
	{
		t.insert_unique(first, last);
	}
	set(const set<Key,Compare,Alloc>& s):t(s.t){}
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x)
	{
		t = x.t;
		return *this;
	}
	template<class K, class C, class A>
	friend bool operator<(const set<K,C,A>& x, const set<K,C,A>& y);
	template<class K, class C, class A>
	friend bool operator>(const set<K, C, A>& x, const set<K, C, A>& y);
	template<class K, class C, class A>
	friend bool operator==(const set<K, C, A>& x, const set<K, C, A>& y);
	template<class K, class C, class A>
	friend bool operator!=(const set<K, C, A>& x, const set<K, C, A>& y);
public:
	value_compare value_com() const{ return t.value_compare(); }
	key_compare key_comp() const{ return t.value_compare(); }
	iterator begin() const{ return t.begin(); }
	iterator end()const{ return t.end(); }
	reverse_iterator rbegin()const{ return t.rbegin(); }
	reverse_iterator rend()const { return t.rend; }
	bool empty() const{ return t.empty(); }
	size_type size()const { return t.size(); }
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }
	iterator insert(const value_type& x) { return t.insert_unique(x); }
	template<class Inputiterator>
	void insert(Inputiterator first, Inputiterator last)
	{
		return t.insert_unique(first, last);
	}
	void erase(const value_type&x) { t.erase(x); }
	void clear() { t.clear(); }
	iterator find() { return t.find(); }
	size_type count() { return t.count; }
	iterator upper_bound(const value_type& x) { return t.upper_bound(x); }
	iterator lower_bound(const value_type& x) { return t.lower_bound(x); }
	std::pair<iterator,iterator> equal_range(const value_type& x) { return t.equal_range(x); }
};
template<class Key, class Compare, class Alloc>
bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y)
{
	return x.t < y.t;
}
template<class Key, class Compare, class Alloc>
bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y)
{
	return x.t == y.t;
}
template<class Key, class Compare, class Alloc>
bool operator!=(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y)
{
	return !(x.t == y.t);
}
template<class Key, class Compare, class Alloc>
bool operator>(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y)
{
	return (!(x.t == y.t)&&!(x.t<y.t));
}
