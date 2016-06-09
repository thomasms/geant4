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
// $Id: G4EnergyLossForExtrapolator.hh,v 1.7 2006/07/21 09:39:02 vnivanch Exp $
// GEANT4 tag $Name: geant4-08-01-patch-02 $
//
//---------------------------------------------------------------------------
//
// ClassName:    G4EnergyLossForExtrapolator
//  
// Description:  This class provide calculation of energy loss, fluctuation, 
//               and msc angle
//
// Author:       09.12.04 V.Ivanchenko 
//
// Modification: 
// 08-04-05 Rename Propogator -> Extrapolator
// 16-03-06 Add muon tables
// 21-03-06 Add verbosity defined in the constructor and Initialisation
//          start only when first public method is called (V.Ivanchenko)
// 03-05-06 Remove unused pointer G4Material* from number of methods (VI)
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef G4EnergyLossForExtrapolator_h
#define G4EnergyLossForExtrapolator_h 1


#include "globals.hh"
#include "G4PhysicsTable.hh"
#include <vector>

class G4ParticleDefinition;
class G4Material;
class G4MaterialCutsCouple;
class G4ProductionCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4EnergyLossForExtrapolator 
{
public:
  G4EnergyLossForExtrapolator(G4int verb = 1);

  virtual ~G4EnergyLossForExtrapolator();

  G4double EnergyAfterStep(G4double kinEnergy, G4double step, 
			   const G4Material*, const G4ParticleDefinition*);

  G4double EnergyAfterStep(G4double kinEnergy, G4double step, 
			   const G4Material*, const G4String& particleName);

  G4double EnergyBeforeStep(G4double kinEnergy, G4double step, 
			    const G4Material*, const G4ParticleDefinition*);

  G4double EnergyBeforeStep(G4double kinEnergy, G4double step, 
			    const G4Material*, const G4String& particleName);

  G4double AverageScatteringAngle(G4double kinEnergy, G4double step, 
				  const G4Material*, const G4ParticleDefinition* part);

  G4double AverageScatteringAngle(G4double kinEnergy, G4double step, 
				  const G4Material*, const G4String& particleName);

  G4double EnergyDispersion(G4double kinEnergy, G4double step, 
			    const G4Material*, const G4ParticleDefinition*);

  G4double EnergyDispersion(G4double kinEnergy, G4double step, 
			    const G4Material*, const G4String& particleName);

  G4double ComputeDEDX(G4double kinEnergy, const G4ParticleDefinition*);

  G4double ComputeRange(G4double kinEnergy, const G4ParticleDefinition*);

  G4double ComputeEnergy(G4double range, const G4ParticleDefinition*);

  void SetVerbose(G4int val);
   
private:

  void Initialisation();

  G4PhysicsTable* PrepareTable();

  const G4ParticleDefinition* FindParticle(const G4String& name);

  G4double ComputeValue(G4double x, const G4PhysicsTable* table);

  void ComputeElectronDEDX(const G4ParticleDefinition* part, G4PhysicsTable* table); 

  void ComputeMuonDEDX(const G4ParticleDefinition* part, G4PhysicsTable* table); 

  void ComputeProtonDEDX(const G4ParticleDefinition* part, G4PhysicsTable* table); 

  G4double ComputeTrueStep(const G4Material*, const G4ParticleDefinition* part, 
			   G4double kinEnergy, G4double stepLength);

  G4double ComputeScatteringAngle(G4double x);

  const G4ParticleDefinition* currentParticle;
  const G4ParticleDefinition* electron;
  const G4ParticleDefinition* positron;
  const G4ParticleDefinition* muonPlus;
  const G4ParticleDefinition* muonMinus;
  const G4ParticleDefinition* proton;

  G4ProductionCuts*        cuts;
  std::vector<const G4MaterialCutsCouple*> couples;

  G4String currentParticleName;

  G4PhysicsTable*          dedxElectron;
  G4PhysicsTable*          dedxPositron;
  G4PhysicsTable*          dedxMuon;
  G4PhysicsTable*          dedxProton;
  G4PhysicsTable*          rangeElectron;
  G4PhysicsTable*          rangePositron;
  G4PhysicsTable*          rangeMuon;
  G4PhysicsTable*          rangeProton;
  G4PhysicsTable*          invRangeElectron;
  G4PhysicsTable*          invRangePositron;
  G4PhysicsTable*          invRangeMuon;
  G4PhysicsTable*          invRangeProton;

  const G4Material* currentMaterial;
  G4int       index;
  G4double    electronDensity;
  G4double    radLength;
  G4double    mass;
  G4double    charge2;
  G4double    kineticEnergy;
  G4double    gam;
  G4double    bg2;
  G4double    beta2;
  G4double    tmax;

  G4double    linLossLimit;
  G4double    emin;
  G4double    emax;
  G4int       nbins;
  G4int       nmat;
  G4int       verbose;
  G4bool      isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::EnergyAfterStep(G4double kinEnergy, 
							     G4double step, 
							     const G4Material* mat, 
							     const G4String& name)
{
  return EnergyAfterStep(kinEnergy,step,mat,FindParticle(name));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::EnergyBeforeStep(G4double kinEnergy, 
							      G4double step, 
							      const G4Material* mat, 
							      const G4String& name)
{
  return EnergyBeforeStep(kinEnergy,step,mat,FindParticle(name));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::AverageScatteringAngle(G4double kinEnergy, 
								    G4double step, 
								    const G4Material* mat, 
								    const G4String& name)
{
  return AverageScatteringAngle(kinEnergy,step,mat,FindParticle(name));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::EnergyDispersion(G4double kinEnergy, 
							      G4double step, 
							      const G4Material* mat, 
							      const G4String& name)
{
  return EnergyDispersion(kinEnergy,step,mat,FindParticle(name));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::AverageScatteringAngle(G4double kinEnergy, 
							     G4double stepLength, 
							     const G4Material* mat, 
							     const G4ParticleDefinition* part)
{
  if(!isInitialised) Initialisation();
  G4double theta = 0.0;
  if(mat && part && kinEnergy > 0.0) {
    G4double step = ComputeTrueStep(mat,part,kinEnergy,stepLength);
    if(step > 0.001*radLength) theta = ComputeScatteringAngle(stepLength);
  }
  return theta;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::ComputeScatteringAngle(G4double x) 
{
  G4double t = x/radLength;
  return 19.23*MeV*std::sqrt(charge2*t)*(1.0 + 0.038*std::log(t))/(beta2*gam*mass);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  
inline G4double G4EnergyLossForExtrapolator::EnergyDispersion(G4double kinEnergy, 
						       G4double stepLength, 
						       const G4Material* mat, 
						       const G4ParticleDefinition* part)
{
  if(!isInitialised) Initialisation();
  G4double sig2 = 0.0;
  if(mat && part ) {
    G4double step = ComputeTrueStep(mat,part,kinEnergy,stepLength);
    sig2 = (1.0/beta2 - 0.5)* twopi_mc2_rcl2 * tmax*step * electronDensity * charge2;
  }
  return sig2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4EnergyLossForExtrapolator::ComputeValue(G4double x, 
							  const G4PhysicsTable* table)
{
  G4double res = 0.0;
  bool b;
  res = ((*table)[index])->GetValue(x, b);
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4EnergyLossForExtrapolator::SetVerbose(G4int val) 
{
  verbose = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
