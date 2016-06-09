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
// $Id: exampleB02.cc,v 1.20 2006/06/29 16:34:23 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - exampleB02
//
// --------------------------------------------------------------
// Comments
//
// This example intends to show how to use both importance sampling and a
// customized scoring making use of the scoring framework
// in a parallel geometry.
// 
// A simple geometry consisting of a 180 cm high concrete cylinder
// is constructed in the mass geometry.
// A geometry is constructed in the parallel geometry
// in order to assign importance values to slabs
// of width 10cm and for scoring. The parallel world volume should 
// overlap the mass world volume and  the radii of the slabs is larger 
// than the radius of the concrete cylinder in the mass geometry.
// Pairs of G4GeometryCell and importance values are stored in
// the importance store.
// The scoring uses the G4CellSCorer and one customized scorer for
// the last slab. 
// 
// 

// --------------------------------------------------------------

#include <iostream>

#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "B02DetectorConstruction.hh"
#include "B02PhysicsList.hh"
#include "B02PrimaryGeneratorAction.hh"

// construction for the parallel geometry
#include "B02ImportanceDetectorConstruction.hh"

// Files specific for biasing and scoring
#include "G4Scorer.hh"
#include "G4ParallelGeometrySampler.hh"
#include "G4IStore.hh"

// customized scorer and store
#include "B02CellScorer.hh"
#include "B02CellScorerStore.hh"

// a score table
#include "G4ScoreTable.hh"

// AIDA stuff

#include "AIDA/IAnalysisFactory.h"
#include "AIDA/ITree.h"
#include "AIDA/ITreeFactory.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogramFactory.h"



int main(int , char **)
{  
  std::ostream *myout = &G4cout;
  G4int numberOfEvent = 100;

  G4long myseed = 345354;

  CLHEP::HepRandom::setTheSeed(myseed);

  G4RunManager *runManager = new G4RunManager;
  
  // create the detector      ---------------------------
  B02DetectorConstruction *detector = new B02DetectorConstruction();
  runManager->SetUserInitialization(detector);
  //  ---------------------------------------------------
  runManager->SetUserInitialization(new B02PhysicsList);
  runManager->SetUserAction(new B02PrimaryGeneratorAction);
  //  runManager->Initialize();


  // create a parallel detector
  B02ImportanceDetectorConstruction *pdet = 
    new B02ImportanceDetectorConstruction;
  runManager->Initialize();


  // create an importance 
  G4IStore aIstore(pdet->GetWorldVolume());
  // create a geometry cell for the world volume replpicanumber is 0!
  G4GeometryCell gWorldVolumeCell(pdet->GetWorldVolume(), 0);
  // set world volume importance to 1
  aIstore.AddImportanceGeometryCell(1, gWorldVolumeCell);

  // create a customized cell scorer store
  B02CellScorerStore b02store;

  // set importance values and create scorers 
  G4int cell(1);
  for (cell=1; cell<=18; cell++) {
    G4GeometryCell gCell = pdet->GetGeometryCell(cell);
    G4double imp = std::pow(2.0,cell-1);
    aIstore.AddImportanceGeometryCell(imp, gCell);
    // adding the standard G4CellScorer for 17 concrete cells
    if (cell<18) {
      b02store.AddG4CellScorer(gCell);
    }
  }

  // create a histogram for a special scorer for the last cell 
  AIDA::IAnalysisFactory *af = AIDA_createAnalysisFactory();
  AIDA::ITreeFactory *tf = af->createTreeFactory();
  AIDA::ITree *tree = tf->create("b02.hbook", "hbook",false,true);
  AIDA::IHistogramFactory *hf = af->createHistogramFactory( *tree );
  AIDA::IHistogram1D *h = 
    hf->createHistogram1D("10","w*sl vs. e", 30, 0., 20*MeV);
  // create a special scorer for the last cell 
  B02CellScorer b02scorer(h);
  



  // creating the geometry cell and add both to the store
  G4GeometryCell gCell = pdet->GetGeometryCell(18);
  b02store.AddB02CellScorer(&b02scorer, gCell);

  // create importance geometry cell pair for the "rest"cell
  // with the same importance as the last concrete cell 
  gCell = pdet->GetGeometryCell(19);
  G4double imp = std::pow(2.0,18); 
  aIstore.AddImportanceGeometryCell(imp, gCell);
  

  // create the importance and scoring sampler for biasing and scoring 
  // in the parallel world

  G4CellStoreScorer scorer(b02store); 

  G4ParallelGeometrySampler pgs(pdet->GetWorldVolume(),
				"neutron");
  pgs.PrepareScoring(&scorer);
  pgs.PrepareImportanceSampling(&aIstore, 0);
  pgs.Configure();

  runManager->BeamOn(numberOfEvent);

  // print a table of the scores
  G4ScoreTable sp(&aIstore);
    sp.Print(b02store.GetMapGeometryCellCellScorer(), myout);

 
  tree->commit();
  tree->close();

  delete af;
  delete tf;
  delete tree;

  pgs.ClearSampling();

  delete runManager;
 
  return 0;
}



