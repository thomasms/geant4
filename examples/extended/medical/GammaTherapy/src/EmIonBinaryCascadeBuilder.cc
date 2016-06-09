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
// $Id: EmIonBinaryCascadeBuilder.cc,v 1.2 2006/06/29 17:27:15 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $

#include "EmIonBinaryCascadeBuilder.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

#include "G4HadronInelasticProcess.hh"
#include "G4BinaryLightIonReaction.hh"
#include "G4TripathiCrossSection.hh"
#include "G4IonsShenCrossSection.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmIonBinaryCascadeBuilder::EmIonBinaryCascadeBuilder(const G4String& name)
   :  G4VPhysicsConstructor(name)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmIonBinaryCascadeBuilder::~EmIonBinaryCascadeBuilder()
{
  delete    theIPdeuteron;
  delete    theIPtriton;
  delete    theIPalpha;
  delete    theIPHe3;
  delete    theIPGenericIon;
  delete    theBC;
  delete    theGenIonBC;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmIonBinaryCascadeBuilder::ConstructProcess()
{
  // Binary Cascade
  G4ParticleDefinition* particle = 0;
  G4ProcessManager* pmanager = 0;
  theBC = new G4BinaryLightIonReaction();
  theBC->SetMinEnergy(80*MeV);
  theBC->SetMaxEnergy(20*GeV);

  G4TripathiCrossSection * TripathiCrossSection= new G4TripathiCrossSection;
  G4IonsShenCrossSection * aShen = new G4IonsShenCrossSection;

  // deuteron
  particle = G4Deuteron::Deuteron();
  pmanager = particle->GetProcessManager();
  theIPdeuteron = new G4DeuteronInelasticProcess();
  G4LEDeuteronInelastic* theDIModel = new G4LEDeuteronInelastic();
  theDIModel->SetMaxEnergy(100*MeV);
  theIPdeuteron->AddDataSet(TripathiCrossSection);
  theIPdeuteron->AddDataSet(aShen);
  theIPdeuteron->RegisterMe(theDIModel);
  theIPdeuteron->RegisterMe(theBC);
  pmanager->AddDiscreteProcess(theIPdeuteron);

  // triton
  particle = G4Triton::Triton();
  pmanager = particle->GetProcessManager();
  theIPtriton = new G4TritonInelasticProcess();
  G4LETritonInelastic* theTIModel = new G4LETritonInelastic();
  theTIModel->SetMaxEnergy(100*MeV);
  theIPtriton->AddDataSet(TripathiCrossSection);
  theIPtriton->AddDataSet(aShen);
  theIPtriton->RegisterMe(theTIModel);
  theIPtriton->RegisterMe(theBC);
  pmanager->AddDiscreteProcess(theIPtriton);

  // alpha
  particle = G4Alpha::Alpha();
  pmanager = particle->GetProcessManager();
  theIPalpha = new G4AlphaInelasticProcess();
  G4LEAlphaInelastic* theAIModel = new G4LEAlphaInelastic();
  theAIModel->SetMaxEnergy(100*MeV);
  theIPalpha->AddDataSet(TripathiCrossSection);
  theIPalpha->AddDataSet(aShen);
  theIPalpha->RegisterMe(theAIModel);
  theIPalpha->RegisterMe(theBC);
  pmanager->AddDiscreteProcess(theIPalpha);


  // GenericIon
  particle = G4GenericIon::GenericIon();
  pmanager = particle->GetProcessManager();
  theIPGenericIon = new G4HadronInelasticProcess("IonInelastic",particle);
  theIPGenericIon->AddDataSet(TripathiCrossSection);
  theIPGenericIon->AddDataSet(aShen);
  theGenIonBC= new G4BinaryLightIonReaction();
  theGenIonBC->SetMinEnergy(0*MeV);
  theGenIonBC->SetMaxEnergy(10*GeV);
  theIPGenericIon->RegisterMe(theGenIonBC);
  pmanager->AddDiscreteProcess(theIPGenericIon);

  // He3
  particle = G4He3::He3();
  pmanager = particle->GetProcessManager();
  theIPHe3 = new G4HadronInelasticProcess("He3Inelastic",particle);
  theIPHe3->AddDataSet(TripathiCrossSection);
  theIPHe3->AddDataSet(aShen);
  theIPHe3->RegisterMe(theGenIonBC);
  pmanager->AddDiscreteProcess(theIPHe3);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

