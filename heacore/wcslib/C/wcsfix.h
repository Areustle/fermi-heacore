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
  $Id: wcsfix.h,v 1.4 2009/09/14 20:25:15 irby Exp $
*=============================================================================
*
* WCSLIB 4.4 - C routines that implement the FITS World Coordinate System
* (WCS) standard.  Refer to
*
*   "Representations of world coordinates in FITS",
*   Greisen, E.W., & Calabretta, M.R. 2002, A&A, 395, 1061 (Paper I)
*
*   "Representations of celestial coordinates in FITS",
*   Calabretta, M.R., & Greisen, E.W. 2002, A&A, 395, 1077 (Paper II)
*
*   "Representations of spectral coordinates in FITS",
*   Greisen, E.W., Calabretta, M.R., Valdes, F.G., & Allen, S.L.
*   2006, A&A, 446, 747 (Paper III)
*
* Refer to the README file provided with WCSLIB for an overview of the
* library.
*
*
* Summary of the wcsfix routines
* ------------------------------
* Routines in this suite identify and translate various forms of non-standard
* construct that are known to occur in FITS WCS headers.  These range from the
* translation of non-standard values for standard WCS keywords, to the repair
* of malformed coordinate representations.
*
* Non-standard keyvalues:
* -----------------------
*   AIPS-convention celestial projection types, NCP and GLS, and spectral
*   types, 'FREQ-LSR', 'FELO-HEL', etc., set in CTYPEia are translated
*   on-the-fly by wcsset() but without modifying the relevant ctype[], pv[] or
*   specsys members of the wcsprm struct.  That is, only the information
*   extracted from ctype[] is translated when wcsset() fills in wcsprm::cel
*   (celprm struct) or wcsprm::spc (spcprm struct).
*
*   On the other hand, these routines do change the values of wcsprm::ctype[],
*   wcsprm::pv[], wcsprm::specsys and other wcsprm struct members as
*   appropriate to produce the same result as if the FITS header itself had
*   been translated.
*
*   Auxiliary WCS header information not used directly by WCSLIB may also be
*   translated.  For example, the older DATE-OBS date format (wcsprm::dateobs)
*   is recast to year-2000 standard form, and MJD-OBS (wcsprm::mjdobs) will be
*   deduced from it if not already set.
*
*   Certain combinations of keyvalues that result in malformed coordinate
*   systems, as described in Sect. 7.3.4 of Paper I, may also be repaired.
*   These are handled by cylfix().
*
* Non-standard keywords:
* ----------------------
*   The AIPS-convention CROTAn keywords are recognized as quasi-standard and
*   as such are accomodated by the wcsprm::crota[] and translated to
*   wcsprm::pc[][] by wcsset().  These are not dealt with here, nor are any
*   other non-standard keywords since these routines work only on the contents
*   of a wcsprm struct and do not deal with FITS headers per se.  In
*   particular, they do not identify or translate CD00i00j, PC00i00j, PROJPn,
*   EPOCH, VELREF or VSOURCEa keywords; this may be done by the FITS WCS
*   header parser supplied with WCSLIB, refer to wcshdr.h.
*
* wcsfix() applies all of the corrections handled by the following specific
* functions which may also be invoked separately:
*
*   - cdfix(): Sets the diagonal element of the CDi_ja matrix to 1.0 if all
*     CDi_ja keywords associated with a particular axis are omitted.
*
*   - datfix(): recast an older DATE-OBS date format in dateobs to year-2000
*     standard form and derive mjdobs from it if not already set.
*     Alternatively, if mjdobs is set and dateobs isn't, then derive dateobs
*     from it.
*
*   - unitfix(): translate some commonly used but non-standard unit strings in
*     the CUNITia keyvalues, e.g. 'DEG' -> 'deg'.
*
*   - celfix(): translate AIPS-convention celestial projection types, NCP and
*     GLS, in ctype[] as set from CTYPEia.
*
*   - spcfix(): translate AIPS-convention spectral types, 'FREQ-LSR',
*     'FELO-HEL', etc., in ctype[] as set from CTYPEia.
*
*   - cylfix(): fixes WCS keyvalues for malformed cylindrical projections that
*     suffer from the problem described in Sect. 7.3.4 of Paper I.
*
*
* wcsfix() - Translate a non-standard WCS struct
* ----------------------------------------------
* wcsfix() applies all of the corrections handled separately by datfix(),
* unitfix(), celfix(), spcfix() and cylfix().
*
* Given:
*   ctrl      int       Do potentially unsafe translations of non-standard
*                       unit strings as described in the usage notes to
*                       wcsutrn().
*
*   naxis     const int []
*                       Image axis lengths.  If this array pointer is set to
*                       zero then cylfix() will not be invoked.
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.
*
* Returned:
*   stat      int [NWCSFIX]
*                       Status returns from each of the functions.  Use the
*                       preprocessor macros NWCSFIX to dimension this vector
*                       and CDFIX, DATFIX, UNITFIX, CELFIX, SPCFIX and CYLFIX
*                       to access its elements.  A status value of -2 is set
*                       for functions that were not invoked.
*
* Function return value:
*             int       Status return value:
*                         0: Success.
*                         1: One or more of the translation functions returned
*                            an error.
*
*
* cdfix() - Fix erroneously omitted CDi_ja keywords
* -------------------------------------------------
* cdfix() sets the diagonal element of the CDi_ja matrix to unity if all
* CDi_ja keywords associated with a given axis were omitted.  According to
* Paper I, if any CDi_ja keywords at all are given in a FITS header then those
* not given default to zero.  This results in a singular matrix with an
* intersecting row and column of zeros.
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.
*
* Function return value:
*             int       Status return value:
*                         -1: No change required (not an error).
*                          0: Success.
*                          1: Null wcsprm pointer passed.
*
*
* datfix() - Translate DATE-OBS and derive MJD-OBS or vice versa
* --------------------------------------------------------------
* datfix() translates the old DATE-OBS date format set in wcsprm::dateobs to
* year-2000 standard form (yyyy-mm-ddThh:mm:ss) and derives MJD-OBS from it if
* not already set.  Alternatively, if wcsprm::mjdobs is set and
* wcsprm::dateobs isn't, then datfix() derives wcsprm::dateobs from it.  If
* both are set but disagree by more than half a day then status 5 is returned.
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.  wcsprm::dateobs
*                       and/or wcsprm::mjdobs may be changed.
*
* Function return value:
*             int       Status return value:
*                         -1: No change required (not an error).
*                          0: Success.
*                          1: Null wcsprm pointer passed.
*                          5: Invalid parameter value.
*
* Notes:
*   The MJD algorithms used by datfix() are from D.A. Hatcher, 1984, QJRAS,
*   25, 53-55, as modified by P.T. Wallace for use in SLALIB subroutines CLDJ
*   and DJCL.
*
*
* unitfix() - Correct aberrant CUNITia keyvalues
* ----------------------------------------------
* unitfix() applies wcsutrn() to translate non-standard CUNITia keyvalues,
* e.g. 'DEG' -> 'deg', also stripping off unnecessary whitespace.
*
* Given:
*   ctrl      int       Do potentially unsafe translations described in the
*                       usage notes to wcsutrn().
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.
*
* Function return value:
*             int       Status return value:
*                         -1: No change required (not an error).
*                          0: Success.
*                          1: Null wcsprm pointer passed.
*
*
* celfix() - Translate AIPS-convention celestial projection types
* ---------------------------------------------------------------
* celfix() translates AIPS-convention celestial projection types, NCP and
* GLS, set in the ctype[] member of the wcsprm struct.
*
* Two additional pv[] keyvalues are created when translating NCP.  If the
* pv[] array was initially allocated by wcsini() then the array will be
* expanded if necessary.  Otherwise, error 2 will be returned if two empty
* slots are not already available for use.
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.  wcsprm::ctype[]
*                       and/or wcsprm::pv[] may be changed.
*
* Function return value:
*             int       Status return value:
*                         -1: No change required (not an error).
*                          0: Success.
*                          1: Null wcsprm pointer passed.
*                          2: Memory allocation failed.
*                          3: Linear transformation matrix is singular.
*                          4: Inconsistent or unrecognized coordinate axis
*                             types.
*                          5: Invalid parameter value.
*                          6: Invalid coordinate transformation parameters.
*                          7: Ill-conditioned coordinate transformation
*                             parameters.
*
*
* spcfix() - Translate AIPS-convention spectral types
* ---------------------------------------------------
* spcfix() translates AIPS-convention spectral coordinate types,
* '{FREQ,FELO,VELO}-{OBS,HEL,LSR}' (e.g. 'FREQ-LSR', 'FELO-HEL', 'VELO-OBS')
* set in wcsprm::ctype[].
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.  wcsprm::ctype[]
*                       and/or wcsprm::specsys may be changed.
*
* Function return value:
*             int      Status return value:
*                         -1: No change required (not an error).
*                          0: Success.
*                          1: Null wcsprm pointer passed.
*                          2: Memory allocation failed.
*                          3: Linear transformation matrix is singular.
*                          4: Inconsistent or unrecognized coordinate axis
*                             types.
*                          5: Invalid parameter value.
*                          6: Invalid coordinate transformation parameters.
*                          7: Ill-conditioned coordinate transformation
*                             parameters.
*
*
* cylfix() - Fix malformed cylindrical projections
* ------------------------------------------------
* cylfix() fixes WCS keyvalues for malformed cylindrical projections that
* suffer from the problem described in Sect. 7.3.4 of Paper I.
*
* Given:
*   naxis     const int []
*                       Image axis lengths.
*
* Given and returned:
*   wcs       struct wcsprm*
*                       Coordinate transformation parameters.
*
* Function return value:
*             int      Status return value:
*                        -1: No change required (not an error).
*                         0: Success.
*                         1: Null wcsprm pointer passed.
*                         2: Memory allocation failed.
*                         3: Linear transformation matrix is singular.
*                         4: Inconsistent or unrecognized coordinate axis
*                            types.
*                         5: Invalid parameter value.
*                         6: Invalid coordinate transformation parameters.
*                         7: Ill-conditioned coordinate transformation
*                            parameters.
*                         8: All of the corner pixel coordinates are
*                            invalid.
*                         9: Could not determine reference pixel coordinate.
*                        10: Could not determine reference pixel value.
*
*
* Global variable: const char *wcsfix_errmsg[] - Status return messages
* ---------------------------------------------------------------------
* Error messages to match the status value returned from each function.
*
*===========================================================================*/

#ifndef WCSLIB_WCSFIX
#define WCSLIB_WCSFIX

#include "wcs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CDFIX    0
#define DATFIX   1
#define UNITFIX  2
#define CELFIX   3
#define SPCFIX   4
#define CYLFIX   5
#define NWCSFIX  6

extern const char *wcsfix_errmsg[];
#define cylfix_errmsg wcsfix_errmsg


int wcsfix(int ctrl, const int naxis[], struct wcsprm *wcs, int stat[]);

int cdfix(struct wcsprm *wcs);

int datfix(struct wcsprm *wcs);

int unitfix(int ctrl, struct wcsprm *wcs);

int celfix(struct wcsprm *wcs);

int spcfix(struct wcsprm *wcs);

int cylfix(const int naxis[], struct wcsprm *wcs);


#ifdef __cplusplus
}
#endif

#endif /* WCSLIB_WCSFIX */
