/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 12:12:42 by fhenrion          #+#    #+#             */
/*   Updated: 2020/01/20 22:56:40 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sha256.h"

// SHA-256 Constants
static const uint32_t k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
	0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
	0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
	0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
	0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
	0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
	0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
	0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
	0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

// SHA-256 Message
static void	sha256_message(const uint8_t data[64], uint32_t m[64])
{
	size_t	i, j;

	for (i = 0, j = 0; i < 16; i++, j += 4)
		m[i] = (data[j]<<24) | (data[j+1]<<16) | (data[j+2]<<8) | (data[j+3]);
	for (i = 16; i < 64; i++)
		m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];
}

// SHA-256 Hashing
static void	sha256_transform(t_sha256_ctx *ctx, const uint8_t data[64])
{
	uint32_t	w[8];	// words
	size_t		i;
	uint32_t	t[2];	// temporary words
	uint32_t	m[64];	// message

	// prepare the message schedule
	sha256_message(data, m);
	// Initialize the eight working variables with previous hash
	for (i = 0; i < 8; i++)
		w[i] = ctx->state[i];
	// hashing
	for (i = 0; i < 64; i++)
	{
		t[0] = w[7] + EP1(w[4]) + CH(w[4],w[5],w[6]) + k[i] + m[i];
		t[1] = EP0(w[0]) + MAJ(w[0],w[1],w[2]);
		w[7] = w[6];
		w[6] = w[5];
		w[5] = w[4];
		w[4] = w[3] + t[0];
		w[3] = w[2];
		w[2] = w[1];
		w[1] = w[0];
		w[0] = t[0] + t[1];
	}
	// save the 8 first words of the hash
	for (i = 0; i < 8; i++)
		ctx->state[i] += w[i];
}

// SHA-256 initial hash value
static void	sha256_init(t_sha256_ctx *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

static void	sha256_update(t_sha256_ctx *ctx, const uint8_t *data, size_t len)
{
	uint32_t i;

	for (i = 0; i < len; i++)
	{
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

// SHA-256 Padding
static void	sha256_final(t_sha256_ctx *ctx, uint8_t hash[32])
{
	uint32_t i;

	i = ctx->datalen;
	ctx->data[i++] = 0x80;
	if (ctx->datalen < 56)
	{
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else
	{
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);
	for (i = 0; i < 4; i++)
	{
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

void		hash_sha256(const uint8_t *data, uint8_t hash[32])
{
	t_sha256_ctx	ctx;

	sha256_init(&ctx);
	sha256_update(&ctx, data, strlen((const char*)data));
	sha256_final(&ctx, hash);
}
