#include <stdio.h>
#include <limits.h>
 
typedef unsigned long long ull;
// Nombre de bits d'un octet * le nombre d'octets d'un unsigned long long.
#define N  (sizeof(ull) * CHAR_BIT)
// Masque x sur un unsigned long long.
#define B(x) (1ULL << (x))
 
ull evolve(ull state, int rule, int nb_gen)
{
	int     i;
	ull     st;
    char    rules[8] = {30, 101, 75, 149, 89, 45, 135, 86};

    while (nb_gen--)
	{
		st = state;
        for (i = N; i; i--)
            putchar(st & B(i) ? '1' : '0');
		putchar('\n');
        state = 0;
		for (i = 0; i < N; i++)
			if (rules[rule] & B(7 & (st>>(i-1) | st<<(N+1-i))))
				state |= B(i);
        if (++rule > 8)
            rule = 0;
	}
    return st;
}

// TODO pour le chiffrement :
// - lire un fd ull par ull
// - appliquer l'automate sur des blocs de taille ull.

int main(int argc, char **argv)
{
	evolve(2343344589778499735, 0, 600);
	return 0;
}