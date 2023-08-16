#include <cstring>
#include <iostream>

#include "bvc_dec/entropy_coding/huffman/decoder.h"

/**
 * The vitter-huffman encoder is able to decode a byte at a time
 *
 *
 */
huffman_decoder::huffman_decoder()
	: output(new bitstream())
{
	nyt = new huffman_node(BVC_HUFFMAN_NODE_NYT, 0, node_list.create_left());
	nodes[BVC_HUFFMAN_NODE_NYT] = nyt;
}

huffman_decoder::~huffman_decoder()
{
	clear();
}

void huffman_decoder::decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols)
{
	for (size_t i = 0; i < in_size; i++)
	{
		stream->write_byte(in_bytes[i]);
	}

	size_t		  out_num_symbols = 0;
	huffman_node* root = nyt;

	while (out_num_symbols < in_num_symbols)
	{
		huffman_node* node = root;
		while (!node->is_leaf())
		{
			node = traverse_tree(node);
		}

		if (node->is_nyt())
		{
			uint8_t b_in = 0;
			for (size_t i = 0; i < 8; i++)
			{
				uint8_t bit = 0;
				stream->read_bit(&bit);

				b_in |= (bit << i);
			}

			output->write_byte(b_in);
			expand_nyt(b_in);
		}
		else
		{
			uint8_t b_in = (uint8_t)node->get_symbol();
			output->write_byte(b_in);
			node->increment();
		}

		out_num_symbols += 8;
	}
}

void huffman_decoder::flush(uint8_t** out_bits, size_t* out_size)
{
	*out_bits = new uint8_t[output->occupancy()];
	memcpy(*out_bits, output->data(), output->occupancy());
	*out_size = output->occupancy();

	clear();
}

void huffman_decoder::clear()
{
	// Cleanup
	for (huffman_node* node : node_list)
	{
		delete node;
	}

	delete stream;
	delete output;
	nodes.clear();

	// Initialisation
	stream = new bitstream();
	output = new bitstream();

	node_list = linked_list<huffman_node*>();
	nyt = new huffman_node(BVC_HUFFMAN_NODE_NYT, 0, node_list.create_left());
	nodes[BVC_HUFFMAN_NODE_NYT] = nyt;
}

huffman_node* huffman_decoder::traverse_tree(huffman_node* in_node)
{
	while (!(stream->occupancy() == 0) && !in_node->is_leaf())
	{
		uint8_t bit = 0;
		stream->read_bit(&bit);
		in_node = in_node->go_via(bit);
	}

	return in_node;
}

void huffman_decoder::expand_nyt(int64_t in_symbol)
{
	// create nodes IN ORDER of increasing counts
	node<huffman_node*>* value_list_node = node_list.create_left();
	node<huffman_node*>* new_nyt_list_node = node_list.create_left();

	// make new nodes
	huffman_node* value_node = new huffman_node(in_symbol, 1, value_list_node);
	nyt = nyt->expand(value_node, new_nyt_list_node);

	nodes[BVC_HUFFMAN_NODE_NYT] = nyt;
	nodes[in_symbol] = value_node;
}