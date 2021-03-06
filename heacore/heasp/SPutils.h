// Useful functions - mostly using CCfits

#ifndef HAVE_HEASP
#include "heasp.h"
#endif

#ifndef HAVE_SPio
#include "SPio.h"
#endif

#define HAVE_SPutils 1

// Read the units associated with a column

void SPreadColUnits(ExtHDU&, string, string&);

// Write the units associated with a column

void SPwriteColUnits(Table&, string, string);

// returns the tform string for the longest string in the input vector.

string SPstringTform(const vector<string>& Data);

// copy from infile to outfile all HDUs which are not manipulated by this library 

Integer SPcopyHDUs(string infile, string outfile);

// copy non-critical keywords from infile to outfile for the HDUnumber instance
// of the HDUname HDU. (this is a wrap-up of the heautils routine HDcopy_keys)

Integer SPcopyKeys(string infile, string outfile, string HDUname, Integer HDUnumber);
