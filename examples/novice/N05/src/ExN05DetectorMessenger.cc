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
// $Id: ExN05DetectorMessenger.cc,v 1.8 2006/06/29 17:53:07 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 

#include "ExN05DetectorMessenger.hh"
#include "ExN05DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"
#include <iomanip>                

#include <sstream>

ExN05DetectorMessenger::ExN05DetectorMessenger(ExN05DetectorConstruction * myDet) : myDetector(myDet)
{ 

  mydetDir = new G4UIdirectory("/exN05/detector/");
  mydetDir->SetGuidance("ExN05 detector control.");
  
  SwitchCmd = new G4UIcmdWithAString("/exN05/detector/switchUserLimits",this);
  SwitchCmd->SetGuidance("Switch On/Off UserLimits for crystals");
  SwitchCmd->SetGuidance(" Choice : on / off ");
  SwitchCmd->SetParameterName("choice",true);
  SwitchCmd->SetDefaultValue("on");
  SwitchCmd->SetCandidates("on ON off OFF");
  SwitchCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  TmaxCmd = new G4UIcmdWithADoubleAndUnit("/exN05/detector/maxTime",this);
  TmaxCmd->SetGuidance("Set maximum time in crystal");
  TmaxCmd->SetParameterName("Tmax",false,false);
  TmaxCmd->SetDefaultUnit("ns");
  TmaxCmd->SetUnitCategory("Time");
  TmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EminCmd = new G4UIcmdWithADoubleAndUnit("/exN05/detector/minEkine",this);
  EminCmd->SetGuidance("Set minimum kinetic energy in crystal");
  EminCmd->SetParameterName("Emin",false,false);
  EminCmd->SetDefaultUnit("MeV");
  EminCmd->SetUnitCategory("Energy");
  EminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RminCmd = new G4UIcmdWithADoubleAndUnit("/exN05/detector/minRange",this);
  RminCmd->SetGuidance("Set minimum range in crystal");
  RminCmd->SetParameterName("Rmin",false,false);
  RminCmd->SetDefaultUnit("mm");
  RminCmd->SetUnitCategory("Length");
  RminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

ExN05DetectorMessenger::~ExN05DetectorMessenger()
{
  delete SwitchCmd;
  delete TmaxCmd;
  delete RminCmd;
  delete EminCmd;
}

void ExN05DetectorMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{ 
  if( command == SwitchCmd ) { 
    if ( (newValues == "off") || (newValues == "OFF") ){
      myDetector->UseUserLimits(false);
    } else {
      myDetector->UseUserLimits(true);
    } 

  } else if( command == TmaxCmd ) {
    myDetector->SetMaxTimeInCrystal(TmaxCmd->GetNewDoubleValue(newValues));

  } else if( command == EminCmd ) {
    myDetector->SetMinEkineInCrystal(EminCmd->GetNewDoubleValue(newValues));

  } else if( command == RminCmd ) {
    myDetector->SetMinRangeInCrystal(RminCmd->GetNewDoubleValue(newValues));
  }
}

G4String ExN05DetectorMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String returnValue('\0');
  std::ostringstream os;
  
  if( command == SwitchCmd ) { 
    if ( myDetector->IsUseUserLimits() )returnValue = "on";
    else returnValue = "off";

  } else if( command == TmaxCmd ) {
    os <<  myDetector->GetMaxTimeInCrystal()/ns << " ns" << '\0';
    returnValue = os.str();

  } else if( command == EminCmd ) {
    os <<  myDetector->GetMinEkineInCrystal()/MeV << " MeV" << '\0';
    returnValue = os.str();

  } else if( command == RminCmd ) {
    os <<  myDetector->GetMinRangeInCrystal()/mm << " mm" << '\0';
    returnValue = os.str();
  }
  return returnValue;
}
