#pragma once
template<class I>
class iterator_traits
{
	typedef typename I::value_type value_type;
	typedef typename I::pointer pointer;
	typedef typename I::reference reference;
	typedef typename I::difference_type difference_type;
	typedef typename I::iterator_category category;
};

