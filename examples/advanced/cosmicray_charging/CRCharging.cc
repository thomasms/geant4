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
// ********************************************************************
// *                                                                  *
// * cosmicray_charging advanced example for Geant4                   *
// * (adapted simulation of test-mass charging in the LISA mission)   *
// *                                                                  *
// * Henrique Araujo (h.araujo@imperial.ac.uk) & Peter Wass           *
// * Imperial College London                                          *
// *                                                                  *
// * main program class                                               *
// *                                                                  *
// ********************************************************************
//
// HISTORY
// 22/02/2004: migrated from LISA-V04
//
// ********************************************************************

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "LISADetectorConstruction.hh"
#include "LISAPhysicsList.hh"
#include "LISAPrimaryGeneratorAction.hh"
#include "LISARunAction.hh"
#include "LISAEventAction.hh"
#include "LISASteppingAction.hh"
#include "LISAStackingAction.hh"

#include <vector>


int main(int argc,char** argv) {
  
  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  LISADetectorConstruction* detector = new LISADetectorConstruction;
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new LISAPhysicsList);
  LISAPrimaryGeneratorAction* generatorAction = new LISAPrimaryGeneratorAction;
  runManager->SetUserAction(generatorAction);
  
  // set user action classes
  LISASteppingAction* steppingAction = new LISASteppingAction;
  runManager->SetUserAction(steppingAction);
  runManager->SetUserAction(new LISAStackingAction);
  runManager->SetUserAction(
     new LISAEventAction(generatorAction,steppingAction));
  runManager->SetUserAction(new LISARunAction);

  
  G4UIsession* session=0;
  if (argc==1) {  // Define UI session for interactive mode.
    // G4UIterminal is a (dumb) terminal.
#ifdef G4UI_USE_XM
    session = new G4UIXm(argc,argv);
#else           
#ifdef G4UI_USE_TCSH
    session = new G4UIterminal(new G4UItcsh);      
#else
    session = new G4UIterminal();
#endif
#endif
  }
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  

  //Initialize G4 kernel
  runManager->Initialize();
  
  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  
  
  // Define UI session for interactive mode.
  if(session) {  
    // G4UIterminal is a (dumb) terminal.
    UI->ApplyCommand("/control/execute init.mac");    
    session->SessionStart();
    delete session;
  }
  // Batch mode
  else { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }
  
  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

