// C wrappers for heasp C++ code to reproduce routines available in the original
// version of heasp.

#include "Cheasp.h"
#include "rmf.h"
#include "rmft.h"
#include "pha.h"
#include "phaII.h"
#include "SPio.h"

// prototypes for conversion routines which we will need

void RMFstructToObject(struct RMF *rmfstruct, rmf& response, Integer mode);
void RMFobjectToStruct(const rmf& response, struct RMF *rmfstruct, Integer mode);

void RMFTstructToObject(struct RMFchan *rmfstruct, rmft& response);
void RMFTobjectToStruct(const rmft& response, struct RMFchan *rmfstruct);

void ARFstructToObject(struct ARF *arfstruct, arf& ea);
void ARFobjectToStruct(const arf& ea, struct ARF *arfstruct);

void PHAstructToObject(struct PHA *phastruct, pha& phaobject);
void PHAobjectToStruct(const pha& phaobject, struct PHA *phastruct);

// *******************************************************************************************
// C wrapper routines. Prototypes in Cheasp.h.

// read the RMF matrix from an open FITS file - if there are multiple RMF extensions then
// read the one in RMFnumber

int ReadRMFMatrix(char *filename, long RMFnumber, struct RMF *rmfstruct)
{
  Integer Status(0);

  rmf inRMF;
  Status = inRMF.readMatrix((string)filename, (Integer)RMFnumber);

  if ( Status == 0 ) {
    RMFobjectToStruct(inRMF, rmfstruct, 1);
    return 0;
  } else {
    return (int)Status;
  }

}

// write the RMF matrix to an opened FITS file

int WriteRMFMatrix(char *filename, struct RMF *rmfstruct)
{

  Integer Status(0);

  rmf outRMF;
  RMFstructToObject(rmfstruct, outRMF, 1);

  Status = outRMF.writeMatrix((string)filename);

  return (int)Status;

}

// read the RMF ebounds from an open FITS file - if there are multiple EBOUNDS extensions then
// read the one in EBDnumber

int ReadRMFEbounds(char *filename, long EBDnumber, struct RMF *rmfstruct)
{
  Integer Status(0);

  rmf inRMF;
  Status = inRMF.readChannelBounds((string)filename, (Integer)EBDnumber);

  if ( Status == 0 ) {
    RMFobjectToStruct(inRMF, rmfstruct, 2);
    return 0;
  } else {
    return (int)Status;
  }
}

// write the RMF ebounds to an opened FITS file

int WriteRMFEbounds(char *filename, struct RMF *rmfstruct)
{
  Integer Status(0);

  rmf outRMF;
  RMFstructToObject(rmfstruct, outRMF, 2);

  Status = outRMF.writeChannelBounds((string)filename);

  return (int)Status;
}

// write information about RMF to stdout

void DisplayRMF(struct RMF *rmfstruct)
{
  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);
  cout << inRMF.disp();

  return;
}

// return the channel for a photon of the given input energy - draws random
// numbers to return NumberPhotons entries in the channel array. Note that this
// routine assumes that memory has been allocated for the channel array.

void ReturnChannel(struct RMF *rmfstruct, float energy, int NumberPhotons, long *channel)
{
  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  vector<Integer> channelVector((Integer)NumberPhotons);

  channelVector = inRMF.RandomChannels((Real)energy,(Integer)NumberPhotons);

  for (size_t i=0; i<(size_t)NumberPhotons; i++) channel[i] = channelVector[i];
  
  return;
}
 
// normalize the response to unity in each energy 

void NormalizeRMF(struct RMF *rmfstruct)
{
  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  inRMF.normalize();

  RMFobjectToStruct(inRMF, rmfstruct, 0);

  return;
}

// compress the response to remove all elements below the threshold value

void CompressRMF(struct RMF *rmfstruct, float threshold)
{
  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  inRMF.compress((Real)threshold);

  RMFobjectToStruct(inRMF, rmfstruct, 0);

  return;
}

int RebinRMFChannel(struct RMF *rmfstruct, struct BinFactors *bin)
{
  Integer Status(0);

  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  vector<Integer> StartBin(bin->NumberBinFactors);
  vector<Integer> EndBin(bin->NumberBinFactors);
  vector<Integer> BinFactor(bin->NumberBinFactors);

  for (size_t i=0; i<StartBin.size(); i++) {
    StartBin[i] = bin->StartBin[i];
    EndBin[i] = bin->EndBin[i];
    BinFactor[i] = bin->Binning[i];
  }

  grouping inGrouping;
  Status = inGrouping.load(StartBin, EndBin, BinFactor, inRMF.NumberChannels(), inRMF.FirstChannel);
  if ( Status != 0 ) return (int)Status;

  Status = inRMF.rebinChannels(inGrouping);
  if ( Status != 0 ) return (int)Status;

  RMFobjectToStruct(inRMF, rmfstruct, 0);

  return (int)Status;
}

int RebinRMFEnergy(struct RMF *rmfstruct, struct BinFactors *bin)
{
  Integer Status(0);

  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  vector<Integer> StartBin(bin->NumberBinFactors);
  vector<Integer> EndBin(bin->NumberBinFactors);
  vector<Integer> BinFactor(bin->NumberBinFactors);

  for (size_t i=0; i<StartBin.size(); i++) {
    StartBin[i] = bin->StartBin[i];
    EndBin[i] = bin->EndBin[i];
    BinFactor[i] = bin->Binning[i];
  }

  grouping inGrouping;
  Status = inGrouping.load(StartBin, EndBin, BinFactor, inRMF.NumberEnergyBins(), 0);
  if ( Status != 0 ) return (int)Status;

  Status = inRMF.rebinEnergies(inGrouping);
  if ( Status != 0 ) return (int)Status;

  RMFobjectToStruct(inRMF, rmfstruct, 0);

  return (int)Status;
}

