/* prisoners.c -- program to simulate the prisoners problem
 * with montecarlo method.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Fri Sep 16 22:14:35 EEST 2022
 * Copyright: (c) 2022 Luis Colorado.  All rights reserved.
 * License: BSD.
 */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FLG_QUIET (1 << 0)
#define RANDOM(n) ((unsigned)random() % (n))

int flags = 0;

unsigned experiment(unsigned n)
{
    unsigned *box  = calloc(n, sizeof *box),
              n2   = n >> 1;
    uint64_t *bits = calloc((n + 63) >> 6, sizeof *bits);

    for (unsigned i = 0; i < n; i++) {
         box[i] = i;
    }

    /* random permutation */
    for (unsigned i = 0, j; i < n; i++) {
        j = i + RANDOM(n - i);
        if (i != j) {
            unsigned
				temp = box[i];
              box[i] = box[j];
              box[j] = temp;
        }
	} /* for */

	/* print permutation */
	if (!(flags & FLG_QUIET)) {
		char *sep  = "";
		for (unsigned i = 0; i < n; i++) {
			printf("%s[%d]->%d", sep, i + 1, box[i] + 1);
			sep = ", ";
        }
        printf("\n");
    }

    /* the experiment */
    unsigned ret_val = 0;
    for (unsigned i = 0; i < n; i++) {
        if (bits[i >> 6] & (1UL << (i & 63)))
            continue;
        unsigned k = 1;
        if (!(flags & FLG_QUIET))
            printf("(%d", i + 1);
        for (unsigned j = box[i]; j != i; j = box[j], k++) {
            bits[j >> 6] |= (1UL << (j & 63));
            if (!(flags & FLG_QUIET))
                printf(", %d", j + 1);
        }
        if (!(flags & FLG_QUIET))
            printf(")<%u>%c\n", k,
					k <= n2
						? '*'
						: '?');
        if (k <= n2)
            ret_val += k;
		else {
			/* if we are not going to print the
			 * cycles, there's no chance to continue
			 * as this experiment has already failed.
			 */
			if (flags & FLG_QUIET)
				break;
		}
    } /* for */
    free(box);
    free(bits);
    return ret_val;
} /* experiment */

int main(int argc, char **argv)
{
    unsigned
		N    = 100,
        ITER =   1,
        opt;

    while ((opt = getopt(argc, argv, "i:n:qv")) != EOF) {
        switch (opt) {
        case 'n': N        =  atoi(optarg); break;
        case 'i': ITER     =  atoi(optarg);
				  if (ITER > 1)
					flags |=  FLG_QUIET;
				  break;
        case 'q': flags   |=  FLG_QUIET;    break;
		case 'v': flags   &= ~FLG_QUIET;    break;
        }
    } /* while */

    srandomdev();
    unsigned
		N2    = N >> 1,
        denom =   ITER,
		num   =      0;

    printf("Calc. %u times, %u prisoners\n", ITER, N);

    for (unsigned i = 0; i < denom; i++)
        if (experiment(N) == N)
            num++;

    printf("Estim. Montecarlo: %d/%d ~= %0.10lf\n",
            num, denom, (double) num / denom);

    /* calculation of 1 - sum(i; n2; n; 1/i) */

    double prob = 1.0;
    for (int i = N2 + 1; i <= N; i++)
        prob -= 1.0 / i;

    printf("  Calculated prob: %0.10lf\n", prob);
} /* main */
