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
// $Id: G4MaterialPropertiesTable.hh,v 1.18 2006/06/29 19:11:15 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 
////////////////////////////////////////////////////////////////////////
// G4MaterialPropertiesTable Definition
////////////////////////////////////////////////////////////////////////
//
// File:        G4MaterialPropertiesTable.hh
// Description: An Material properties table is a hash table, with 
// 		key = property name, and value either G4double or
//              G4MaterialPropertyVector 
// Version:     1.0
// Created:     1996-02-08
// Author:      Juliet Armstrong
// Updated:     2005-05-12 add SetGROUPVEL() by P. Gumplinger
//              2002-11-05 add named material constants by P. Gumplinger
//              1999-11-05 Migration from G4RWTPtrHashDictionary to STL
//                         by John Allison
//              1999-10-29 add method and class descriptors
//              1997-03-25 by Peter Gumplinger
//              > cosmetics (only)
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#ifndef G4MaterialPropertiesTable_h
#define G4MaterialPropertiesTable_h 1

/////////////
// Includes
/////////////

#include <cmath>
#include <map>
#include "globals.hh"
#include "G4MaterialPropertyVector.hh"

// Class Description:
// An Material properties table is a hash table, with key = property
// name, and value either G4double or G4MaterialPropertyVector.
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

class G4MaterialPropertiesTable {

        ////////////////
        // Constructor
        ////////////////

public: // Without description

	G4MaterialPropertiesTable(); 
	
	///////////////
	// Destructor
	///////////////

public: // Without description

	~G4MaterialPropertiesTable();

	////////////
	// Methods
	////////////

public: // With description

        void AddConstProperty(const char     *key,
                              G4double PropertyValue);
        // Add a new property to the table by giving a key-name and value 

	void AddProperty(const char     *key,
		         G4double *PhotonMomenta,
		         G4double *PropertyValues,
		         G4int     NumEntries);
        // Add a new property to the table by giving a key-name and the
        // arrays x and y of size NumEntries.

	void AddProperty(const char *key, G4MaterialPropertyVector *opv);
        // Add a new property to the table by giving a key-name and an
        // already constructed G4MaterialPropertyVector.

        void RemoveConstProperty(const char *key);
        // Remove a constant property from the table.

	void RemoveProperty(const char *key);
        // Remove a property from the table.

        G4double GetConstProperty(const char *key);
        // Get the constant property from the table corresponding to the key-name

        G4bool ConstPropertyExists(const char *key);
        // Return true if a const property 'key' exists.

	G4MaterialPropertyVector* GetProperty(const char *key);
        // Get the property from the table corresponding to the key-name.

	void AddEntry(const char *key, G4double aPhotonMomentum,
                                 G4double  aPropertyValue);
        // Add a new entry (pair of numbers) to the table for a given key.

	void RemoveEntry(const char *key, G4double  aPhotonMomentum);
        // Remove an entry from the table for a given key and x-value.

	void DumpTable();

private:

	/////////////////////////
	// Private Data members
	/////////////////////////

	std::map<G4String, G4MaterialPropertyVector*, std::less<G4String> > MPT;
	typedef std::map<G4String, G4MaterialPropertyVector*,
	  std::less<G4String> >::iterator MPTiterator;

        std::map< G4String, G4double, std::less<G4String> > MPTC;
        typedef std::map< G4String, G4double,
          std::less<G4String> >::iterator MPTCiterator;

        G4MaterialPropertyVector* SetGROUPVEL();

};

#endif /* G4MaterialPropertiesTable_h */