// transpose the response

void TransposeRMF(struct RMF *rmfstruct, struct RMFchan *rmfchan)
{
  rmf inRMF;
  rmft outRMFT;

  RMFstructToObject(rmfstruct, inRMF, 0);
  outRMFT.load(inRMF);
  RMFTobjectToStruct(outRMFT, rmfchan);

  return;
}

// return a single value from the matrix

float ReturnRMFElement(struct RMF *rmfstruct, long channel, long energybin)
{
  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  return (float)inRMF.ElementValue((Integer)channel, (Integer)energybin);
}

// return a single value from the transposed matrix

float ReturnRMFchanElement(struct RMFchan *rmfchan, long channel, long energybin)
{
  rmft inRMFT;
  RMFTstructToObject(rmfchan, inRMFT);

  return (float)inRMFT.ElementValue((Integer)channel, (Integer)energybin);
}

// add rmf2 onto rmf1

int AddRMF(struct RMF *rmf1, struct RMF *rmf2)
{
  rmf inRMF1, inRMF2;
  RMFstructToObject(rmf1, inRMF1, 0);
  RMFstructToObject(rmf2, inRMF2, 0);

  if ( !inRMF1.checkCompatibility(inRMF2) ) return 1;

  inRMF1 += inRMF2;

  RMFobjectToStruct(inRMF1, rmf1, 0);

  return 0;
}

// read the effective areas from a FITS file - if there are multiple SPECRESP extensions then
// read the one in ARFFnumber

int ReadARF(char *filename, long ARFnumber, struct ARF *arfstruct)
{
  Integer Status(0);

  arf ea;
  Status = ea.read((string)filename, (Integer)ARFnumber);

  if ( Status == 0 ) {
    ARFobjectToStruct(ea, arfstruct);
    return 0;
  } else {
    return (int)Status;
  }
}

// write the ARF to an opened FITS file

int WriteARF(char *filename, struct ARF *arfstruct)
{
  Integer Status(0);

  arf ea;
  ARFstructToObject(arfstruct, ea);

  Status = ea.write((string)filename);

  return (int)Status;
}

// add arf2 onto arf1 //

int AddARF(struct ARF *arf1, struct ARF *arf2)
{
  arf ea1, ea2;
  ARFstructToObject(arf1, ea1);
  ARFstructToObject(arf2, ea2);

  if ( !ea1.checkCompatibility(ea2) ) return 1;

  ea1 += ea2;

  ARFobjectToStruct(ea1, arf1);

  return 0;
}

// write information about ARF to stdout //

void DisplayARF(struct ARF *arfstruct)
{
  arf ea;
  ARFstructToObject(arfstruct, ea);
  cout << ea.disp();

  return;
}

// multiply the ARF into the RMF

long MergeARFRMF(struct ARF *arfstruct, struct RMF *rmfstruct){

  // convert input ARF structure into ARF object

  arf inARF;
  ARFstructToObject(arfstruct, inARF);

  // convert input RMF structure into RMF object

  rmf inRMF;
  RMFstructToObject(rmfstruct, inRMF, 0);

  // Check for compatibility

  if ( !inRMF.checkCompatibility(inARF) ) return(-1);

  // Merge ARF and RMF

  inRMF *= inARF;

  // convert RMF object into output RMF structure

  RMFobjectToStruct(inRMF, rmfstruct, 0);

  return(0); 

}

// read the type I pha extension from a FITS file - if there are multiple pha 
// extensions then read the one in phanumber

int ReadPHAtypeI(char *filename, long PHAnumber, struct PHA *phastruct)
{
  Integer Status(0);

  pha phaobject;
  Status = phaobject.read((string)filename, (Integer)PHAnumber);

  if ( Status == 0 ) {
    PHAobjectToStruct(phaobject, phastruct);
    return 0;
  } else {
    return (int)Status;
  }
}
 
// read the type II PHA extension from a FITS file - if there are multiple PHA
// extensions then read the one in PHAnumber - within the typeII extension reads the 
// spectra listed in the SpectrumNumber vector. Note that this assumes that the output
// pha array of structs has been set up with the correct size.

int ReadPHAtypeII(char *filename, long PHAnumber, long NumberSpectra, long *SpectrumNumber, struct PHA **phastructs)
{
  Integer Status(0);
  vector<Integer> SpNum(NumberSpectra);
  for (size_t i=0; i<SpNum.size(); i++) SpNum[i] = SpectrumNumber[i];

  phaII sp;
  Status = sp.read((string)filename, (Integer)PHAnumber, SpNum);

  if ( Status == 0 ) {
    for (size_t i=0; i<SpNum.size(); i++) PHAobjectToStruct(sp.phas[i], phastructs[i]);
    return 0;
  } else {
    return (int)Status;
  }
}

// write the type I PHA extension to a FITS file

int WritePHAtypeI(char *filename, struct PHA *phastruct)
{
  Integer Status(0);

  pha phaobject;
  PHAstructToObject(phastruct, phaobject);

  Status = phaobject.write((string)filename);

  return (int)Status;
}

// write the type II PHA extension to a FITS file

int WritePHAtypeII(char *filename, long NumberSpectra, struct PHA **phastructs)
{
  Integer Status(0);

  phaII sp;
  for (size_t i=0; i<(size_t)NumberSpectra; i++) PHAstructToObject(phastructs[i], sp.phas[i]);

  Status = sp.write((string)filename);

  return (int)Status;
}

// return the type of a PHA extension. Return -1 if can't be found.

