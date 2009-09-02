/*============================================================================

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

===============================================================================*/
/****************************************************************************

   Copyright (C) 2009 William Hart
   
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <mpir.h>
#include "flint.h"
#include "ulong_extras.h"

mp_limb_t flint_pseudosquares[] = {17, 73, 241, 1009, 2641, 8089, 18001, 
          53881, 87481, 117049, 515761, 1083289, 3206641, 3818929, 9257329, 
          22000801, 48473881, 48473881, 175244281, 427733329, 427733329, 
          898716289, 2805544681, 2805544681, 2805544681, 10310263441,
          23616331489, 85157610409, 85157610409, 196265095009, 196265095009,
          2871842842801, 2871842842801, 2871842842801, 26250887023729,
          26250887023729, 112434732901969, 112434732901969, 112434732901969,
          178936222537081, 178936222537081, 696161110209049, 696161110209049,
          2854909648103881, 6450045516630769, 6450045516630769, 
          11641399247947921, 11641399247947921, 190621428905186449,
          196640248121928601, 712624335095093521, 1773855791877850321 };

#define FLINT_NUM_PSEUDOSQUARES 52

int n_is_prime_pseudosquare(mp_limb_t n)
{
   if (n < 2UL) return 0;
   
   if (n & 1UL == 0UL) 
   {
      if (n == 2UL) return 1;
      return 0;
   }
   
   n_compute_primes(FLINT_PSEUDOSQUARES_CUTOFF);
   mp_limb_t p;

   for (ulong i = 0; i < FLINT_PSEUDOSQUARES_CUTOFF; i++)
   {
      p = flint_primes[i];
      if (p*p > n) return 1;
      double ppre = flint_prime_inverses[i];
      if (!n_mod2_precomp(n, p, ppre)) return 0;
   }

   mp_limb_t B = flint_primes[FLINT_PSEUDOSQUARES_CUTOFF];

   mp_limb_t NB = (n - 1)/B + 1;

   unsigned int i, j, m1 = 0;

   for (i = 0; i < FLINT_NUM_PSEUDOSQUARES; i++)
   {
      if (flint_pseudosquares[i] > NB) break;
   }

   mp_limb_t exp = (n - 1)/2;

   for (j = 0; j <= i; j++)
   {
      mp_limb_t mod = n_powmod2(flint_primes[j], exp, n);
      if ((mod != 1UL) && (mod != n - 1)) return 0;
      if (mod == n - 1) m1 = 1;
   }

   mp_limb_t mod8 = n % 8;

   if ((mod8 == 3) || (mod8 == 7)) return 1;

   if (mod8 == 5)
   {
      mp_limb_t mod = n_powmod2(2UL, exp, n);
      if (mod == n - 1) return 1;
      printf("Whoah, %lu is a probable prime, but not prime, please report!!\n", n);
      abort();
   } else
   {
      if (m1) return 1;
      for (j = i + 1; j < FLINT_NUM_PSEUDOSQUARES + 1; j++)
      {
         mp_limb_t mod = n_powmod(flint_primes[j], exp, n);
         if (mod == n - 1) return 1;
         if (mod != 1)
         {
            printf("Whoah, %lu is a probable prime, but not prime, please report!!\n", n);
            abort();
         }
      }
      printf("Whoah, %lu is a probable prime, but not prime, please report!!\n", n);
      abort();
   }
}