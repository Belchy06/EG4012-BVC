#include <cstring>

#include "ovc_enc/entropy/huffman/encoder.h"

/**
 * The vitter-huffman encoder is able to encode a byte at a time
 *
 *
 */
huffman_encoder::huffman_encoder()
{
	clear();
}

huffman_encoder::~huffman_encoder()
{
	for (huffman_node* node : node_list)
	{
		delete node;
	}
}

void huffman_encoder::encode(const uint8_t* in_bytes, size_t in_size)
{
	for (size_t i = 0; i < in_size; i++)
	{
		encode_internal(in_bytes[i]);
	}
}

void huffman_encoder::flush(uint8_t** out_bytes, size_t* out_size)
{
	*out_bytes = new uint8_t[bitstream->occupancy()];
	memcpy(*out_bytes, bitstream->data(), bitstream->occupancy());
	*out_size = bitstream->occupancy();

	clear();
}

void huffman_encoder::clear()
{
	delete bitstream;
	bitstream = new ovc::bitstream();

	nyt = new huffman_node(ovc::HUFFMAN_NODE_NYT, 0, node_list.create_left());
	nodes[ovc::HUFFMAN_NODE_NYT] = nyt;
}

void huffman_encoder::encode_internal(uint8_t in_symbol)
{
	if (nodes.count(in_symbol))
	{
		huffman_node* node = nodes[in_symbol];

		uint8_t			bit = 0;
		ovc::bitstream* code = node->get_code();
		code->read_idx = 0;
		while (code->read_bit(&bit))
		{
			bitstream->write_bit(bit);
		}

		node->increment();
	}
	else
	{
		// not yet transferred
		uint8_t			bit = 0;
		ovc::bitstream* code = nyt->get_code();
		code->read_idx = 0;
		while (code->read_bit(&bit))
		{
			bitstream->write_bit(bit);
		}
		bitstream->write_byte(in_symbol);

		expand_nyt(in_symbol);
	}
}

void huffman_encoder::expand_nyt(int64_t in_symbol)
{
	// create nodes IN ORDER of increasing counts
	node<huffman_node*>* value_list_node = node_list.create_left();
	node<huffman_node*>* new_nyt_list_node = node_list.create_left();

	// make new nodes
	huffman_node* value_node = new huffman_node(in_symbol, 1, value_list_node);
	nyt = nyt->expand(value_node, new_nyt_list_node);

	nodes[ovc::HUFFMAN_NODE_NYT] = nyt;
	nodes[in_symbol] = value_node;
}