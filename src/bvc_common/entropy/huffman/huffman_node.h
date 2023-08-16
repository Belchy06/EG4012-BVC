#pragma once

#include <stdint.h>

#include "bvc_common/bitstream/bitstream.h"
#include "bvc_common/entropy/huffman/huffman.h"
#include "bvc_common/linked_list/linked_list.h"

class huffman_node
{
public:
	huffman_node(int64_t in_symbol, size_t in_count, node<huffman_node*>* in_list_node);

	huffman_node* expand(huffman_node* in_value_node, node<huffman_node*>* in_new_nyt_list_node);
	void		  increment();

	int64_t		  get_symbol();
	huffman_node* go_via(uint8_t in_bit);
	bitstream*	  get_code();

	bool is_internal() const;
	bool is_leaf() const;
	bool is_nyt() const;

	bool operator>(const huffman_node& rhs) const;
	bool operator<(const huffman_node& rhs) const { return rhs > *this; }
	bool operator>=(const huffman_node& rhs) const { return !(rhs > *this); }
	bool operator<=(const huffman_node& rhs) const { return !(rhs < *this); }

private:
	huffman_node* find_successor();
	void		  swap_with(huffman_node* in_other);
	void		  adjust_code_to_parent(uint8_t bit);

private:
	int64_t				 symbol;
	size_t				 count;
	node<huffman_node*>* list_node;

	huffman_node* left;
	huffman_node* right;
	huffman_node* parent;

	bitstream code;
};