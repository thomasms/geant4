//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#ifndef exrdmHisto_h
#define exrdmHisto_h 1

//---------------------------------------------------------------------------
//
// ClassName:   exrdmHisto
//
// Description: Utility class to hold and manipulate histograms/nTuples
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class exrdmHistoMessenger;

namespace AIDA {
 class ITree;
 class ITuple;
 class IHistogram1D;
}

class exrdmHisto
{

public:
  exrdmHisto();

  ~exrdmHisto();

  void book();
  // Book predefined histogramms 

  void save();
  // Save histogramms to file

  void add1D(const G4String&, const G4String&, G4int nb=100, G4double x1=0., 
                                               G4double x2=1., G4double u=1.);
  // In this method histogramms are predefined

  void setHisto1D(G4int, G4int, G4double, G4double, G4double);
  // It change bins and boundaries

  void fillHisto(G4int, G4double, G4double);
  // exrdmHistogramms are filled

  void scaleHisto(G4int, G4double);

  void addTuple(const G4String&, const G4String&, const G4String&);
  // In this method nTuple is booked

  void fillTuple(G4int, const G4String&, G4double);
  // Fill nTuple parameter with a double

  void fillTuple(G4int, const G4String&, G4String);
  // Fill nTuple parameter with a string

  void addRow(G4int);
  // Save tuple event 

  void setFileName(const G4String&);

  void setFileType(const G4String&);

private:

  G4String histName;
  G4String histType;

  G4int    nHisto;
  G4int    nTuple;
  G4int    verbose;
  G4int    defaultAct;

  std::vector<AIDA::IHistogram1D*> histo;
  std::vector<AIDA::ITuple*>   ntup;
  AIDA::ITree*    tree;
  exrdmHistoMessenger* messenger;

  std::vector<G4int>     active;
  std::vector<G4int>     bins;
  std::vector<G4double>  xmin;
  std::vector<G4double>  xmax;
  std::vector<G4double>  unit;
  std::vector<G4String>  ids;
  std::vector<G4String>  titles;
  std::vector<G4String>  tupleName;
  std::vector<G4String>  tupleId;
  std::vector<G4String>  tupleList;

};

#endif