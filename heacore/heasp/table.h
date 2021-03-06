// Class definition for table model object

#ifndef HAVE_HEASP
#include "heasp.h"
#endif

#define HAVE_table 1

// class definition for individual parameters within the table model

class tableParameter{
 public:

  string Name;                  // Parameter name
  int InterpolationMethod;      // 0==linear, 1==log
  Real InitialValue;            // Initial value for fit
  Real Delta;                   // Delta for fit
  Real Minimum;                 // Hard lower-limit (should correspond to first tabulated value)
  Real Bottom;                  // Soft lower-limit
  Real Top;                     // Soft upper-limit
  Real Maximum;                 // Hard upper-limit (should correspond to last tabulated value)
  vector<Real> TabulatedValues; // Tabulated parameter values

  //constructor

  tableParameter();

  // destructor

  ~tableParameter();

  // display information about the table parameter - return as a string

  string disp();

  // clear contents of the table parameter (mainly useful for Python)

  void clear();

};

// class definition for individual spectra (and additional spectra) within the table model

class tableSpectrum{
 public:

  vector<Real> Flux;
  vector<Real> ParameterValues;
  vector<vector<Real> > addFlux;

  //constructor

  tableSpectrum();

  // destructor

  ~tableSpectrum();

  // push an additional parameter spectrum

  void pushaddFlux(vector<Real>);

  // get an additional parameter spectrum

  vector<Real> getaddFlux(Integer Number);

  // display information about the table spectrum - return as a string

  string disp();

  // clear contents of the table spectrum (mainly useful for Python)

  void clear();

};

// class definition for table

class table{
 public:

  vector<tableParameter> Parameters;
  vector<tableSpectrum> Spectra;
  string ModelName;
  string ModelUnits;
  int NumIntParams;
  int NumAddParams;
  bool isError;
  bool isRedshift;
  bool isAdditive;
  vector<Real> Energies;

  // constructor

  table();

  // destructor

  ~table();

  // Push table Parameter object

  void pushParameter(const tableParameter& paramObject);

  // Push spectrum Parameter object

  void pushSpectrum(const tableSpectrum& spectrumObject);

  // Get table Parameter object (counts from zero)

  tableParameter getParameter(Integer number);

  // get table Spectrum object (counts from zero)

  tableSpectrum getSpectrum(Integer number);

  // display information about the table - return as a string

  string disp();

  // clear contents of table object (mainly useful for Python)

  void clear();

  // check for completeness and consistency of information in the table
  // if there is a problem then return diagnostic in string

  string check();

  // write to a FITS file

  Integer write(string filename);

};
