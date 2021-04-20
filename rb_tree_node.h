#pragma once
typedef bool _rb_tree_color_type;
const _rb_tree_color_type _rb_tree_red = false;
const _rb_tree_color_type _rb_tree_black = true;
struct _rb_tree_node_base {
	typedef _rb_tree_color_type color_type;
	typedef _rb_tree_node_base* base_ptr;
	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;
	static base_ptr minimum(base_ptr x);
	static base_ptr maximum(base_ptr x);
	virtual ~_rb_tree_node_base() {};
};
typename _rb_tree_node_base::base_ptr
_rb_tree_node_base::minimum(base_ptr x)
{
	while (x->left != nullptr)
		x = x->left;
	return x;
}
typename _rb_tree_node_base::base_ptr
_rb_tree_node_base::maximum(base_ptr x)
{
	while (x->right != nullptr)
		x = x->right;
	return x;
}
template<class T>
struct _rb_tree_node :public _rb_tree_node_base {
	typedef _rb_tree_node<T>* link_type;
	T value;
};