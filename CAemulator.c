/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAemulator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:31:09 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/18 15:33:01 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define B(x) (1ULL << (x))

static void write_state(uint64_t state) {
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

static uint64_t generate(uint64_t state, uint8_t rule) {
	uint64_t new_state = 0;
	for (uint64_t i = 0; i < 64; ++i) {
		if (rule & B(0b111 & (state >> (i - 1) | (state << (64 - i))))) {
			new_state |= B(i);
		}
	}
	return new_state;
}

uint64_t rand_uint64(void) {
	srand(time(NULL));
	uint64_t r = 0;
	for (int i=0; i<64; i += 15 /*30*/) {
		r = r*((uint64_t)RAND_MAX + 1) + rand();
	}
	return r;
}

int main(int argc, char *argv[]) {
	uint8_t rule;
	long long iterations_nb;
	if (argc < 3) {
		printf("USAGE: rule(0-255) nb_of_iterations\n");
		return 0;
	} else {
		int input = atoi(argv[1]);
		if (input < 0 || input > 255) {
			printf("Error: rule must be in range 0-255.\n");
			return 1;
		}
		rule = (uint8_t)input;
		iterations_nb = atoll(argv[2]);
		if (iterations_nb < 0) {
			printf("Error: iterations must be positive.\n");
			return 1;
		}
	}
	uint64_t seed = rand_uint64();
	write_state(seed);
	for (long long i = 0; i < iterations_nb; ++i) {
		seed = generate(seed, rule);
		write_state(seed);
	}
	return 0;
}
