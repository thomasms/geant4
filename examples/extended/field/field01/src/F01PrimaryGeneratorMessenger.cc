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
// $Id: F01PrimaryGeneratorMessenger.cc,v 1.5 2006/06/29 17:16:45 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "F01PrimaryGeneratorMessenger.hh"

#include "F01PrimaryGeneratorAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

F01PrimaryGeneratorMessenger::F01PrimaryGeneratorMessenger(F01PrimaryGeneratorAction* F01Gun)
  : F01Action(F01Gun)
{ 
  RndmCmd = new G4UIcmdWithAString("/gun/random",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on, off(default)");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("off");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  setxvertexCmd = new G4UIcmdWithADoubleAndUnit("/gun/xvertex",this);
  setxvertexCmd->SetGuidance(" Set x coord. of the primary vertex.");
  setxvertexCmd->SetParameterName("xv",true);
  setxvertexCmd->SetDefaultValue(0.0*mm) ; 
  
  setyvertexCmd = new G4UIcmdWithADoubleAndUnit("/gun/yvertex",this);
  setyvertexCmd->SetGuidance(" Set y coord. of the primary vertex.");
  setyvertexCmd->SetParameterName("yv",true);
  setyvertexCmd->SetDefaultValue(0.0*mm) ; 
  
  setzvertexCmd = new G4UIcmdWithADoubleAndUnit("/gun/zvertex",this);
  setzvertexCmd->SetGuidance(" Set z coord. of the primary vertex.");
  setzvertexCmd->SetParameterName("zv",true);
  setzvertexCmd->SetDefaultValue(0.0*mm) ; 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

F01PrimaryGeneratorMessenger::~F01PrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete setxvertexCmd;
  delete setyvertexCmd;
  delete setzvertexCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void F01PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{ 
  if( command == RndmCmd )
   { F01Action->SetRndmFlag(newValue);}
  if( command == setxvertexCmd)
   { F01Action->Setxvertex(setxvertexCmd->GetNewDoubleValue(newValue));}
  if( command == setyvertexCmd)
   { F01Action->Setyvertex(setyvertexCmd->GetNewDoubleValue(newValue));}
  if( command == setzvertexCmd)
   { F01Action->Setzvertex(setzvertexCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

