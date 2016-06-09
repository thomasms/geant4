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
//
// File name:     RadmonPhysicsPositronStandard.cc
// Creation date: Nov 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonPhysicsPositronStandard.cc,v 1.4 2006/06/29 16:19:50 gunter Exp $
// Tag:           $Name: geant4-08-01 $
//

#include "RadmonPhysicsPositronStandard.hh"

#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "G4LeptonConstructor.hh"

#include "G4ProcessManager.hh"
#include "G4MultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4StepLimiter.hh"

RadmonVSubPhysicsListWithLabel *                RadmonPhysicsPositronStandard :: New(void) const
{
 return new RadmonPhysicsPositronStandard;
}



void                                            RadmonPhysicsPositronStandard :: ConstructParticle(void)
{
 G4Gamma::GammaDefinition();
 G4LeptonConstructor::ConstructParticle();
}



void                                            RadmonPhysicsPositronStandard :: ConstructProcess(void)
{
 G4ProcessManager * manager(G4Positron::PositronDefinition()->GetProcessManager());

 manager->AddProcess(new G4MultipleScattering, ordInActive,           1, 1);
 manager->AddProcess(new G4eIonisation,        ordInActive,           2, 2);
 manager->AddProcess(new G4eBremsstrahlung,    ordInActive, ordInActive, 3);
 manager->AddProcess(new G4eplusAnnihilation,            0, ordInActive, 4);
 manager->AddProcess(new G4StepLimiter,        ordInActive, ordInActive, 5);
}



void                                            RadmonPhysicsPositronStandard :: SetCuts(void)
{
}





const RadmonPhysicsInfoList &                   RadmonPhysicsPositronStandard :: Provides(void) const
{
 if (infoList.GetNPhysicsInfos()==0)
 {
  RadmonPhysicsInfo info;
  
  info.SetProcessName("MultipleScattering");
  info.SetParticleDefinition(G4Positron::PositronDefinition());
  info.SetMinEnergy(0.1*keV);
  info.SetMaxEnergy(100.*TeV);
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("Ionisation");
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("Bremsstrahlung");
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("ePlusAnnihilation");
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("StepLimiter");
  info.SetMinEnergy(0.*eV);
  info.SetMaxEnergy(DBL_MAX);
  infoList.InsertPhysicsInfo(info);
 }
 
 return infoList;
}
