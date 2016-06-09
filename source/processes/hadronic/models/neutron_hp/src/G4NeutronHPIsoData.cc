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
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
#include "G4NeutronHPIsoData.hh"
#include "G4NeutronHPDataUsed.hh"

  G4bool G4NeutronHPIsoData::Init(G4int A, G4int Z, G4double abun, G4String dirName, G4String aFSType)
  {
    theChannelData = NULL;
    G4double abundance = abun/100.;
    G4String filename;
    G4bool result = true;
    G4NeutronHPDataUsed aFile = theNames.GetName(A, Z, dirName, aFSType, result);
    filename = aFile.GetName();
//    if(filename=="") return false;
    std::ifstream theChannel(filename);
    
    if(Z==1 && (aFile.GetZ()!=Z || std::abs(aFile.GetA()-A)>0.0001) )
    {
      if(getenv("NeutronHPNamesLogging")) G4cout << "Skipped = "<< filename <<" "<<A<<" "<<Z<<G4endl;
      theChannel.close();
      return false;
    }
    if(!theChannel) {theChannel.close(); return false;}
    // accommodating deficiencie of some compilers
    if(theChannel.eof()) {theChannel.close(); return false;} 
    if(!theChannel) {theChannel.close(); return false;}
    G4int dummy; 
    theChannel >> dummy >> dummy;
    theChannelData = new G4NeutronHPVector;
    G4int nData;
    theChannel >> nData;
    theChannelData->Init(theChannel, nData, eV, abundance*barn);
//    G4cout << "Channel Data Statistics: "<<theChannelData->GetVectorLength()<<G4endl;
//    G4cout << "Channel data"<<G4endl;
//     G4int hpw;
//     G4cin >> hpw;
//    theChannelData->Dump();
    theChannel.close();
    return result;
  }
  
  void G4NeutronHPIsoData::Init(G4int A, G4int Z, G4double abun) //fill PhysicsVector for this Isotope
  {
    G4String dirName;
    if(!getenv("NeutronHPCrossSections")) 
       throw G4HadronicException(__FILE__, __LINE__, "Please setenv NeutronHPCrossSections to point to the neutron cross-section files.");
    G4String baseName = getenv("NeutronHPCrossSections");
    dirName = baseName+"/Fission";
    if(Z>89) 
    {
      Init(A, Z, abun, dirName, "/CrossSection/");
    }
    else
    {
       theChannelData = new G4NeutronHPVector;
    }
    theFissionData = theChannelData;
    theChannelData = NULL; // fast fix for double delete; revisit later. @@@@@@@
    dirName = baseName+"/Capture";
    Init(A, Z, abun, dirName, "/CrossSection/");
    theCaptureData = theChannelData;
     theChannelData = NULL;
    dirName = baseName+"/Elastic";
    Init(A, Z, abun, dirName, "/CrossSection/");
    theElasticData = theChannelData;
    theChannelData = NULL;
    dirName = baseName+"/Inelastic";
    Init(A, Z, abun, dirName, "/CrossSection/");
    theInelasticData = theChannelData;
    theChannelData = NULL;
    
//    if(theInelasticData!=NULL) G4cout << "Inelastic Data Statistics: "<<theInelasticData->GetVectorLength()<<G4endl;
//    if(theElasticData!=NULL) G4cout << "Elastic Data Statistics: "<<theElasticData->GetVectorLength()<<G4endl;
//    if(theCaptureData!=NULL) G4cout << "Capture Data Statistics: "<<theCaptureData->GetVectorLength()<<G4endl;
//    if(theFissionData!=NULL) G4cout << "Fission Data Statistics: "<<theFissionData->GetVectorLength()<<G4endl;
//  G4cout << "Inelastic data"<<G4endl;
//  if(theInelasticData!=NULL) theInelasticData->Dump();
//  G4cout << "Elastic data"<<G4endl;
//  if(theElasticData!=NULL) theElasticData->Dump();
//  G4cout << "Capture data"<<G4endl;
//  if(theCaptureData!=NULL) theCaptureData->Dump();
//  G4cout << "Fission data"<<G4endl;
//  if(theFissionData!=NULL) theFissionData->Dump();

  }
  
  G4String G4NeutronHPIsoData::GetName(G4int A, G4int Z, G4String base, G4String rest)
  {
    G4bool dbool;
    return (theNames.GetName(A, Z, base, rest, dbool)).GetName();
  }
  
