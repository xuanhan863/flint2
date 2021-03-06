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

#include "flint.h"
#include "long_extras.h"
#include "ulong_extras.h"
#include "padic_poly.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    padic_ctx_t ctx;
    fmpz_t p;
    slong N;

    printf("get/set_fmpq_poly... ");
    fflush(stdout);

    flint_randinit(state);

    /* Qp -> Q -> Qp */
    for (i = 0; i < 10000; i++)
    {
        padic_poly_t a, b;
        fmpq_poly_t c;

        fmpz_init_set_ui(p, n_randtest_prime(state, 0));
        N = n_randint(state, PADIC_TEST_PREC_MAX - PADIC_TEST_PREC_MIN) 
            + PADIC_TEST_PREC_MIN;
        padic_ctx_init(ctx, p, FLINT_MAX(0, N-10), FLINT_MAX(0, N+10), PADIC_SERIES);

        padic_poly_init2(a, 0, N);
        padic_poly_init2(b, 0, N);
        fmpq_poly_init(c);

        padic_poly_randtest(a, state, n_randint(state, 10), ctx);

        padic_poly_get_fmpq_poly(c, a, ctx);
        padic_poly_set_fmpq_poly(b, c, ctx);

        result = (padic_poly_equal(a, b) && padic_poly_is_reduced(a, ctx));
        if (!result)
        {
            printf("FAIL:\n");
            printf("a = "), padic_poly_debug(a), printf("\n\n");
            printf("b = "), padic_poly_debug(b), printf("\n\n");
            printf("c = "), fmpq_poly_print(c), printf("\n\n");
            abort();
        }

        padic_poly_clear(a);
        padic_poly_clear(b);
        fmpq_poly_clear(c);

        padic_ctx_clear(ctx);
        fmpz_clear(p);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
