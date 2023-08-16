#pragma once

#include <map>
#include <set>

#include "bvc_common/entropy/huffman/huffman_node.h"
#include "bvc_common/entropy/huffman/linked_list.h"
#include "bvc_common/entropy/huffman/node.h"

#include "bvc_dec/entropy_coding/entropy_decoder.h"

class huffman_decoder : public bvc_entropy_decoder
{
public:
	huffman_decoder();
	~huffman_decoder();

	virtual void decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols) override;

	virtual void flush(uint8_t** out_bits, size_t* out_size) override;

private:
	virtual void clear() override;

	void		  expand_nyt(int64_t in_symbol);
	huffman_node* traverse_tree(huffman_node* in_node);

private:
	bitstream*	  output;
	huffman_node* nyt;

	std::map<int64_t, huffman_node*> nodes;
	linked_list<huffman_node*>		 node_list;
};