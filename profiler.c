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

    Copyright (C) 2007 William Hart and David Harvey
    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "profiler.h"

/*
   clock_last[i] is the last read clock value for clock #i.
   clock_accum[i] is the total time attributed to clock #i so far.
   These should not be read directly; use get_clock(i) instead.
 */
double clock_last[FLINT_NUM_CLOCKS];
double clock_accum[FLINT_NUM_CLOCKS];

void
prof_repeat(double *min, double *max, profile_target_t target, void *arg)
{
    /* Number of timings that were at least DURATION_THRESHOLD microseconds */
    ulong good_count = 0;
    double max_time = DBL_MIN, min_time = DBL_MAX;

    /* First try one loop */
    ulong num_trials = 4;
    double last_time;
    init_clock(0);
    target(arg, num_trials);
    last_time = get_clock(0);

    /* Loop until we have enough good times */
    while (1)
    {
        double per_trial = last_time / num_trials;

        /* If the last recorded time was long enough, record it */
        if (last_time > DURATION_THRESHOLD)
        {
            if (good_count)
            {
                if (per_trial > max_time)
                    max_time = per_trial;
                if (per_trial < min_time)
                    min_time = per_trial;
            }
            else
                max_time = min_time = per_trial;

            if (++good_count == 5)
            {
                /* We've got enough data */
                break;
            }
        }

        /*
           Adjust num_trials so that the elapsed time gravitates towards
           DURATION_TARGET; num_trials can be changed by a factor of
           at most 25%, and must be at least 1
         */
        {
            double adjust_ratio;
            if (last_time < 0.0001)
                last_time = 0.0001;
            adjust_ratio = DURATION_TARGET / last_time;
            if (adjust_ratio > 1.25)
                adjust_ratio = 1.25;
            if (adjust_ratio < 0.75)
                adjust_ratio = 0.75;
            num_trials = (ulong) ceil(adjust_ratio * num_trials);
            /* Just to be safe */
            if (num_trials == 0)
                num_trials = 1;
        }

        /* Run another trial */
        init_clock(0);
        target(arg, num_trials);
        last_time = get_clock(0);
    }

    /* Store results */
    if (min)
        *min = min_time;
    if (max)
        *max = max_time;
}

void get_memory_usage(meminfo_t meminfo)
{
    FILE * file = fopen("/proc/self/status", "r");

    ulong result;
    char line[128];

    while (fgets(line, 128, file) != NULL)
    {
        result = 0;

        if (strncmp(line, "VmSize:", 7) == 0)
        {
            sscanf(line, "VmSize: %lu kB\n", &result);
            meminfo->size = result;
        }
        else if (strncmp(line, "VmPeak:", 7) == 0)
        {
            sscanf(line, "VmPeak: %lu kB\n", &result);
            meminfo->peak = result;
        }
        else if (strncmp(line, "VmHWM:", 6) == 0)
        {
            sscanf(line, "VmHWM: %lu kB\n", &result);
            meminfo->hwm = result;
        }
        else if (strncmp(line, "VmRSS:", 6) == 0)
        {
            sscanf(line, "VmRSS: %lu kB\n", &result);
            meminfo->rss = result;
        }
    }

    fclose(file);
}

