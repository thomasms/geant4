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
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History:
//      17 August 2004  P. Gumplinger, T. MacPhail
// ------------------------------------------------------------
//
#ifndef G4DecayWithSpin_h
#define G4DecayWithSpin_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VRestDiscreteProcess.hh"
#include "G4ParticleChangeForDecay.hh"

#include "G4Decay.hh"

class G4VExtDecayer;

class G4DecayWithSpin : public G4Decay
{
  public:
    //  Constructors
    G4DecayWithSpin(const G4String& processName ="DecayWithSpin");

    //  Destructor
    virtual ~G4DecayWithSpin();

  protected: // With Description
    virtual G4VParticleChange* DecayIt(
                             const G4Track& aTrack,
                             const G4Step&  aStep
                            );
    // The DecayIt() method returns by pointer a particle-change object,
    // which has information of daughter particles.

  private:
  G4ThreeVector Spin_Precession(const G4Step& aStep,
                                G4ThreeVector B, G4double deltatime );

};

#endif
