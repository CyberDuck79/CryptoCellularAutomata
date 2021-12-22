/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca64.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 15:19:56 by fhenrion          #+#    #+#             */
/*   Updated: 2021/04/11 15:17:33 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/ca64.h"

void write_state(const uint64_t state) {
	static const char square[6] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88};

	for (uint64_t i = 0; i < 64; ++i) {
		if (state & B(i)) {
			write(1, square, 6);
		} else {
			write(1, "  ", 2);
		}
	}
	write(1, "\n", 1);
}

	/* Cryptographic algorithm using a 1D cellular automata with radius 4. */

uint64_t generate(const uint64_t state, const int rule_i) {
	
	/* This algorithm takes as input an unsigned long long (uint64_t), 
	evolves it with cellular automata rules, and returns the modified uint64. */
	
	/* Five 1D radius 4 CA rules are alternately used to generate the CA states. */
	static uint16_t rules[5] = {39318, 42390, 43350, 51510, 57630};
	
	uint64_t new_state = 0;
	
	/* A for loop is used to fill in each bit of a uint64_t according to the cellular automata rule. */
	
	for (uint64_t i = 0; i < 64; ++i) {
		
	/* Each rule is applied directly to the uint64_t using bitwise operations. The net effect of these
	operations on the particular bit will depend on the value of the rule itself. */
		
		if (rules[rule_i] & B(0b1111 & ((state >> (i - 1)) | (state << (63 - i)) | (state << (64 - i))))) {
			new_state |= B(i);
			
	/* Goes through 64 bits of uint64_t and evolves them according to the rule. */
		}
	}
	#ifdef DEBUG
		write_state(state); // -> output for tests
	#endif
	return new_state;
	/* Note that the function returns a new uint64_t initialized within the function
	(memory was not allocated for this). */
}
