//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
///////////////////////////////////////////////////////////////////////////////
// File: CCalDetectorConstruction.cc
// Description: CCalDetectorConstruction user action class to construct 
//              detector geometry
///////////////////////////////////////////////////////////////////////////////
#include "CCalDetectorConstruction.hh"

//#define debug

#ifdef debug
#include "G4Timer.hh"
#endif

#include "CCalMaterialFactory.hh"
#include "CCalRotationMatrixFactory.hh"
#include "CCalSensAssign.hh"
#include "CCalMagneticField.hh"
#include "CCalG4Hall.hh"
#include "CCalutils.hh"

#include "CCalSensitiveConfiguration.hh"
#include "CCalEcalOrganization.hh"
#include "CCalHcalOrganization.hh"
#include "G4SDManager.hh"

#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4PropagatorInField.hh"
#include "G4TransportationManager.hh"

#include "G4ClassicalRK4.hh"
#include "G4SimpleRunge.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleHeum.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

CCalDetectorConstruction::CCalDetectorConstruction() {}

CCalDetectorConstruction::~CCalDetectorConstruction() {}

G4VPhysicalVolume* CCalDetectorConstruction::Construct() {

  /////////
  //Instantiate for the first time the materials and rotations
#ifdef debug
  G4cout << "Retrieving materials...." << G4endl;
#endif
  CCalMaterialFactory::getInstance("material.cms");

#ifdef debug
  G4cout << "Retrieving rotation matrices....." << G4endl;
#endif
  CCalRotationMatrixFactory::getInstance("rotation.cms");

  //-------------------------------------------------------------------------
  // Magnetic field
  //-------------------------------------------------------------------------

  static G4bool fieldIsInitialized = false;
  //And finally that it was not initialized previously
  if (!fieldIsInitialized) {
    CCalMagneticField* ccalField=new CCalMagneticField("fmap.tb96");
    G4double field = ccalField->GetConstantFieldvalue();
    if (field == 0) {
      ccalField = NULL;
      G4cout << "***************************" << G4endl
	     << "*                         *" << G4endl
	     << "*  Magnetic Field is off  *" << G4endl
	     << "*                         *" << G4endl
	     << "***************************" << G4endl;
    } else {
      G4cout << "***************************" << G4endl
	     << "*                         *" << G4endl
	     << "*  Magnetic Field is on   *" << G4endl
	     << "*                         *" << G4endl
	     << "***************************" << G4endl << G4endl
	     << " Field Value " << tab << field << G4endl;
    }
    G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(ccalField);
    G4Mag_UsualEqRhs *fEquation = new G4Mag_UsualEqRhs(ccalField); 

    G4MagIntegratorStepper *pStepper = new G4ClassicalRK4 (fEquation);
    //pStepper = new G4ExplicitEuler( fEquation );
    //pStepper = new G4ImplicitEuler( fEquation );      
    //pStepper = new G4SimpleRunge( fEquation );        
    //pStepper = new G4SimpleHeum( fEquation );         
    //pStepper = new G4HelixExplicitEuler( fEquation ); 
    //pStepper = new G4HelixImplicitEuler( fEquation ); 
    //pStepper = new G4HelixSimpleRunge( fEquation );   
    //pStepper = new G4CashKarpRKF45( fEquation );      
    //pStepper = new G4RKG3_Stepper( fEquation );       

    G4ChordFinder *pChordFinder = new G4ChordFinder(ccalField,
                                                    1.e-1*mm, pStepper);
    pChordFinder->SetDeltaChord(1.0e-3*mm);
    fieldMgr->SetChordFinder(pChordFinder);
    fieldMgr->SetDeltaOneStep(1.0e-3*mm);
    fieldMgr->SetDeltaIntersection(1.0e-4*mm);
    G4PropagatorInField* fieldPropagator
      = G4TransportationManager::GetTransportationManager()
      ->GetPropagatorInField();
    fieldPropagator->SetMinimumEpsilonStep(1.e-5*mm);
    fieldPropagator->SetMaximumEpsilonStep(1.e-2*mm);
    fieldIsInitialized = true;
  }

#ifdef debug
  G4cout << tab << "CCalDetectorConstruction: Starting timer!!!" 
         << G4endl;
  G4Timer timer;
  timer.Start();
#endif

  //HCAL Test Beam 96
  CCalG4Hall*  testBeamHCal96 = new CCalG4Hall("HcalTB96");
  testBeamHCal96->constructHierarchy();
#ifdef debug
  timer.Stop();
  G4cout << tab << "CCalDetectorConstruction: Total time to "
         << "construct the geometry: " << timer << G4endl;
#endif //debug
  G4VPhysicalVolume* volume = testBeamHCal96->PhysicalVolume(0);

  //Addsenistive detector types 
  G4bool result;
  G4int sensitive;
  sensitive = CCalSensitiveConfiguration::getInstance()->
    getSensitiveFlag("HadronCalorimeter");
  if (sensitive>0) result = CCalSensAssign::getInstance()->
		     addCaloSD("HadronCalorimeter", new CCalHcalOrganization);
  sensitive = CCalSensitiveConfiguration::getInstance()->
    getSensitiveFlag("CrystalMatrixModule");
  if (sensitive>0) result = CCalSensAssign::getInstance()->
		     addCaloSD("CrystalMatrix", new CCalEcalOrganization);

  //Assign the sensitive detectors
  result = CCalSensAssign::getInstance()->assign();

  //Create the stacking manager required by Calorimeter
  result = CCalSensAssign::getInstance()->stackingAction();
  
  return volume;

}