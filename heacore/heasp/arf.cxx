// arf object code. Definitions in arf.h

#ifndef HAVE_arf
#include "arf.h"
#endif

#ifndef HAVE_SPio
#include "SPio.h"
#endif


// Class arf

// default constructor

arf::arf()
{
}


// Destructor

arf::~arf()
{
}

// reading from ARF file. 

Integer arf::read(string filename)
{
  return(this->read(filename, 1, 1));
}

// reading from ARF file. This option allows multiple extensions in the same file

Integer arf::read(string filename, Integer ARFnumber)
{
  return(this->read(filename, ARFnumber, 1));
}

// reading from ARF file. This option allows the reading of a single line in a type II file

Integer arf::read(string filename, Integer ARFnumber, Integer RowNumber)
{
  const string hduName("SPECRESP");
  string DefString;

  // Read in the SPECRESP extension number ARFnumber
  // and set up an object called arf with the contents

  const vector<string> hduKeys;
  const vector<string> primaryKey;


  std::auto_ptr<FITS> pInfile(0);
      
  try{
    pInfile.reset(new FITS(filename,Read,hduName,false,hduKeys,primaryKey,(int)ARFnumber));
  } catch(...) {
    return(NoSuchFile);
  }

  ExtHDU& arf = pInfile->extension(hduName);

  // read the standard keywords and store in the object

  DefString = "UNKNOWN";
  Version = SPreadKey(arf, "HDUVERS", RowNumber, DefString);
  if ( Version == "UNKNOWN" ) {
    Version = SPreadKey(arf, "HDUVERS1", RowNumber, DefString);
  }

  Telescope = SPreadKey(arf, "TELESCOP", RowNumber, DefString);
  
  Instrument = SPreadKey(arf, "INSTRUME", RowNumber, DefString);

  Detector = SPreadKey(arf, "DETNAM", RowNumber, DefString);

  Filter = SPreadKey(arf, "FILTER", RowNumber, DefString);

  DefString = "SPECRESP";
  ExtensionName = SPreadKey(arf, "EXTNAME", RowNumber, DefString);

  // Read the data

  SPreadCol(arf,"ENERG_LO",RowNumber,LowEnergy);
  if ( LowEnergy.size() == 0 ) return(NoEnergLo);
  SPreadCol(arf,"ENERG_HI",RowNumber,HighEnergy);
  if ( HighEnergy.size() == 0 ) return(NoEnergHi);
  SPreadCol(arf,"SPECRESP",RowNumber,EffArea);
  if ( EffArea.size() == 0 ) return(NoSpecresp);

  // Read the units
  
  SPreadColUnits(arf, "ENERG_LO", EnergyUnits);
  SPreadColUnits(arf, "SPECRESP", arfUnits);

  return(0); 

}

// Deep copy

arf& arf::operator=(const arf& beta)
{
  // Copy the scalars

  EnergyUnits = beta.EnergyUnits;
  arfUnits = beta.arfUnits;

  Version = beta.Version;
  Telescope = beta.Telescope;  
  Instrument = beta.Instrument;
  Detector = beta.Detector;  
  Filter = beta.Filter;
  ExtensionName = beta.ExtensionName;

  // now copy the arrays

  LowEnergy.resize(beta.LowEnergy.size());
  for (size_t i=0; i<beta.LowEnergy.size(); i++) LowEnergy[i] = beta.LowEnergy[i];
  HighEnergy.resize(beta.HighEnergy.size());
  for (size_t i=0; i<beta.HighEnergy.size(); i++) HighEnergy[i] = beta.HighEnergy[i];
  EffArea.resize(beta.EffArea.size());
  for (size_t i=0; i<beta.EffArea.size(); i++) EffArea[i] = beta.EffArea[i];

  return *this;

}

// Return number of energy bins

Integer arf::NumberEnergyBins()
{
  return LowEnergy.size();
}

// Display information about the arf - return as a string

string arf::disp()
{

  ostringstream outstr;

  outstr << "ARF information : " << endl;
  outstr << "   Number of bins       = " << NumberEnergyBins()<< endl;
  outstr << "   Version              = " << Version<< endl;
  outstr << "   Telescope            = " << Telescope<< endl;
  outstr << "   Instrument           = " << Instrument<< endl;  
  outstr << "   Detector             = " << Detector<< endl;  
  outstr << "   Filter               = " << Filter<< endl;  
  outstr << "   ExtensionName        = " << ExtensionName<< endl;

  if ( LowEnergy.size() > 1 ) outstr << "  LowEnergy array read" << endl;
  if ( HighEnergy.size() > 1 ) outstr << "  HighEnergy array read" << endl;
  if ( EffArea.size() > 1 ) outstr << "  EffArea array read" << endl;
  outstr << endl;

  return outstr.str();
}

// Clear information from the arf

void arf::clear()
{
  LowEnergy.clear();
  HighEnergy.clear();
  EffArea.clear();

  EnergyUnits = " ";
  arfUnits = " ";

  Version = " ";
  Telescope = " ";
  Instrument = " ";
  Detector = " ";
  Filter = " ";
  ExtensionName = " ";

  return;
}

// Check completeness and consistency of information in arf