int ReturnPHAtype(char *filename, long PHAnumber)
{
  Integer Status(0);
  Integer type = PHAtype((string)filename, (Integer)PHAnumber, Status);
  if ( Status == 0 ) {
    return (int)type;
  } else {
    return -1;
  }
}

/* write information about spectrum to stdout */

void DisplayPHAtypeI(struct PHA *phastruct)
{
  pha phaobject;
  PHAstructToObject(phastruct, phaobject);

  cout << phaobject.disp();

  return;
}

/* write information about spectra to stdout */

void DisplayPHAtypeII(long NumberSpectra, struct PHA **phastructs)
{
  pha phaobject;
  for (long i=0; i<NumberSpectra; i++) {
    PHAstructToObject(phastructs[i], phaobject);
    cout << "Spectrum # " << i+1 << endl;
    cout << phaobject.disp();
  }

  return;
}

/* Rebin spectrum */

int RebinPHA(struct PHA *phastruct, struct BinFactors *bin)
{
  Integer Status(0);

  pha phaobject;
  PHAstructToObject(phastruct, phaobject);

  vector<Integer> StartBin(bin->NumberBinFactors);
  vector<Integer> EndBin(bin->NumberBinFactors);
  vector<Integer> BinFactor(bin->NumberBinFactors);

  for (size_t i=0; i<StartBin.size(); i++) {
    StartBin[i] = bin->StartBin[i];
    EndBin[i] = bin->EndBin[i];
    BinFactor[i] = bin->Binning[i];
  }

  grouping inGrouping;
  Status = inGrouping.load(StartBin, EndBin, BinFactor, phaobject.NumberChannels(), phaobject.FirstChannel);
  if ( Status != 0 ) return (int)Status;

  Status = phaobject.rebinChannels(inGrouping);
  if ( Status != 0 ) return (int)Status;

  PHAobjectToStruct(phaobject, phastruct);

  return (int)Status;

}

// return 0 if COUNTS column exists and is integer or COUNTS column does not exist

