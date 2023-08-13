#include <cstring>

#include "huffman_encoder.h"

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
	// terminating bytes
	encode_internal(0);
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
		bitstream*	  code = node->get_code();
		uint8_t*	  data = code->data();
		size_t		  num_bits = code->size();

		size_t bit_idx = 0;
		while (bit_idx < num_bits)
		{
			size_t byte = bit_idx / 8;
			size_t bit = bit_idx % 8;

			uint8_t temp = (data[byte] >> bit) & 0x1;
			stream->write_bit(temp);
			bit_idx++;
		}

		node->increment();
	}
	else
	{
		// not yet transferred
		uint8_t	   bit = 0;
		bitstream* code = nyt->get_code();
		while (code->read_bit(&bit))
		{
			stream->write_bit(bit);
		}

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