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

    Copyright (C) 2011 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "fmpz_poly_mat.h"
#include "ulong_extras.h"

int
main(void)
{
    slong i;
    flint_rand_t state;

    printf("trace....");
    fflush(stdout);

    flint_randinit(state);

    /* Test trace(AB) = trace(BA) */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fmpz_poly_mat_t A, B, AB, BA;
        fmpz_poly_t trab, trba;
        slong m, n;

        m = n_randint(state, 10);
        n = n_randint(state, 10);

        fmpz_poly_mat_init(A, m, n);
        fmpz_poly_mat_init(B, n, m);
        fmpz_poly_mat_init(AB, m, m);
        fmpz_poly_mat_init(BA, n, n);

        fmpz_poly_init(trab);
        fmpz_poly_init(trba);

        fmpz_poly_mat_randtest(A, state, 1 + n_randint(state, 10),
            1 + n_randint(state, 100));
        fmpz_poly_mat_randtest(B, state, 1 + n_randint(state, 10),
            1 + n_randint(state, 100));

        fmpz_poly_mat_mul(AB, A, B);
        fmpz_poly_mat_mul(BA, B, A);

        fmpz_poly_mat_trace(trab, AB);
        fmpz_poly_mat_trace(trba, BA);

        if (!fmpz_poly_equal(trab, trba))
        {
            printf("FAIL:\n");
            fmpz_poly_mat_print(A, "x"), printf("\n");
            fmpz_poly_mat_print(B, "x"), printf("\n");
            fmpz_poly_mat_print(AB, "x"), printf("\n");
            fmpz_poly_mat_print(BA, "x"), printf("\n");
            printf("tr(AB): "),  fmpz_poly_print(trab),    printf("\n");
            printf("tr(BA): "),  fmpz_poly_print(trba),    printf("\n");
            abort();
        }

        fmpz_poly_mat_clear(A);
        fmpz_poly_mat_clear(B);
        fmpz_poly_mat_clear(AB);
        fmpz_poly_mat_clear(BA);
        fmpz_poly_clear(trab);
        fmpz_poly_clear(trba);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return 0;
}
