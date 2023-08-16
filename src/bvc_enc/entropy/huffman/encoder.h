#pragma once

#include <map>
#include <set>

#include "bvc_common/entropy/huffman/huffman_node.h"
#include "bvc_common/linked_list/linked_list.h"
#include "bvc_common/linked_list/node.h"
#include "bvc_enc/entropy/entropy_encoder.h"

class huffman_encoder : public bvc_entropy_encoder
{
public:
	huffman_encoder();
	~huffman_encoder();

	virtual void encode(const uint8_t* in_bytes, size_t in_size) override;
	virtual void flush(uint8_t** out_bits, size_t* out_size) override;

private:
	void encode_internal(uint8_t in_symbol);
	void expand_nyt(int64_t in_symbol);

private:
	huffman_node* nyt;

	std::map<int64_t, huffman_node*> nodes;
	linked_list<huffman_node*>		 node_list;
};