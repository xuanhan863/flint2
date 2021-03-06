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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static char * docsin[] = {
    "../../fmpz/doc/fmpz.txt", 
    "../../fmpz_vec/doc/fmpz_vec.txt", 
    "../../fmpz_factor/doc/fmpz_factor.txt", 
    "../../fmpz_mat/doc/fmpz_mat.txt", 
    "../../fmpz_poly/doc/fmpz_poly.txt", 
    "../../fmpz_poly_factor/doc/fmpz_poly_factor.txt", 
    "../../fmpq/doc/fmpq.txt", 
    "../../fmpq_mat/doc/fmpq_mat.txt", 
    "../../fmpq_poly/doc/fmpq_poly.txt", 
    "../../fmpz_poly_q/doc/fmpz_poly_q.txt", 
    "../../fmpz_poly_mat/doc/fmpz_poly_mat.txt", 
    "../../nmod_vec/doc/nmod_vec.txt",
    "../../nmod_mat/doc/nmod_mat.txt",
    "../../nmod_poly/doc/nmod_poly.txt",
    "../../nmod_poly_mat/doc/nmod_poly_mat.txt",
    "../../fmpz_mod_poly/doc/fmpz_mod_poly.txt",
    "../../padic/doc/padic.txt", 
    "../../arith/doc/arith.txt", 
    "../../ulong_extras/doc/ulong_extras.txt",
    "../../long_extras/doc/long_extras.txt",
    "../../doc/longlong.txt",
    "../../mpn_extras/doc/mpn_extras.txt",
    "../../doc/profiler.txt", 
    "../../interfaces/doc/interfaces.txt",
    "../../fft/doc/fft.txt",
    "../../qsieve/doc/qsieve.txt",
    "../../perm/doc/perm.txt",
};

static char * docsout[] = {
    "input/fmpz.tex", 
    "input/fmpz_vec.tex", 
    "input/fmpz_factor.tex", 
    "input/fmpz_mat.tex",
    "input/fmpz_poly.tex", 
    "input/fmpz_poly_factor.tex", 
    "input/fmpq.tex", 
    "input/fmpq_mat.tex", 
    "input/fmpq_poly.tex", 
    "input/fmpz_poly_q.tex", 
    "input/fmpz_poly_mat.tex", 
    "input/nmod_vec.tex",
    "input/nmod_mat.tex",
    "input/nmod_poly.tex",
    "input/nmod_poly_mat.tex",
    "input/fmpz_mod_poly.tex",
    "input/padic.tex", 
    "input/arith.tex", 
    "input/ulong_extras.tex",
    "input/long_extras.tex",
    "input/longlong.tex", 
    "input/mpn_extras.tex",
    "input/profiler.tex", 
    "input/interfaces.tex",
    "input/fft.tex",
    "input/qsieve.tex",
    "input/perm.tex",
};


static const int ndocs = sizeof(docsin) / sizeof(char *);

static FILE *in, *out;              /* Input and output handles           */

static int line;                    /* Current line number                */
static int error;


/* print latex code for the function prototype "text" of length "len" */
void
printfuncheader(const char* text, int len)
{
    /* We try to be clever and remove newlines and any whitespaces following
       them. */
    fprintf(out, "\n");
    fprintf(out, "\\vspace*{0.5em}\n");
    fprintf(out, "\\begin{lstlisting}\n");
    int i = 0;
    while(i < len)
    {
        if(text[i] != '\n')
            fprintf(out, "%c", text[i++]);
        else
        {
            int hasspace = text[i - 1] == ' ';
            while(i < len-1 && text[++i] == ' ');
            if(!hasspace)
                fprintf(out, " ");
        }
    }
    fprintf(out, "\n\\end{lstlisting}\n");
    fprintf(out, "\\vspace*{-0.5em}\n");
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* We read at most 80 characters at a time. Reading more makes error reporting
   less reliable, reading less makes the debug output even more horrible. */
#define YY_INPUT(buf, result, max_size) \
{ \
    result = fread(buf, 1, MIN(max_size, 80), in); \
    int myindex; \
    for(myindex = 0;myindex < result;++myindex) line += (buf[myindex] == '\n'); \
}
/* use this debug switch if you are desperate enough */
#if 0
#define YY_DEBUG
#endif

/* parser definition, automatically generated from create_doc.leg */
#include "create_doc_gen.c"

int main(void)
{
    int i;

    for (i = 0; i < ndocs; i++)
    {
        char* name = docsin[i];
        line = 0;
        in   = fopen(docsin[i], "r");
        out  = fopen(docsout[i], "w");

        if(yyparse() == 0 || !feof(in))
        {
            printf("\n");
            printf("Parse exception:\n");
            printf("Encountered malformed input near line %d \n", line);
            printf("in file %s.\n\n", name);
            return 1;
        }

        fclose(in);
        fclose(out);
    }

    return EXIT_SUCCESS;
}

