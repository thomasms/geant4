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
// -------------------------------------------------------------------
// $Id: MicrobeamEMField.hh,v 1.5 2006/06/29 16:05:03 gunter Exp $
// -------------------------------------------------------------------

#ifndef MicrobeamEMField_h
#define MicrobeamEMField_h 1

#include "globals.hh"
#include "G4ElectroMagneticField.hh"

class MicrobeamEMField
#ifndef STANDALONE
 : public G4ElectroMagneticField
#endif

{
  
public:
  MicrobeamEMField();
  void  GetFieldValue( const  double Point[4], double *Bfield ) const;
		       
  G4bool DoesFieldChangeEnergy() const {return true;}

};

#endif
