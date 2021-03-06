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
   Copyright (C) 2011 Fredrik Johansson

******************************************************************************/

#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"


void
_fmpz_poly_revert_series(fmpz * Qinv, const fmpz * Q, slong n)
{
    _fmpz_poly_revert_series_lagrange_fast(Qinv, Q, n);
}

void
fmpz_poly_revert_series(fmpz_poly_t Qinv, const fmpz_poly_t Q, slong n)
{
    fmpz *Qcopy;
    int Qalloc;
    slong Qlen = Q->length;

    if (Qlen < 2 || !fmpz_is_zero(Q->coeffs) || !fmpz_is_pm1(Q->coeffs + 1))
    {
        printf("Exception (fmpz_poly_revert_series). Input must have \n"
               "zero constant term and +1 or -1 as coefficient of x^1\n.");
        abort();
    }

    if (Qlen >= n)
    {
        Qcopy = Q->coeffs;
        Qalloc = 0;
    }
    else
    {
        slong i;
        Qcopy = (fmpz *) flint_malloc(n * sizeof(fmpz));
        for (i = 0; i < Qlen; i++)
            Qcopy[i] = Q->coeffs[i];
        for ( ; i < n; i++)
            Qcopy[i] = 0;
        Qalloc = 1;
    }

    if (Qinv != Q)
    {
        fmpz_poly_fit_length(Qinv, n);
        _fmpz_poly_revert_series(Qinv->coeffs, Qcopy, n);
    }
    else
    {
        fmpz_poly_t t;
        fmpz_poly_init2(t, n);
        _fmpz_poly_revert_series(t->coeffs, Qcopy, n);
        fmpz_poly_swap(Qinv, t);
        fmpz_poly_clear(t);
    }
    
    _fmpz_poly_set_length(Qinv, n);
    _fmpz_poly_normalise(Qinv);

    if (Qalloc)
        flint_free(Qcopy);
}
