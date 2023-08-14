#pragma once

#include <cstddef>
#include <iterator>

#include "node.h"

template <class T>
class linked_list
{
public:
	~linked_list()
	{
		node<T>* n = get_head();
		while (n)
		{
			node<T>* next = n->get_next();
			delete n;
			n = next;
		}
	}

	node<T>* get_head() const { return head; }

	void insert_left(T value)
	{
		node<T>* new_node = new node<T>(value);
		new_node->next = get_head();
		head = new_node;
	}

	node<T>* create_left()
	{
		node<T>* new_node = new node<T>;
		new_node->next = get_head();
		head = new_node;

		return new_node;
	}

	/*
	 * Iterators
	 */
	class Iterator
	{
		node<T>* n;

	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::forward_iterator_tag;

		Iterator(node<T>* in_node)
			: n(in_node) {}

		Iterator& operator++()
		{
			n = n->next;
			return *this;
		} // prefix
		Iterator operator++(int)
		{
			Iterator i = *this;
			++*this;
			return i;
		} // postfix
		bool operator==(const Iterator& o) const { return n == o.n; }
		bool operator!=(const Iterator& o) const { return !(*this == o); }
		T	 operator*() { return n->get_value(); }
	};

	Iterator begin() { return Iterator(head); }
	Iterator end() { return Iterator(nullptr); }

private:
	node<T>* head = nullptr;
};