int CheckPHAcounts(char *filename, long PHAnumber)
{
  Integer Status(0);
  bool isCounts = IsPHAcounts((string)filename, (Integer)PHAnumber, Status);
  if ( Status == 0 ) {
    if ( isCounts) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}

// return the number of spectra in a type II PHA extension

long ReturnNumberofSpectra(char *filename, long PHAnumber)
{
  Integer Status(0);
  Integer nSpectra = NumberofSpectra((string)filename, (Integer)PHAnumber, Status);
  if ( Status == 0 ) {
    return (long)nSpectra;
  } else {
    return -1;
  }
}

// Set up a grouping array using the BinFactors structure

int SPSetGroupArray(int inputSize, struct BinFactors *binning, int *groupArray)
{

  vector<Integer> StartBin(binning->NumberBinFactors);
  vector<Integer> EndBin(binning->NumberBinFactors);
  vector<Integer> BinFactor(binning->NumberBinFactors);

  for (size_t i=0; i<StartBin.size(); i++) {
    StartBin[i] = binning->StartBin[i];
    EndBin[i] = binning->EndBin[i];
    BinFactor[i] = binning->Binning[i];
  }

  grouping groupInfo;
  Integer Status(0);
  Status = groupInfo.load(StartBin, EndBin, BinFactor, (Integer)inputSize, 1);

  if ( Status == 0 ) {
    for (size_t i=0; i<groupInfo.flag.size(); i++) groupArray[i] = (int)groupInfo.flag[i];
  }

  return (int)Status; 

}



// Bin an array using the information in the grouping array. Assumes that the output array
// has been malloc'ed correctly.

int SPBinArray(int inputSize, float *input, int *groupArray, int mode, float *output)
{
  vector<Real> inArray(inputSize);
  vector<Real> outArray;
  grouping groupInfo;
  Integer binMode;

  for (size_t i=0; i<inArray.size(); i++) inArray[i] = (Real)input[i];
  groupInfo.flag.resize(inputSize);
  for (size_t i=0; i<groupInfo.flag.size(); i++) groupInfo.flag[i] = (Integer)groupArray[i];

  if ( mode == 1 ) {
    binMode = 1;
  } else {
    binMode = mode + 1;
  }

  GroupBin(inArray, binMode, groupInfo, outArray);

  for (size_t i=0; i<outArray.size(); i++) output[i] = (float)outArray[i];

  return 0;
}

// Read an ascii file with binning factors and load the binning array

int SPReadBinningFile(char *filename, struct BinFactors *binning)
{

  Integer Status(0);
  vector<Integer> StartBin, EndBin, BinFactor;

  Status = ReadBinFactors((string)filename, StartBin, EndBin, BinFactor);

  if ( Status != 0 ) return (int)Status;

  binning->NumberBinFactors = StartBin.size();
  binning->StartBin = (long *) malloc(binning->NumberBinFactors*sizeof(long));
  binning->EndBin   = (long *) malloc(binning->NumberBinFactors*sizeof(long));
  binning->Binning  = (long *) malloc(binning->NumberBinFactors*sizeof(long));

  for (size_t i=0; i<StartBin.size(); i++) {
    binning->StartBin[i] = StartBin[i];
    binning->EndBin[i]   = EndBin[i];
    binning->Binning[i]  = BinFactor[i];
  }

  return 0;

}


// *******************************************************************************************
// conversion routines

// The RMF is a bit complicated because we have to make sure that when reading
// the matrix/ebounds extensions we don't overwrite the ebounds/matrix.
// mode = 0 means do both, 1 = matrix, 2 = ebounds

void RMFstructToObject(struct RMF *rmfstruct, rmf& output, Integer mode)
{

  if ( mode != 2) output.FirstChannel = rmfstruct->FirstChannel;
  if ( mode != 2) output.AreaScaling = rmfstruct->AreaScaling;
  if ( mode != 2) output.ResponseThreshold = rmfstruct->ResponseThreshold;
  output.ChannelType = rmfstruct->ChannelType;
  if ( mode != 2 ) output.RMFVersion = rmfstruct->RMFVersion;
  if ( mode != 1 ) output.EBDVersion = rmfstruct->EBDVersion;
  output.Telescope = rmfstruct->Telescope;
  output.Instrument = rmfstruct->Instrument;
  output.Detector = rmfstruct->Detector;
  output.Filter = rmfstruct->Filter;
  if ( mode != 2) output.RMFType = rmfstruct->RMFType;
  if ( mode != 2) output.RMFExtensionName = rmfstruct->RMFExtensionName;
  if ( mode != 1) output.EBDExtensionName = rmfstruct->EBDExtensionName;

  output.EnergyUnits = rmfstruct->EnergyUnits;
  if ( mode != 2) output.RMFUnits = rmfstruct->RMFUnits;

  if ( mode != 2) {
   output.NumberGroups.resize(rmfstruct->NumberEnergyBins);
   for (size_t i=0; i<output.NumberGroups.size(); i++) output.NumberGroups[i] = rmfstruct->NumberGroups[i];
   output.FirstGroup.resize(rmfstruct->NumberEnergyBins);
   for (size_t i=0; i<output.FirstGroup.size(); i++) output.FirstGroup[i] = rmfstruct->FirstGroup[i];
  

   output.FirstChannelGroup.resize(rmfstruct->NumberTotalGroups);
   for (size_t i=0; i<output.FirstChannelGroup.size(); i++) output.FirstChannelGroup[i] = rmfstruct->FirstChannelGroup[i];
   output.NumberChannelsGroup.resize(rmfstruct->NumberTotalGroups);
   for (size_t i=0; i<output.NumberChannelsGroup.size(); i++) output.NumberChannelsGroup[i] = rmfstruct->NumberChannelGroups[i];
   output.FirstElement.resize(rmfstruct->NumberTotalGroups);
   for (size_t i=0; i<output.FirstElement.size(); i++) output.FirstElement[i] = rmfstruct->FirstElement[i];

   if ( rmfstruct->isOrder ) {
     output.OrderGroup.resize(rmfstruct->NumberTotalGroups);
     for (size_t i=0; i<output.OrderGroup.size(); i++) output.OrderGroup[i] = rmfstruct->OrderGroup[i];
   } else {
     output.OrderGroup.resize(0);
   }

   output.LowEnergy.resize(rmfstruct->NumberEnergyBins);
   for (size_t i=0; i<output.LowEnergy.size(); i++) output.LowEnergy[i] = rmfstruct->LowEnergy[i];
   output.HighEnergy.resize(rmfstruct->NumberEnergyBins);
   for (size_t i=0; i<output.HighEnergy.size(); i++) output.HighEnergy[i] = rmfstruct->HighEnergy[i];
  

   output.Matrix.resize(rmfstruct->NumberTotalElements);
   for (size_t i=0; i<output.Matrix.size(); i++) output.Matrix[i] = rmfstruct->Matrix[i];
  }

  if ( mode != 1 ) {
    output.ChannelLowEnergy.resize(rmfstruct->NumberChannels);
    for (size_t i=0; i<output.ChannelLowEnergy.size(); i++) output.ChannelLowEnergy[i] = rmfstruct->ChannelLowEnergy[i];
    output.ChannelHighEnergy.resize(rmfstruct->NumberChannels);
    for (size_t i=0; i<output.ChannelHighEnergy.size(); i++) output.ChannelHighEnergy[i] = rmfstruct->ChannelHighEnergy[i];
  }
 
  return;

}


void RMFobjectToStruct(const rmf& input, struct RMF *rmfstruct, Integer mode)
{

  if ( mode != 2 ) rmfstruct->FirstChannel = input.FirstChannel;
  if ( mode != 2 ) rmfstruct->AreaScaling = input.AreaScaling;
  if ( mode != 2 ) rmfstruct->ResponseThreshold = input.ResponseThreshold;
  for (size_t i=0; i<=input.ChannelType.size(); i++) rmfstruct->ChannelType[i] = input.ChannelType.c_str()[i];
  if ( mode != 2 ) for (size_t i=0; i<=input.RMFVersion.size(); i++) rmfstruct->RMFVersion[i] = input.RMFVersion.c_str()[i];
  if ( mode != 1 ) for (size_t i=0; i<=input.EBDVersion.size(); i++) rmfstruct->EBDVersion[i] = input.EBDVersion.c_str()[i];
  for (size_t i=0; i<=input.Telescope.size(); i++) rmfstruct->Telescope[i] = input.Telescope.c_str()[i];
  for (size_t i=0; i<=input.Instrument.size(); i++) rmfstruct->Instrument[i] = input.Instrument.c_str()[i];
  for (size_t i=0; i<=input.Detector.size(); i++) rmfstruct->Detector[i] = input.Detector.c_str()[i];
  for (size_t i=0; i<=input.Filter.size(); i++) rmfstruct->Filter[i] = input.Filter.c_str()[i];
  if ( mode != 2 ) for (size_t i=0; i<=input.RMFType.size(); i++) rmfstruct->RMFType[i] = input.RMFType.c_str()[i];
  if ( mode != 2 ) for (size_t i=0; i<=input.RMFExtensionName.size(); i++) rmfstruct->RMFExtensionName[i] = input.RMFExtensionName.c_str()[i];
  if ( mode != 1 ) for (size_t i=0; i<=input.EBDExtensionName.size(); i++) rmfstruct->EBDExtensionName[i] = input.EBDExtensionName.c_str()[i];

  for (size_t i=0; i<=input.EnergyUnits.size(); i++) rmfstruct->EnergyUnits[i] = input.EnergyUnits.c_str()[i];
  if ( mode != 2 ) for (size_t i=0; i<=input.RMFUnits.size(); i++) rmfstruct->RMFUnits[i] = input.RMFUnits.c_str()[i];

  if ( mode != 1 ) rmfstruct->NumberChannels = input.ChannelLowEnergy.size();
  if ( mode != 2 ) rmfstruct->NumberEnergyBins = input.NumberGroups.size();
  if ( mode != 2 ) rmfstruct->NumberTotalGroups = input.FirstChannelGroup.size();
  if ( mode != 2 ) rmfstruct->NumberTotalElements = input.Matrix.size();

  if ( mode != 2 ) {
    rmfstruct->NumberGroups = (long *) malloc(rmfstruct->NumberEnergyBins*sizeof(long));
    for (size_t i=0; i<(size_t)rmfstruct->NumberEnergyBins; i++) rmfstruct->NumberGroups[i] = input.NumberGroups[i];
    rmfstruct->FirstGroup = (long *) malloc(rmfstruct->NumberEnergyBins*sizeof(long));
    for (size_t i=0; i<(size_t)rmfstruct->NumberEnergyBins; i++) rmfstruct->FirstGroup[i] = input.FirstGroup[i];
  

    rmfstruct->FirstChannelGroup = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
    for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->FirstChannelGroup[i] = input.FirstChannelGroup[i];
    rmfstruct->NumberChannelGroups = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
    for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->NumberChannelGroups[i] = input.NumberChannelsGroup[i];
    rmfstruct->FirstElement = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
    for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->FirstElement[i] = input.FirstElement[i];

    if ( input.OrderGroup.size() > 0 ) {
      rmfstruct->isOrder = 1;
      rmfstruct->OrderGroup = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
      for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->OrderGroup[i] = input.OrderGroup[i];
    } else {
      rmfstruct->isOrder = 0;
    }

    rmfstruct->LowEnergy = (float *) malloc(rmfstruct->NumberEnergyBins*sizeof(float));
    for (size_t i=0; i<(size_t)rmfstruct->NumberEnergyBins; i++) rmfstruct->LowEnergy[i] = input.LowEnergy[i];
    rmfstruct->HighEnergy = (float *) malloc(rmfstruct->NumberEnergyBins*sizeof(float));
    for (size_t i=0; i<(size_t)rmfstruct->NumberEnergyBins; i++) rmfstruct->HighEnergy[i] = input.HighEnergy[i];

    rmfstruct->Matrix = (float *) malloc(rmfstruct->NumberTotalElements*sizeof(float));
    for (size_t i=0; i<(size_t)rmfstruct->NumberTotalElements; i++) rmfstruct->Matrix[i] = input.Matrix[i];
  }

  if ( mode != 1 ) {
    rmfstruct->ChannelLowEnergy = (float *) malloc(rmfstruct->NumberChannels*sizeof(float));
    for (size_t i=0; i<(size_t)rmfstruct->NumberChannels; i++) rmfstruct->ChannelLowEnergy[i] = input.ChannelLowEnergy[i];
    rmfstruct->ChannelHighEnergy = (float *) malloc(rmfstruct->NumberChannels*sizeof(float));
    for (size_t i=0; i<(size_t)rmfstruct->NumberChannels; i++) rmfstruct->ChannelHighEnergy[i] = input.ChannelHighEnergy[i];
  }

  return;

}

void RMFTstructToObject(struct RMFchan *rmfstruct, rmft& output)
{

  output.FirstChannel = rmfstruct->FirstChannel;
  output.AreaScaling = rmfstruct->AreaScaling;
  output.ResponseThreshold = rmfstruct->ResponseThreshold;
  output.ChannelType = rmfstruct->ChannelType;
  output.RMFVersion = rmfstruct->RMFVersion;
  output.EBDVersion = rmfstruct->EBDVersion;
  output.Telescope = rmfstruct->Telescope;
  output.Instrument = rmfstruct->Instrument;
  output.Detector = rmfstruct->Detector;
  output.Filter = rmfstruct->Filter;
  output.RMFType = rmfstruct->RMFType;
  output.RMFExtensionName = rmfstruct->RMFExtensionName;
  output.EBDExtensionName = rmfstruct->EBDExtensionName;

  output.EnergyUnits = rmfstruct->EnergyUnits;
  output.RMFUnits = rmfstruct->RMFUnits;

  output.NumberGroups.resize(rmfstruct->NumberChannels);
  for (size_t i=0; i<output.NumberGroups.size(); i++) output.NumberGroups[i] = rmfstruct->NumberGroups[i];
  output.FirstGroup.resize(rmfstruct->NumberChannels);
  for (size_t i=0; i<output.FirstGroup.size(); i++) output.FirstGroup[i] = rmfstruct->FirstGroup[i];
  

  output.FirstEnergyGroup.resize(rmfstruct->NumberTotalGroups);
  for (size_t i=0; i<output.FirstEnergyGroup.size(); i++) output.FirstEnergyGroup[i] = rmfstruct->FirstEnergyGroup[i];
  output.NumberEnergiesGroup.resize(rmfstruct->NumberTotalGroups);
  for (size_t i=0; i<output.NumberEnergiesGroup.size(); i++) output.NumberEnergiesGroup[i] = rmfstruct->NumberEnergyGroups[i];
  output.FirstElement.resize(rmfstruct->NumberTotalGroups);
  for (size_t i=0; i<output.FirstElement.size(); i++) output.FirstElement[i] = rmfstruct->FirstElement[i];
  output.OrderGroup.resize(rmfstruct->NumberTotalGroups);
  for (size_t i=0; i<output.OrderGroup.size(); i++) output.OrderGroup[i] = rmfstruct->OrderGroup[i];
  

  output.LowEnergy.resize(rmfstruct->NumberEnergyBins);
  for (size_t i=0; i<output.LowEnergy.size(); i++) output.LowEnergy[i] = rmfstruct->LowEnergy[i];
  output.HighEnergy.resize(rmfstruct->NumberEnergyBins);
  for (size_t i=0; i<output.HighEnergy.size(); i++) output.HighEnergy[i] = rmfstruct->HighEnergy[i];
  

  output.Matrix.resize(rmfstruct->NumberTotalElements);
  for (size_t i=0; i<output.Matrix.size(); i++) output.Matrix[i] = rmfstruct->Matrix[i];
  

  output.ChannelLowEnergy.resize(rmfstruct->NumberChannels);
  for (size_t i=0; i<output.ChannelLowEnergy.size(); i++) output.ChannelLowEnergy[i] = rmfstruct->ChannelLowEnergy[i];
  output.ChannelHighEnergy.resize(rmfstruct->NumberChannels);
  for (size_t i=0; i<output.ChannelHighEnergy.size(); i++) output.ChannelHighEnergy[i] = rmfstruct->ChannelHighEnergy[i];

  return;

}


void RMFTobjectToStruct(const rmft& input, struct RMFchan *rmfstruct)
{

  rmfstruct->FirstChannel = input.FirstChannel;
  rmfstruct->AreaScaling = input.AreaScaling;
  rmfstruct->ResponseThreshold = input.ResponseThreshold;
  for (size_t i=0; i<=input.ChannelType.size(); i++) rmfstruct->ChannelType[i] = input.ChannelType.c_str()[i];
  for (size_t i=0; i<=input.RMFVersion.size(); i++) rmfstruct->RMFVersion[i] = input.RMFVersion.c_str()[i];
  for (size_t i=0; i<=input.EBDVersion.size(); i++) rmfstruct->EBDVersion[i] = input.EBDVersion.c_str()[i];
  for (size_t i=0; i<=input.Telescope.size(); i++) rmfstruct->Telescope[i] = input.Telescope.c_str()[i];
  for (size_t i=0; i<=input.Instrument.size(); i++) rmfstruct->Instrument[i] = input.Instrument.c_str()[i];
  for (size_t i=0; i<=input.Detector.size(); i++) rmfstruct->Detector[i] = input.Detector.c_str()[i];
  for (size_t i=0; i<=input.Filter.size(); i++) rmfstruct->Filter[i] = input.Filter.c_str()[i];
  for (size_t i=0; i<=input.RMFType.size(); i++) rmfstruct->RMFType[i] = input.RMFType.c_str()[i];
  for (size_t i=0; i<=input.RMFExtensionName.size(); i++) rmfstruct->RMFExtensionName[i] = input.RMFExtensionName.c_str()[i];
  for (size_t i=0; i<=input.EBDExtensionName.size(); i++) rmfstruct->EBDExtensionName[i] = input.EBDExtensionName.c_str()[i];

  for (size_t i=0; i<=input.EnergyUnits.size(); i++) rmfstruct->EnergyUnits[i] = input.EnergyUnits.c_str()[i];
  for (size_t i=0; i<=input.RMFUnits.size(); i++) rmfstruct->RMFUnits[i] = input.RMFUnits.c_str()[i];

  rmfstruct->NumberChannels = input.ChannelLowEnergy.size();
  rmfstruct->NumberEnergyBins = input.NumberGroups.size();
  rmfstruct->NumberTotalGroups = input.FirstEnergyGroup.size();
  rmfstruct->NumberTotalElements = input.Matrix.size();

  rmfstruct->NumberGroups = (long *) malloc(rmfstruct->NumberChannels*sizeof(long));
  for (size_t i=0; i<(size_t)rmfstruct->NumberChannels; i++) rmfstruct->NumberGroups[i] = input.NumberGroups[i];
  rmfstruct->FirstGroup = (long *) malloc(rmfstruct->NumberChannels*sizeof(long));
  for (size_t i=0; i<(size_t)rmfstruct->NumberChannels; i++) rmfstruct->FirstGroup[i] = input.FirstGroup[i];
  

  rmfstruct->FirstEnergyGroup = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
  for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->FirstEnergyGroup[i] = input.FirstEnergyGroup[i];
  rmfstruct->NumberEnergyGroups = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
  for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->NumberEnergyGroups[i] = input.NumberEnergiesGroup[i];
  rmfstruct->FirstElement = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
  for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->FirstElement[i] = input.FirstElement[i];
  rmfstruct->OrderGroup = (long *) malloc(rmfstruct->NumberTotalGroups*sizeof(long));
  for (size_t i=0; i<(size_t)rmfstruct->NumberTotalGroups; i++) rmfstruct->OrderGroup[i] = input.OrderGroup[i];

  rmfstruct->LowEnergy = (float *) malloc(rmfstruct->NumberEnergyBins*sizeof(float));
  for (size_t i=0; i<(size_t)rmfstruct->NumberEnergyBins; i++) rmfstruct->LowEnergy[i] = input.LowEnergy[i];
  rmfstruct->HighEnergy = (float *) malloc(rmfstruct->NumberEnergyBins*sizeof(float));
  for (size_t i=0; i<(size_t)rmfstruct->NumberEnergyBins; i++) rmfstruct->HighEnergy[i] = input.HighEnergy[i];

  rmfstruct->Matrix = (float *) malloc(rmfstruct->NumberTotalElements*sizeof(float));
  for (size_t i=0; i<(size_t)rmfstruct->NumberTotalElements; i++) rmfstruct->Matrix[i] = input.Matrix[i];
  
  rmfstruct->ChannelLowEnergy = (float *) malloc(rmfstruct->NumberChannels*sizeof(float));
  for (size_t i=0; i<(size_t)rmfstruct->NumberChannels; i++) rmfstruct->ChannelLowEnergy[i] = input.ChannelLowEnergy[i];
  rmfstruct->ChannelHighEnergy = (float *) malloc(rmfstruct->NumberChannels*sizeof(float));
  for (size_t i=0; i<(size_t)rmfstruct->NumberChannels; i++) rmfstruct->ChannelHighEnergy[i] = input.ChannelHighEnergy[i];

  return;

}

void ARFstructToObject(struct ARF *arfstruct, arf& ea)
{

  ea.Version = arfstruct->ARFVersion;
  ea.Telescope = arfstruct->Telescope;  
  ea.Instrument = arfstruct->Instrument;
  ea.Detector = arfstruct->Detector;  
  ea.Filter = arfstruct->Filter;
  ea.ExtensionName = arfstruct->ARFExtensionName;

  ea.EnergyUnits = arfstruct->EnergyUnits;
  ea.arfUnits = arfstruct->arfUnits;

  // now copy the arrays

  ea.LowEnergy.resize(arfstruct->NumberEnergyBins);
  for (size_t i=0; i<ea.LowEnergy.size(); i++) ea.LowEnergy[i] = arfstruct->LowEnergy[i];
  ea.HighEnergy.resize(arfstruct->NumberEnergyBins);
  for (size_t i=0; i<ea.HighEnergy.size(); i++) ea.HighEnergy[i] = arfstruct->HighEnergy[i];
  ea.EffArea.resize(arfstruct->NumberEnergyBins);
  for (size_t i=0; i<ea.EffArea.size(); i++) ea.EffArea[i] = arfstruct->EffArea[i];
  
  return;

}

void ARFobjectToStruct(const arf& ea, struct ARF *arfstruct)
{

  for (size_t i=0; i<=ea.Version.size(); i++) arfstruct->ARFVersion[i] = ea.Version[i];
  for (size_t i=0; i<=ea.Telescope.size(); i++) arfstruct->Telescope[i] = ea.Telescope[i];
  for (size_t i=0; i<=ea.Instrument.size(); i++) arfstruct->Instrument[i] = ea.Instrument[i];
  for (size_t i=0; i<=ea.Detector.size(); i++) arfstruct->Detector[i] = ea.Detector[i];
  for (size_t i=0; i<=ea.Filter.size(); i++) arfstruct->Filter[i] = ea.Filter[i];
  for (size_t i=0; i<=ea.ExtensionName.size(); i++) arfstruct->ARFExtensionName[i] = ea.ExtensionName[i];

  for (size_t i=0; i<=ea.EnergyUnits.size(); i++) arfstruct->EnergyUnits[i] = ea.EnergyUnits[i];
  for (size_t i=0; i<=ea.arfUnits.size(); i++) arfstruct->arfUnits[i] = ea.arfUnits[i];

  // now copy the arrays

  arfstruct->NumberEnergyBins = ea.LowEnergy.size();

  arfstruct->LowEnergy = (float *) malloc(arfstruct->NumberEnergyBins*sizeof(float));
  for (size_t i=0; i<(size_t)arfstruct->NumberEnergyBins; i++) arfstruct->LowEnergy[i] = ea.LowEnergy[i];
  arfstruct->HighEnergy = (float *) malloc(arfstruct->NumberEnergyBins*sizeof(float));
  for (size_t i=0; i<(size_t)arfstruct->NumberEnergyBins; i++) arfstruct->HighEnergy[i] = ea.HighEnergy[i];
  arfstruct->EffArea = (float *) malloc(arfstruct->NumberEnergyBins*sizeof(float));
  for (size_t i=0; i<(size_t)arfstruct->NumberEnergyBins; i++) arfstruct->EffArea[i] = ea.HighEnergy[i];

  return;

}

void PHAstructToObject(struct PHA *phastruct, pha& phaobject)
{

  phaobject.FirstChannel = phastruct->FirstChannel;
  phaobject.Exposure = phastruct->Exposure;
  phaobject.CorrectionScaling = phastruct->CorrectionScaling;
  phaobject.DetChans = phastruct->DetChans;
  phaobject.Poisserr = phastruct->Poisserr;
  phaobject.Datatype = phastruct->Datatype;
  phaobject.Spectrumtype = phastruct->Spectrumtype;
  phaobject.ResponseFile = phastruct->ResponseFile;
  phaobject.AncillaryFile = phastruct->AncillaryFile;
  phaobject.BackgroundFile = phastruct->BackgroundFile;
  phaobject.CorrectionFile = phastruct->CorrectionFile;
  phaobject.ChannelType = phastruct->ChannelType;
  phaobject.PHAVersion = phastruct->PHAVersion;
  phaobject.Telescope = phastruct->Telescope;  
  phaobject.Instrument = phastruct->Instrument;
  phaobject.Detector = phastruct->Detector;  
  phaobject.Filter = phastruct->Filter;
  phaobject.Datamode = phastruct->Datamode;

  // now copy the arrays

  phaobject.Channel.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.Channel.size(); i++) phaobject.Channel[i] = phastruct->Channel[i];
  phaobject.Pha.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.Pha.size(); i++) phaobject.Pha[i] = phastruct->Pha[i];
  phaobject.StatError.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.StatError.size(); i++) phaobject.StatError[i] = phastruct->StatError[i];
  phaobject.SysError.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.SysError.size(); i++) phaobject.SysError[i] = phastruct->SysError[i];

  phaobject.Quality.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.Quality.size(); i++) phaobject.Quality[i] = phastruct->Quality[i];
  phaobject.Group.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.Group.size(); i++) phaobject.Group[i] = phastruct->Grouping[i];

  phaobject.AreaScaling.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.AreaScaling.size(); i++) phaobject.AreaScaling[i] = phastruct->AreaScaling[i];
  phaobject.BackScaling.resize(phastruct->NumberChannels);
  for (size_t i=0; i<phaobject.BackScaling.size(); i++) phaobject.BackScaling[i] = phastruct->BackScaling[i];
  
  return;

}


