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
// $Id: StackingAction.cc,v 1.3 2006/06/29 17:24:30 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
/////////////////////////////////////////////////////////////////////////
//
// StackingAction
//
// Created: 31.04.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
// 

#include "StackingAction.hh"

#include "HistoManager.hh"
#include "StackingMessenger.hh"

#include "G4Track.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction()
{
  stackMessenger = new StackingMessenger(this);
  histoManager   = HistoManager::GetPointer();
  killSecondary  = false;
  pname          = ""; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{
  delete stackMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ClassificationOfNewTrack status = fUrgent;

  if (aTrack->GetTrackStatus() == fAlive) 
    histoManager->ScoreNewTrack(aTrack);

  const G4String name = aTrack->GetDefinition()->GetParticleName();

  if(histoManager->GetVerbose() > 1 ) {
    G4cout << "Track #"
	   << aTrack->GetTrackID() << " of " << name
	   << " E(MeV)= " << aTrack->GetKineticEnergy()/MeV
	   << " produced by " 
	   << histoManager->CurrentDefinition()->GetParticleName()
	   << " ID= " << aTrack->GetParentID()
	   << " with E(MeV)= " << histoManager->CurrentKinEnergy()/MeV
	   << G4endl;
  }

  //stack or delete secondaries
  if (killSecondary)      status = fKill;
  else if(pname == name)  status = fKill; 

  return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
