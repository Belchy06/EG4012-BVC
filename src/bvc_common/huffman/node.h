#pragma once

#include <algorithm>

template <class T>
class linked_list;

template <class T>
class node
{
public:
	node()
		: next(nullptr)
		, value(0)
	{
	}

	node(T in_value)
		: next(nullptr)
		, value(in_value)
	{
	}

	T	  get_value() const { return value; }
	void  set_value(T in_value) { value = in_value; }
	node* get_next() const { return next; }

	friend class linked_list<T>;

	template <class X>
	friend void swap(node<X>& in_node1, node<X>& in_node2) { std::swap(in_node1.value, in_node2.value); }

private:
	node<T>* next;
	T		 value;
};