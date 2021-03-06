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

    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "ulong_extras.h"
#include "fmpz.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("cmp_ui....");
    fflush(stdout);

    flint_randinit(state);

    /* Compare with fmpz_cmp */
    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpz_t a, b;
        ulong n;
        int lhs, rhs;

        fmpz_init(a);
        fmpz_init(b);

        fmpz_randtest(a, state, 200);

        n = n_randtest(state);
        fmpz_set_ui(b, n);

        lhs = fmpz_cmp(a, b);
        rhs = fmpz_cmp_ui(a, n);

        result = (lhs < 0) ? (rhs < 0) : ((lhs > 0) ? (rhs > 0) : (rhs == 0));
        if (result == 0)
        {
            printf("FAIL:\n");
            printf("a = "), fmpz_print(a), printf("\n");
            printf("b = "), fmpz_print(b), printf("\n");
            printf("n = %lu\n", n);
            printf("cmp(a, b) = %d\n", fmpz_cmp(a, b));
            printf("cmp_ui(a, n) = %d\n", fmpz_cmp_ui(a, n));
            abort();
        }

        fmpz_clear(a);
        fmpz_clear(b);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
