#include <algorithm>

#include "bvc_common/entropy/huffman/huffman_node.h"

huffman_node::huffman_node(int64_t in_symbol, size_t in_count, node<huffman_node*>* in_list_node)
	: symbol(in_symbol)
	, count(in_count)
	, left(nullptr)
	, right(nullptr)
	, parent(nullptr)
	, list_node(in_list_node)
{
	list_node->set_value(this);
}

bool huffman_node::is_internal() const
{
	return symbol == BVC_HUFFMAN_NODE_INTERNAL;
}

bool huffman_node::is_leaf() const
{
	return !is_internal();
}

bool huffman_node::is_nyt() const
{
	return symbol == BVC_HUFFMAN_NODE_NYT;
}

huffman_node* huffman_node::expand(huffman_node* in_value_node, node<huffman_node*>* in_new_nyt_list_node)
{
	huffman_node* new_nyt = new huffman_node(BVC_HUFFMAN_NODE_NYT, 0, in_new_nyt_list_node);
	new_nyt->parent = this;
	left = new_nyt;

	in_value_node->parent = this;
	right = in_value_node;

	new_nyt->adjust_code_to_parent(0);
	in_value_node->adjust_code_to_parent(1);

	symbol = BVC_HUFFMAN_NODE_INTERNAL;
	increment();

	return new_nyt;
}

void huffman_node::increment()
{
	count++;

	huffman_node* p_parent = parent;
	while (true)
	{
		huffman_node* successor = find_successor();
		if (!successor || *this <= *successor)
		{
			break;
		}

		if (successor == this->parent)
		{
			// check if not parent
			// this occurs when node is sibling to NYT
			// break the loop, parent's value is going to be incremented
			break;
		}

		swap_with(successor);
	}

	if (is_internal())
	{
		// internal node skips all nodes that counts are equal
		// so it swapped with same-valued node
		// need to increment old path
		// p_parent because parents are swapped
		if (p_parent)
		{
			p_parent->increment();
		}
	}
	else
	{
		// leaf node doesn't skip same valued nodes
		// so it swapped with lower-valued node
		// need to increment new path
		if (parent)
		{
			parent->increment();
		}
	}
}

int64_t huffman_node::get_symbol()
{
	return symbol;
}

huffman_node* huffman_node::go_via(uint8_t in_bit)
{
	switch (in_bit)
	{
		case 0:
			return left;
		case 1:
			return right;
		default:
			return nullptr;
	}
}

bitstream* huffman_node::get_code()
{
	return &code;
}

bool huffman_node::operator>(const huffman_node& rhs) const
{
	int lcount = count * 2;
	int rcount = rhs.count * 2;

	if (is_internal())
	{
		lcount++;
	}

	if (rhs.is_internal())
	{
		rcount++;
	}

	return lcount > rcount;
}

huffman_node* huffman_node::find_successor()
{
	node<huffman_node*>* potential_next = list_node->get_next();
	return potential_next ? potential_next->get_value() : nullptr;
}

void huffman_node::swap_with(huffman_node* in_other)
{
	bool this_is_left = this->parent->left == this;
	bool node_is_left = in_other->parent->left == in_other;

	if (this_is_left)
	{
		this->parent->left = in_other;
	}
	else
	{
		this->parent->right = in_other;
	}

	if (node_is_left)
	{
		in_other->parent->left = this;
	}
	else
	{
		in_other->parent->right = this;
	}

	using std::swap;

	swap(parent, in_other->parent);
	adjust_code_to_parent(node_is_left ? 0 : 1);
	in_other->adjust_code_to_parent(this_is_left ? 0 : 1);

	swap(*list_node, *in_other->list_node);
	swap(list_node, in_other->list_node);
}

void huffman_node::adjust_code_to_parent(uint8_t bit)
{
	code = parent->code;

	code.write_bit(bit);

	if (left)
	{
		left->adjust_code_to_parent(0);
	}
	if (right)
	{
		right->adjust_code_to_parent(1);
	}
}