string arf::check()
{
  ostringstream outstr;

  // check for any data

  if ( EffArea.size() == 0 ) outstr << "EffArea has no data" << endl;

  // check for consistency of array sizes

  if ( LowEnergy.size() != EffArea.size() ) {
    outstr << "LowEnergy has size (" << LowEnergy.size() << ") but EffArea has size ("
	 << EffArea.size() << ")" << endl;
  }

  if ( HighEnergy.size() != EffArea.size() ) {
    outstr << "HighEnergy has size (" << HighEnergy.size() << ") but EffArea has size ("
	 << EffArea.size() << ")" << endl;
  }

  return outstr.str();
}


// Write arf as type I file

Integer arf::write(string filename)
{
  string Blank = " ";

  vector<string> ttype;
  vector<string> tform;
  vector<string> tunit;

  // Create a new FITS file instance  

  std::auto_ptr<FITS> pFits(0);
      
  try {                
    pFits.reset( new FITS(filename,Write) );
  } catch (FITS::CantCreate) {
    return(CannotCreate);       
  }

  // set up the column descriptors

  ttype.push_back("ENERG_LO");
  tform.push_back("E");
  tunit.push_back(" ");

  ttype.push_back("ENERG_HI");
  tform.push_back("E");
  tunit.push_back(" ");

  ttype.push_back("EFFAREA");
  tform.push_back("E");
  tunit.push_back(" ");

  // Create the new extension

  Table* parf = pFits->addTable(ExtensionName,LowEnergy.size(),ttype,tform,tunit);
  Table& arf = *parf;

  // Write the standard keywords
  
  SPwriteKey(arf, "HDUCLASS", (string)"OGIP", Blank);
  SPwriteKey(arf, "HDUCLAS1", (string)"RESPONSE", Blank);
  SPwriteKey(arf, "HDUCLAS2", (string)"SPECRESP", Blank);
  SPwriteKey(arf, "HDUVERS", Version, "OGIP version number");
  SPwriteKey(arf, "TELESCOP", Telescope, Blank);
  SPwriteKey(arf, "INSTRUME", Instrument, Blank);
  SPwriteKey(arf, "DETNAM", Detector, Blank);
  SPwriteKey(arf, "FILTER", Filter, Blank);

  // Write the arrays

  SPwriteCol(arf, "ENERG_LO", LowEnergy);
  SPwriteCol(arf, "ENERG_HI", HighEnergy);
  SPwriteCol(arf, "EFFAREA", EffArea);

  // Write the units

  SPwriteColUnits(arf, "ENERG_LO", EnergyUnits);
  SPwriteColUnits(arf, "ENERG_HI", EnergyUnits);
  SPwriteColUnits(arf, "EFFAREA", arfUnits);

  return(0);
}

// Write arf as type I file copying keywords and extra extensions from another arf file

Integer arf::write(string filename, string copyfilename)
{
  return(this->write(filename, copyfilename, 1));
}

// Write arf as type I file copying keywords and extra extensions from another arf file. Use the HDUnumber instance of the arf extension in both files.

Integer arf::write(string filename, string copyfilename, Integer HDUnumber)
{
  Integer Status(0);

  Status = this->write(filename);

  if ( Status != 0 ) return(Status);

  Status = SPcopyKeys(copyfilename, filename, "SPECRESP", HDUnumber);

  if ( Status != 0 ) return(Status);

  Status = SPcopyHDUs(copyfilename, filename);

  return(Status);
}


// Sum to another arf

arf& arf::operator+=(const arf& a)
{
  // check that the ARF to be added is compatible

  if ( !checkCompatibility(a) ) return *this;

  // Sum effective areas

  for (size_t i=0; i<EffArea.size(); i++) EffArea[i] += a.EffArea[i];

  return *this;
}

// Check compatibility between arfs

bool arf::checkCompatibility(const arf& a)
{
  if ( LowEnergy.size() != a.LowEnergy.size() ) return false;
  for ( size_t i=0; i < (size_t)LowEnergy.size(); i++ ) {
    if ( LowEnergy[i] != a.LowEnergy[i] ) return false;
    if ( HighEnergy[i] != a.HighEnergy[i] ) return false;
  }

  if ( EnergyUnits != a.EnergyUnits ) return false;
  if ( arfUnits != a.arfUnits ) return false;

  return true;
}


arf operator+ (const arf& a, const arf& b)
{
  arf c(a);
  return c += b;
}

// return the number of spectra in a type II PHA extension

Integer NumberofARFs(string filename, Integer HDUnumber, Integer& Status)
{

  if ( Status != 0 ) return 0;

  const string hduName("SPECRESP");

  // Read in the SPECRESP extension number HDUnumber
  // and set up an object called arf with the contents

  const vector<string> hduKeys;
  const vector<string> primaryKey;

  auto_ptr<FITS> pInfile(0);

  try {
    pInfile.reset(new FITS(filename,Read,hduName,false,hduKeys,primaryKey,(int)HDUnumber));
  } catch(...) {
    Status = NoSuchFile;
    return 0;
  }

  ExtHDU& arf = pInfile->extension(hduName);

  return (Integer)arf.rows();

}



