/*============================================================================

  WCSLIB 4.4 - an implementation of the FITS WCS standard.
  Copyright (C) 1995-2009, Mark Calabretta

  This file is part of WCSLIB.

  WCSLIB is free software: you can redistribute it and/or modify it under the
  terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option)
  any later version.

  WCSLIB is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
  more details.

  You should have received a copy of the GNU Lesser General Public License
  along with WCSLIB.  If not, see <http://www.gnu.org/licenses/>.

  Correspondence concerning WCSLIB may be directed to:
    Internet email: mcalabre@atnf.csiro.au
    Postal address: Dr. Mark Calabretta
                    Australia Telescope National Facility, CSIRO
                    PO Box 76
                    Epping NSW 1710
                    AUSTRALIA

  Author: Mark Calabretta, Australia Telescope National Facility
  http://www.atnf.csiro.au/~mcalabre/index.html
  $Id: wcsutil.c,v 1.4 2009/09/14 20:25:15 irby Exp $
*===========================================================================*/

#include <string.h>

#include "wcsutil.h"

/*--------------------------------------------------------------------------*/

void wcsutil_blank_fill(int n, char c[])

{
  int k;

  for (k = strlen(c); k < n; k++) {
    c[k] = ' ';
  }

  return;
}

/*--------------------------------------------------------------------------*/

void wcsutil_null_fill(int n, char c[])

{
  int j, k;

  if (n <= 0) return;

  /* Null-fill the string. */
  *(c+n-1) = '\0';
  for (j = 0; j < n; j++) {
    if (c[j] == '\0') {
      for (k = j+1; k < n; k++) {
        c[k] = '\0';
      }
      break;
    }
  }

  for (k = j-1; k > 0; k--) {
    if (c[k] != ' ') break;
    c[k] = '\0';
  }

   return;
}

/*--------------------------------------------------------------------------*/

int wcsutil_allEq(int nvec, int nelem, const double *first)

{
  double v0;
  const double *vp;

  if (nvec <= 0 || nelem <= 0) return 0;

  v0 = *first;
  for (vp = first+nelem; vp < first + nvec*nelem; vp += nelem) {
    if (*vp != v0) return 0;
  }

  return 1;
}

/*--------------------------------------------------------------------------*/

void wcsutil_setAll(int nvec, int nelem, double *first)

{
  double v0, *vp;

  if (nvec <= 0 || nelem <= 0) return;

  v0 = *first;
  for (vp = first+nelem; vp < first + nvec*nelem; vp += nelem) {
    *vp = v0;
  }
}

/*--------------------------------------------------------------------------*/

void wcsutil_setAli(int nvec, int nelem, int *first)

{
  int v0, *vp;

  if (nvec <= 0 || nelem <= 0) return;

  v0 = *first;
  for (vp = first+nelem; vp < first + nvec*nelem; vp += nelem) {
    *vp = v0;
  }
}

/*--------------------------------------------------------------------------*/

void wcsutil_setBit(int nelem, const int *sel, int bits, int *array)

{
  int *arrp;

  if (bits == 0 || nelem <= 0) return;

  if (sel == 0x0) {
    /* All elements selected. */
    for (arrp = array; arrp < array + nelem; arrp++) {
      *arrp |= bits;
    }

  } else {
    /* Some elements selected. */
    for (arrp = array; arrp < array + nelem; arrp++) {
      if (*(sel++)) *arrp |= bits;
    }
  }
}
