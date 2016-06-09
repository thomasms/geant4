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
// -*- C++ -*-
// $Id: g4_gdml_read_write.cc,v 1.6 2006/06/29 17:21:23 gunter Exp $
//
// Author: Radovan Chytracek 2000 - 2004
//         Witek Pokorski
//
#include <stdexcept>

#include "globals.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4TransportationManager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "g4rwDetectorConstruction.hh"
#include "g4rwPhysicsList.hh"
#include "g4rwPrimaryGeneratorAction.hh"

//g4 writer
#include "G4Writer/G4GDMLWriter.h"


int main()
{
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  //
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // set mandatory initialization classes
  runManager->SetUserInitialization(new gogdmlDetectorConstruction);
  runManager->SetUserInitialization(new gogdmlPhysicsList);

  // set mandatory user action class
  runManager->SetUserAction(new gogdmlPrimaryGeneratorAction);

  // initialize G4 kernel
  runManager->Initialize();  

  // scanning geometry tree
  G4VPhysicalVolume* g4wv =
    G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking()->GetWorldVolume();
  
  G4GDMLWriter g4writer("GDMLschema/gdml.xsd", "g4writertest.gdml");
  
  try
  {
    g4writer.DumpGeometryInfo(g4wv);
  }
  catch(std::logic_error &lerr)
  {
    std::cout << "Caught an exception: " 
              << lerr.what () << std::endl;
  }

  
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // G4UIterminal is a (dumb) terminal.
  G4UIsession * session = new G4UIterminal(new G4UItcsh);
  
  UI->ApplyCommand("/control/execute vis.mac"); 

  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/event/verbose 0");
  UI->ApplyCommand("/tracking/verbose 1");

  // start a run
  int numberOfEvent = 10;
  runManager->BeamOn(numberOfEvent);

  session->SessionStart();

  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

