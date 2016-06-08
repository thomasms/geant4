// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: ExN02SteppingAction.cc,v 1.3 1998/10/09 14:35:35 japost Exp $
// GEANT4 tag $Name: geant4-00 $
//
// 

#include "ExN02SteppingAction.hh"
#include "ExN02DetectorConstruction.hh"
#include "ExN02EventAction.hh"
#include "G4SteppingManager.hh"

ExN02SteppingAction::ExN02SteppingAction(ExN02DetectorConstruction* myDC, ExN02EventAction* myEA)
:myDetector(myDC), eventAction(myEA)
{ }

void ExN02SteppingAction::UserSteppingAction()
{
  G4Step* aStep = GetSteppingManager()->GetStep();
  
  // collect the energy deposited in the absorber
  
  const G4VPhysicalVolume* currentVolume = aStep->GetPreStepPoint()-> GetPhysicalVolume();

#if 0
  const G4VPhysicalVolume* absorber = myDetector->getAbsorber();

  if (currentVolume == absorber)
   {
    G4double EdepStep = aStep->GetTotalEnergyDeposit();
    eventAction->addEdep(EdepStep);
   } 
#endif
}

