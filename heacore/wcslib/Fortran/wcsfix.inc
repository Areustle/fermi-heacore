*=======================================================================
*
* WCSLIB 4.4 - an implementation of the FITS WCS standard.
* Copyright (C) 1995-2009, Mark Calabretta
*
* This file is part of WCSLIB.
*
* WCSLIB is free software: you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* WCSLIB is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with WCSLIB.  If not, see http://www.gnu.org/licenses.
*
* Correspondence concerning WCSLIB may be directed to:
*   Internet email: mcalabre@atnf.csiro.au
*   Postal address: Dr. Mark Calabretta
*                   Australia Telescope National Facility, CSIRO
*                   PO Box 76
*                   Epping NSW 1710
*                   AUSTRALIA
*
* Author: Mark Calabretta, Australia Telescope National Facility
* http://www.atnf.csiro.au/~mcalabre/index.html
* $Id: wcsfix.inc,v 1.2 2009/09/14 20:25:19 irby Exp $
*=======================================================================

*     Functions.
      INTEGER   CDFIX, CELFIX, CYLFIX, DATFIX, SPCFIX, UNITFIX, WCSFIX

*     Flag bits for the RELAX argument.
      INTEGER   WCSFIX_CD, WCSFIX_CEL, WCSFIX_CYL, WCSFIX_DAT,
     :          WCSFIX_NWCS, WCSFIX_SPC, WCSFIX_UNIT

      PARAMETER (WCSFIX_CD   = 1)
      PARAMETER (WCSFIX_DAT  = 2)
      PARAMETER (WCSFIX_UNIT = 3)
      PARAMETER (WCSFIX_CEL  = 4)
      PARAMETER (WCSFIX_SPC  = 5)
      PARAMETER (WCSFIX_CYL  = 6)
      PARAMETER (WCSFIX_NWCS = 6)

      CHARACTER WCSFIX_ERRMSG(0:10)*80
      DATA WCSFIX_ERRMSG /
     :  'Success',
     :  'Null wcsprm pointer passed',
     :  'Memory allocation failed',
     :  'Linear transformation matrix is singular',
     :  'Inconsistent or unrecognized coordinate axis types',
     :  'Invalid parameter value',
     :  'Invalid coordinate transformation parameters',
     :  'Ill-conditioned coordinate transformation parameters',
     :  'All of the corner pixel coordinates are invalid',
     :  'Could not determine reference pixel coordinate',
     :  'Could not determine reference pixel value'/
