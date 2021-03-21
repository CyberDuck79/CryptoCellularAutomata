/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca_hash64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 14:27:19 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 15:36:13 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/ca64.h"

static uint64_t to_block_padding(const char *str_block, size_t padding) {
	uint64_t block = 0;
	size_t i = 0;
	
	while (i < padding) {
		block |= (uint64_t)str_block[i] << (i * 8);
		++i;
	}
	while (i < 8) {
		block |= (uint64_t)'#' << (i * 8);
		++i;
	}
	return block;
}

static uint64_t to_block(const char *str_block) {
	uint64_t block = 0;
	
	for (size_t i = 0; i < 8; ++i) {
		block |= (uint64_t)str_block[i] << (i * 8);
	}
	return block;
}

uint64_t generate_hash(const char *passphrase) {
	const size_t len = strlen(passphrase);
	const size_t nb_blocks = len / 8;
	const size_t padding = len % 8;
	
	uint64_t block;
	uint64_t hash = 0;
	int rule_i = nb_blocks % 5;
	
	for (size_t i = 0; i < nb_blocks; ++i) {
		block = to_block(passphrase);
		passphrase += 8;
		hash ^= generate(block, rule_i);
		for (size_t j = 0; j < 8; ++j) {
			hash ^= generate(hash, rule_i);
		}
		rule_i = shift_rule(block, rule_i);
	}
	if (padding) {
		block = to_block_padding(passphrase, padding);
		hash ^= generate(block, rule_i);
		for (size_t j = 0; j < 8; ++j) {
			hash ^= generate(hash, rule_i);
		}
	}
	return hash;
}
