/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011, 2012 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "long_extras.h"
#include "ulong_extras.h"
#include "padic_poly.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("add... ");
    fflush(stdout);

    flint_randinit(state);

    /* Check aliasing of a and c */
    for (i = 0; i < 10000; i++)
    {
        fmpz_t p;
        slong N;
        padic_ctx_t ctx;

        padic_poly_t a, b, c;

        fmpz_init_set_ui(p, n_randtest_prime(state, 0));
        N = n_randint(state, PADIC_TEST_PREC_MAX - PADIC_TEST_PREC_MIN) 
            + PADIC_TEST_PREC_MIN;
        padic_ctx_init(ctx, p, FLINT_MAX(0, N-10), FLINT_MAX(0, N+10), PADIC_SERIES);

        padic_poly_init2(a, 0, N);
        padic_poly_init2(b, 0, N);
        padic_poly_init2(c, 0, N);

        padic_poly_randtest(a, state, n_randint(state, 100), ctx);
        padic_poly_randtest(b, state, n_randint(state, 100), ctx);

        padic_poly_add(c, a, b, ctx);
        padic_poly_add(a, a, b, ctx);

        result = (padic_poly_equal(a, c) && padic_poly_is_reduced(a, ctx));
        if (!result)
        {
            printf("FAIL (alias a, c):\n");
            printf("a  = "), padic_poly_print(a, ctx), printf("\n\n");
            printf("b  = "), padic_poly_print(b, ctx), printf("\n\n");
            printf("c  = "), padic_poly_print(c, ctx), printf("\n\n");
            abort();
        }

        padic_poly_clear(a);
        padic_poly_clear(b);
        padic_poly_clear(c);

        padic_ctx_clear(ctx);
        fmpz_clear(p);
    }

    /* Check aliasing of b and c */
    for (i = 0; i < 10000; i++)
    {
        fmpz_t p;
        slong N;
        padic_ctx_t ctx;

        padic_poly_t a, b, c;

        fmpz_init_set_ui(p, n_randtest_prime(state, 0));
        N = n_randint(state, PADIC_TEST_PREC_MAX - PADIC_TEST_PREC_MIN) 
            + PADIC_TEST_PREC_MIN;
        padic_ctx_init(ctx, p, FLINT_MAX(0, N-10), FLINT_MAX(0, N+10), PADIC_SERIES);

        padic_poly_init2(a, 0, N);
        padic_poly_init2(b, 0, N);
        padic_poly_init2(c, 0, N);

        padic_poly_randtest(a, state, n_randint(state, 100), ctx);
        padic_poly_randtest(b, state, n_randint(state, 100), ctx);

        padic_poly_add(c, a, b, ctx);
        padic_poly_add(b, a, b, ctx);

        result = (padic_poly_equal(b, c) && padic_poly_is_reduced(b, ctx));
        if (!result)
        {
            printf("FAIL (alias b, c):\n");
            padic_poly_print(a, ctx), printf("\n\n");
            padic_poly_print(b, ctx), printf("\n\n");
            padic_poly_print(c, ctx), printf("\n\n");
            abort();
        }

        padic_poly_clear(a);
        padic_poly_clear(b);
        padic_poly_clear(c);

        padic_ctx_clear(ctx);
        fmpz_clear(p);
    }

    /* Compare with Q */
    for (i = 0; i < 10000; i++)
    {
        fmpz_t p;
        slong N;
        padic_ctx_t ctx;

        padic_poly_t a, b, c, d;
        fmpq_poly_t x, y, z;

        fmpz_init_set_ui(p, n_randtest_prime(state, 0));
        N = n_randint(state, PADIC_TEST_PREC_MAX - PADIC_TEST_PREC_MIN) 
            + PADIC_TEST_PREC_MIN;
        padic_ctx_init(ctx, p, FLINT_MAX(0, N-10), FLINT_MAX(0, N+10), PADIC_SERIES);

        padic_poly_init2(a, 0, N);
        padic_poly_init2(b, 0, N);
        padic_poly_init2(c, 0, N);
        padic_poly_init2(d, 0, N);

        fmpq_poly_init(x);
        fmpq_poly_init(y);
        fmpq_poly_init(z);

        padic_poly_randtest(b, state, n_randint(state, 50), ctx);
        padic_poly_randtest(c, state, n_randint(state, 50), ctx);

        padic_poly_add(a, b, c, ctx);

        padic_poly_get_fmpq_poly(y, b, ctx);
        padic_poly_get_fmpq_poly(z, c, ctx);

        fmpq_poly_add(x, y, z);
        padic_poly_set_fmpq_poly(d, x, ctx);

        result = (padic_poly_equal(a, d) && padic_poly_is_reduced(a, ctx));
        if (!result)
        {
            printf("FAIL (cmp with Q):\n");
            printf("N = %ld, val(b) = %ld, val(c) = %ld\n", N, b->val, c->val);
            padic_poly_print(c, ctx), printf("\n\n");
            padic_poly_print(d, ctx), printf("\n\n");
            abort();
        }

        padic_poly_clear(a);
        padic_poly_clear(b);
        padic_poly_clear(c);
        padic_poly_clear(d);

        fmpq_poly_clear(x);
        fmpq_poly_clear(y);
        fmpq_poly_clear(z);

        padic_ctx_clear(ctx);
        fmpz_clear(p);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