void PHAobjectToStruct(const pha& phaobject, struct PHA *phastruct)
{

  phastruct->FirstChannel = phaobject.FirstChannel;
  phastruct->Exposure = phaobject.Exposure;
  phastruct->CorrectionScaling = phaobject.CorrectionScaling;
  phastruct->DetChans = phaobject.DetChans;
  phastruct->Poisserr = phaobject.Poisserr;

  for (size_t i=0; i<=phaobject.Datatype.size(); i++) phastruct->Datatype[i] = phaobject.Datatype[i];
  for (size_t i=0; i<=phaobject.Spectrumtype.size(); i++) phastruct->Spectrumtype[i] = phaobject.Spectrumtype[i];
  for (size_t i=0; i<=phaobject.ResponseFile.size(); i++) phastruct->ResponseFile[i] = phaobject.ResponseFile[i];
  for (size_t i=0; i<=phaobject.AncillaryFile.size(); i++) phastruct->AncillaryFile[i] = phaobject.AncillaryFile[i];
  for (size_t i=0; i<=phaobject.BackgroundFile.size(); i++) phastruct->BackgroundFile[i] = phaobject.BackgroundFile[i];
  for (size_t i=0; i<=phaobject.CorrectionFile.size(); i++) phastruct->CorrectionFile[i] = phaobject.CorrectionFile[i];
  for (size_t i=0; i<=phaobject.ChannelType.size(); i++) phastruct->ChannelType[i] = phaobject.ChannelType[i];
  for (size_t i=0; i<=phaobject.PHAVersion.size(); i++) phastruct->PHAVersion[i] = phaobject.PHAVersion[i];
  for (size_t i=0; i<=phaobject.Telescope.size(); i++) phastruct->Telescope[i] = phaobject.Telescope[i]; 
  for (size_t i=0; i<=phaobject.Instrument.size(); i++) phastruct->Instrument[i] = phaobject.Instrument[i];
  for (size_t i=0; i<=phaobject.Detector.size(); i++) phastruct->Detector[i] = phaobject.Detector[i];
  for (size_t i=0; i<=phaobject.Filter.size(); i++) phastruct->Filter[i] = phaobject.Filter[i];
  for (size_t i=0; i<=phaobject.Datamode.size(); i++) phastruct->Datamode[i] = phaobject.Datamode[i];

  // now copy the arrays

  phastruct->NumberChannels = phaobject.Pha.size();

  phastruct->Channel = (int*) malloc(phaobject.Channel.size()*sizeof(int));
  for (size_t i=0; i<phaobject.Channel.size(); i++) phastruct->Channel[i] = phaobject.Channel[i];
  phastruct->Pha = (float*) malloc(phaobject.Pha.size()*sizeof(float));
  for (size_t i=0; i<phaobject.Pha.size(); i++) phastruct->Pha[i] = phaobject.Pha[i];
  phastruct->StatError = (float*) malloc(phaobject.StatError.size()*sizeof(float));
  for (size_t i=0; i<phaobject.StatError.size(); i++) phastruct->StatError[i] = phaobject.StatError[i];
  phastruct->SysError = (float*) malloc(phaobject.SysError.size()*sizeof(float));
  for (size_t i=0; i<phaobject.SysError.size(); i++) phastruct->SysError[i] = phaobject.SysError[i];

  phastruct->Quality = (int*) malloc(phaobject.Quality.size()*sizeof(int));
  for (size_t i=0; i<phaobject.Quality.size(); i++) phastruct->Quality[i] = phaobject.Quality[i];
  phastruct->Grouping = (int*) malloc(phaobject.Group.size()*sizeof(int));
  for (size_t i=0; i<phaobject.Group.size(); i++) phastruct->Grouping[i] = phaobject.Group[i];

  phastruct->AreaScaling = (float*) malloc(phaobject.AreaScaling.size()*sizeof(float));
  for (size_t i=0; i<phaobject.AreaScaling.size(); i++) phastruct->AreaScaling[i] = phaobject.AreaScaling[i];
  phastruct->BackScaling = (float*) malloc(phaobject.BackScaling.size()*sizeof(float));
  for (size_t i=0; i<phaobject.BackScaling.size(); i++) phastruct->BackScaling[i] = phaobject.BackScaling[i];
  
  return;

}

/* Set the CCfits verbose mode */

void SPsetCCfitsVerbose(int mode)
{
  bool verbosity = false;

  if ( mode == 0 ) {
    verbosity = true;
    //debug
    cout << "CCfits verbosity set to true" << std::endl;

  }

  FITS::setVerboseMode(verbosity);

  return;
}

/* copy all HDUs which are not manipulated by this library */

int SPcopyExtensions(char *infile, char *outfile)
{
  Integer Status(0);

  Status = SPcopyHDUs((string)infile, (string)outfile);

  return((int)Status);
}

/* copy all non-critical keywords for the hdunumber instance of the extension hduname. */

int SPcopyKeywords(char *infile, char *outfile, char *hduname, int hdunumber)
{
  Integer Status(0);

  Status = SPcopyKeys((string)infile, (string)outfile, (string)hduname, (Integer)hdunumber);

  return((int)Status);
}

