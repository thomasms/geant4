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
// $Id: G4HadronElasticPhysics.hh,v 1.5 2006/06/29 17:58:32 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HadronElasticPhysics
//
// Author: 11 April 2006 V. Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//

#ifndef G4HadronElasticPhysics_h
#define G4HadronElasticPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4UHadronElasticProcess.hh"
#include "G4HadronElastic.hh"
#include <vector>

class G4HadronElasticPhysics : public G4VPhysicsConstructor
{
public: 
  G4HadronElasticPhysics(const G4String& name = "elastic",
			 G4int ver = 1, G4bool hp = false);
  virtual ~G4HadronElasticPhysics();

public: 
  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type 
  virtual void ConstructProcess();

  void SetLimitSWave(G4double);

  void SetLimitIonKineticEnergy(G4double);

private:

  std::vector<G4HadronicProcess*> p_list;

  G4String mname;

  G4double pLimit;
  G4double edepLimit;

  G4int    verbose;
  G4bool   hpFlag;
  G4bool   wasActivated;
};

inline void G4HadronElasticPhysics::SetLimitSWave(G4double val)
{
  pLimit = val;
}

inline void G4HadronElasticPhysics::SetLimitIonKineticEnergy(G4double val)
{
  edepLimit = val;
}


#endif








