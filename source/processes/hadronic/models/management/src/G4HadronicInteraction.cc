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
 // Hadronic Interaction  base class
 // original by H.P. Wellisch
 // modified by J.L. Chuma, TRIUMF, 21-Mar-1997
 // Last modified: 04-Apr-1997
 // reimplemented 1.11.2003 JPW.
 
#include "G4HadronicInteraction.hh"
 
 G4double
  G4HadronicInteraction::GetMinEnergy(
   const G4Material *aMaterial, const G4Element *anElement ) const
  {
    size_t i;
    if( IsBlocked(aMaterial) )return 0.*GeV;
    if( IsBlocked(anElement) )return 0.*GeV;
    for( i=0; i<theMinEnergyListElements.size(); ++i )
    {
      if( anElement == theMinEnergyListElements[i].second )return theMinEnergyListElements[i].first;
    }
    for( i=0; i<theMinEnergyList.size(); ++i )
    {
      if( aMaterial == theMinEnergyList[i].second )return theMinEnergyList[i].first;
    }
    if(IsBlocked()) return 0.*GeV;
    if( verboseLevel > 0 )
      G4cout << "*** Warning from HadronicInteraction::GetMinEnergy" << G4endl
           << "    material " << aMaterial->GetName()
           << " not found in min energy List" << G4endl;
    return theMinEnergy;
  }
 
 void
  G4HadronicInteraction::SetMinEnergy(
   G4double anEnergy,
   G4Element *anElement )
  {
    if( IsBlocked(anElement) )
      G4cout << "*** Warning from HadronicInteraction::SetMinEnergy" << G4endl
           << "    The model is not active for the Element  "
           << anElement->GetName() << "." << G4endl;
    
    for( size_t i=0; i<theMinEnergyListElements.size(); ++i )
    {
      if( anElement == theMinEnergyListElements[i].second )
      {
        theMinEnergyListElements[i].first = anEnergy;
        return;
      }
    }
    theMinEnergyListElements.push_back(std::pair<G4double, G4Element *>(anEnergy, anElement));
  }
 
 void
  G4HadronicInteraction::SetMinEnergy(
   G4double anEnergy,
   G4Material *aMaterial )
  {
    if( IsBlocked(aMaterial) )
      G4cout << "*** Warning from HadronicInteraction::SetMinEnergy" << G4endl
           << "    The model is not active for the Material "
           << aMaterial->GetName() << "." << G4endl;
    
    for( size_t i=0; i<theMinEnergyList.size(); ++i )
    {
      if( aMaterial == theMinEnergyList[i].second )
      {
        theMinEnergyList[i].first = anEnergy;
        return;
      }
    }
    theMinEnergyList.push_back(std::pair<G4double, G4Material *>(anEnergy, aMaterial));
  }
 
 G4double
  G4HadronicInteraction::GetMaxEnergy(
   const G4Material *aMaterial, const G4Element *anElement ) const
  {
    size_t i;
    if( IsBlocked(aMaterial) )return 0.0*GeV;
    if( IsBlocked(anElement) )return 0.0*GeV;
    for( i=0; i<theMaxEnergyListElements.size(); ++i )
    {
      if( anElement == theMaxEnergyListElements[i].second )return theMaxEnergyListElements[i].first;
    }
    for( i=0; i<theMaxEnergyList.size(); ++i )
    {
      if( aMaterial == theMaxEnergyList[i].second )return theMaxEnergyList[i].first;
    }
    if(IsBlocked()) return 0.*GeV;
    if( verboseLevel > 0 )
      G4cout << "*** Warning from HadronicInteraction::GetMaxEnergy" << G4endl
           << "    material " << aMaterial->GetName()
           << " not found in min energy List" << G4endl;
    
    return theMaxEnergy;
  }
 
 void
  G4HadronicInteraction::SetMaxEnergy(
   G4double anEnergy,
   G4Element *anElement ) 
  {
    if( IsBlocked(anElement) )
      G4cout << "*** Warning from HadronicInteraction::SetMaxEnergy" << G4endl
           << "Warning: The model is not active for the Element  "
           << anElement->GetName() << "." << G4endl;
    
    for( size_t i=0; i<theMaxEnergyListElements.size(); ++i )
    {
      if( anElement == theMaxEnergyListElements[i].second )
      {
        theMaxEnergyListElements[i].first = anEnergy;
        return;
      }
    }
    theMaxEnergyListElements.push_back(std::pair<G4double, G4Element *>(anEnergy, anElement));
  }

 void
  G4HadronicInteraction::SetMaxEnergy(
   G4double anEnergy,
   G4Material *aMaterial )
  {
    if( IsBlocked(aMaterial) ) 
      G4cout << "*** Warning from HadronicInteraction::SetMaxEnergy" << G4endl
           << "Warning: The model is not active for the Material "
           << aMaterial->GetName() << "." << G4endl;
    
    for( size_t i=0; i<theMaxEnergyList.size(); ++i )
    {
      if( aMaterial == theMaxEnergyList[i].second )
      {
        theMaxEnergyList[i].first = anEnergy;
        return;
      }
    }
    theMaxEnergyList.push_back(std::pair<G4double, G4Material *>(anEnergy, aMaterial));
  }

 void 
  G4HadronicInteraction::DeActivateFor( G4Material *aMaterial )
  {
    theBlockedList.push_back(aMaterial);
  }

 void 
  G4HadronicInteraction::DeActivateFor( G4Element *anElement )
  {
    theBlockedListElements.push_back(anElement);
  }

 G4bool 
  G4HadronicInteraction::IsBlocked( const G4Material *aMaterial ) const
  {
    for( size_t i=0; i<theBlockedList.size(); ++i )
    {
      if( aMaterial == theBlockedList[i] )
      {
        return true;
      }
    }
    return false;
  }
 
 G4bool 
  G4HadronicInteraction::IsBlocked( const G4Element *anElement ) const
  {
    for( size_t i=0; i<theBlockedListElements.size(); ++i )
    {
      if( anElement == theBlockedListElements[i] )
      {
        return true;
      }
    }
    return false;
  }
 
 /* end of file */
 
