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
// ====================================================================
//
//   ExN04PrimaryGeneratorMessenger.hh
//   $Id: ExN04PrimaryGeneratorMessenger.hh,v 1.3 2006/06/29 17:04:52 gunter Exp $
//
// ====================================================================
#ifndef EXN04_PRIMARY_GENERATOR_MESSENGER_H
#define EXN04_PRIMARY_GENERATOR_MESSENGER_H

#include "globals.hh"
#include "G4UImessenger.hh"

class ExN04PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;

class ExN04PrimaryGeneratorMessenger : public G4UImessenger {
private:
  ExN04PrimaryGeneratorAction* primaryAction;

  G4UIdirectory* dir;
  G4UIdirectory* mydetdir;
  G4UIcmdWithAString* select;

public:
  ExN04PrimaryGeneratorMessenger(ExN04PrimaryGeneratorAction* genaction);
  ~ExN04PrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);  
};


#endif
