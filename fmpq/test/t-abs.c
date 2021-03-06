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

    Copyright (C) 2009 William Hart
    Copyright (C) 2013 Tom Bachmann (adapt for fmpq)

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "fmpq.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("abs....");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpq_t a, b;
        mpq_t c, d;

        fmpq_init(a);
        fmpq_init(b);
        mpq_init(c);
        mpq_init(d);

        fmpq_randtest(a, state, 200);

        fmpq_get_mpq(c, a);

        fmpq_abs(b, a);
        mpq_abs(c, c);

        fmpq_get_mpq(d, b);

        result = (mpq_cmp(c, d) == 0);
        if (!result)
        {
            printf("FAIL:\n");
            gmp_printf("c = %Qd, d = %Qd\n", c, d);
            abort();
        }

        fmpq_clear(a);
        fmpq_clear(b);
        mpq_clear(c);
        mpq_clear(d);
    }

    /* Check aliasing */
    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpq_t a;
        mpq_t c, d;

        fmpq_init(a);
        mpq_init(c);
        mpq_init(d);

        fmpq_randtest(a, state, 200);

        fmpq_get_mpq(c, a);

        fmpq_abs(a, a);
        mpq_abs(c, c);

        fmpq_get_mpq(d, a);

        result = (mpq_cmp(c, d) == 0);
        if (!result)
        {
            printf("FAIL:\n");
            gmp_printf("c = %Qd, d = %Qd\n", c, d);
            abort();
        }

        fmpq_clear(a);
        mpq_clear(c);
        mpq_clear(d);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return 0;
}

