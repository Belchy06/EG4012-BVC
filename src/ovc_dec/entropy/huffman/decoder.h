#pragma once

#include <map>
#include <set>

#include "ovc_common/entropy/huffman/huffman_node.h"
#include "ovc_common/linked_list/node.h"
#include "ovc_common/linked_list/linked_list.h"
#include "ovc_dec/entropy/decoder.h"

class huffman_decoder : public ovc_entropy_decoder
{
public:
	huffman_decoder();
	~huffman_decoder();

	virtual void decode(uint8_t* in_bytes, size_t in_size, size_t in_num_symbols) override;

	virtual void flush(uint8_t** out_bytes, size_t* out_size) override;

private:
	void clear();

	void		  expand_nyt(int64_t in_symbol);
	huffman_node* traverse_tree(huffman_node* in_node);

private:
	ovc_bitstream* output;
	huffman_node*  nyt;

	std::map<int64_t, huffman_node*> nodes;
	linked_list<huffman_node*>		 node_list;
};