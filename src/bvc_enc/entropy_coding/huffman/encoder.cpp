#include <cstring>

#include "bvc_enc/entropy_coding/huffman/encoder.h"

/**
 * The vitter-huffman encoder is able to encode a byte at a time
 *
 *
 */
huffman_encoder::huffman_encoder()
{
	nyt = new huffman_node(BVC_HUFFMAN_NODE_NYT, 0, node_list.create_left());
	nodes[BVC_HUFFMAN_NODE_NYT] = nyt;
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

void huffman_encoder::flush(uint8_t** out_bits, size_t* out_size)
{
	*out_bits = new uint8_t[stream->occupancy()];
	memcpy(*out_bits, stream->data(), stream->occupancy());
	*out_size = stream->occupancy();
}

void huffman_encoder::encode_internal(uint8_t in_symbol)
{
	if (nodes.count(in_symbol))
	{
		huffman_node* node = nodes[in_symbol];

		uint8_t	   bit = 0;
		bitstream* code = node->get_code();
		code->read_idx = 0;
		while (code->read_bit(&bit))
		{
			stream->write_bit(bit);
		}

		node->increment();
	}
	else
	{
		// not yet transferred
		uint8_t	   bit = 0;
		bitstream* code = nyt->get_code();
		code->read_idx = 0;
		while (code->read_bit(&bit))
		{
			stream->write_bit(bit);
		}
		stream->write_byte(in_symbol);

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

	nodes[BVC_HUFFMAN_NODE_NYT] = nyt;
	nodes[in_symbol] = value_node;
}