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
// $Id: G4StackingMessenger.cc,v 1.5 2006/06/29 18:10:23 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// --------------------------------------------------------------------

#include "G4StackingMessenger.hh"
#include "G4StackManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4ios.hh"

G4StackingMessenger::G4StackingMessenger(G4StackManager * fCont)
:fContainer(fCont)
{
  stackDir = new G4UIdirectory("/event/stack/");
  stackDir->SetGuidance("Stack control commands.");

  statusCmd = new G4UIcmdWithoutParameter("/event/stack/status",this);
  statusCmd->SetGuidance("List current status of the stack.");

  clearCmd = new G4UIcmdWithAnInteger("/event/stack/clear",this);
  clearCmd->SetGuidance("Clear stacked tracks.");
  clearCmd->SetGuidance(" 2 : clear all tracks in all stacks");
  clearCmd->SetGuidance(" 1 : clear tracks in the urgent and waiting stacks");
  clearCmd->SetGuidance(" 0 : clear tracks in the waiting stack (default)");
  clearCmd->SetGuidance("-1 : clear tracks in the urgent stack");
  clearCmd->SetGuidance("-2 : clear tracks in the postponed stack");
  clearCmd->SetParameterName("level",true);
  clearCmd->SetDefaultValue(0);
  clearCmd->SetRange("level>=-2&&level<=2");
  clearCmd->AvailableForStates(G4State_GeomClosed,G4State_EventProc);
}

G4StackingMessenger::~G4StackingMessenger()
{
  delete statusCmd;
  delete clearCmd;
  delete stackDir;
}

void G4StackingMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{
  if( command==statusCmd )
  {
    G4cout << "========================== Current status of the stack =====" << G4endl;
    G4cout << " Number of tracks in the stack" << G4endl;
    G4cout << "    Urgent stack    : " << fContainer->GetNUrgentTrack() << G4endl;
    G4cout << "    Waiting stack   : " << fContainer->GetNWaitingTrack() << G4endl;
    G4cout << "    Postponed stack : " << fContainer->GetNPostponedTrack() << G4endl;
  }
  if( command==clearCmd )
  {
    G4int vc = clearCmd->GetNewIntValue(newValues);
    switch (vc)
    { 
      case 2:
        fContainer->ClearPostponeStack();
      case 1:
        fContainer->ClearUrgentStack();
      case 0:
        fContainer->ClearWaitingStack();
        break;
      case -1:
        fContainer->ClearUrgentStack();
        break;
      case -2:
        fContainer->ClearPostponeStack();
        break;
    }
  }
}

