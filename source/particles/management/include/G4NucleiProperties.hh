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
// $Id: G4NucleiProperties.hh,v 1.14 2006/06/29 19:23:54 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
// ------------------------------------------------------------
// Hadronic Process: Nuclear De-excitations by V. Lara (Oct 1998)
// Migrate into particles category by H.Kurashige (17 Nov. 98)
// Added Shell-Pairing corrections to the Cameron mass 
// excess formula by V.Lara (9 May 99)
// 

#ifndef G4NucleiProperties_h
#define G4NucleiProperties_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4NucleiPropertiesTable.hh"
#include "G4NucleiPropertiesTheoreticalTable.hh"
#include "G4ParticleTable.hh"

class G4NucleiProperties
{
 // Class Description
 //   G4NucleiProperties is an utility class to provide mass formula of nuclei
 //   (i.e. it has static member function only)


public: 

  // Destructor
  ~G4NucleiProperties() { };

  // Default constructor ()
  G4NucleiProperties(){};


public:  // With Description

	// Calculate Mass Excess of nucleus A,Z
	static G4double GetMassExcess(const G4int A, const G4int Z);

	static G4double GetAtomicMass(const G4double A, const G4double Z);
	
	static G4double GetBindingEnergy(const G4int A, const G4int Z);
	
	static G4double GetNuclearMass(const G4double A, const G4double Z);

private:

	// Calculate Mass Excess according to Cameron's liquid drop formula
//	static G4double CameronMassExcess(const G4int A, const G4int Z);

	static G4double  AtomicMass(G4double A, G4double Z);
	
	static G4double BindingEnergy(G4double A, G4double Z);

	static G4double MassExcess(G4double A, G4double Z);
	
	
};



#endif








