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

uint64_t generate(const uint64_t state, const int rule_i) {
	static uint16_t rules[5] = {39318, 42390, 43350, 51510, 57630};
	uint64_t new_state = 0;

	for (uint64_t i = 0; i < 64; ++i) {
		if (rules[rule_i] & B(0b1111 & ((state >> (i - 1)) | (state << (63 - i)) | (state << (64 - i))))) {
			new_state |= B(i);
		}
	}
	#ifdef DEBUG
		write_state(state); // -> output for tests
	#endif
	return new_state;
}