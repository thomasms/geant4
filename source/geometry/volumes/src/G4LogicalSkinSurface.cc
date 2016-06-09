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
// $Id: G4LogicalSkinSurface.cc,v 1.13 2006/06/29 18:57:59 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// --------------------------------------------------------------------
// G4LogicalSkinSurface Implementation
// --------------------------------------------------------------------
//
// A Logical Surface class for the surface surrounding a single
// logical volume.
//
// Created:     1997-06-26
// Author:      John Apostolakis (John.Apostolakis@cern.ch)
//
// ----------------------------------------------------------------------

#include "G4LogicalSkinSurface.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"

G4LogicalSkinSurfaceTable G4LogicalSkinSurface::theSurfaceTable;

//
// Constructors & destructor
//

G4LogicalSkinSurface::G4LogicalSkinSurface(const G4String&   name,
                                           G4LogicalVolume*  logicalVolume,
                                           G4SurfaceProperty* surfaceProperty)
  : G4LogicalSurface(name, surfaceProperty),
    LogVolume(logicalVolume)
{
  // Store in the table of Surfaces
  //
  theSurfaceTable.push_back(this);
}

G4LogicalSkinSurface::G4LogicalSkinSurface(const G4LogicalSkinSurface& right)
  : G4LogicalSurface(right.GetName(), right.GetSurfaceProperty())
{
  SetTransitionRadiationSurface(right.GetTransitionRadiationSurface());
  LogVolume = right.LogVolume;
  theSurfaceTable = right.theSurfaceTable;
}

G4LogicalSkinSurface::~G4LogicalSkinSurface()
{
}

//
// Operators
//

const G4LogicalSkinSurface&
G4LogicalSkinSurface::operator=(const G4LogicalSkinSurface& right)
{
  if (&right == this) return *this;
  if (&right)
  {
    SetSurfaceProperty(right.GetSurfaceProperty());
    SetName(right.GetName());
    SetTransitionRadiationSurface(right.GetTransitionRadiationSurface());
    LogVolume = right.LogVolume;
    theSurfaceTable = right.theSurfaceTable;
  }
  return *this;
}

G4int
G4LogicalSkinSurface::operator==(const G4LogicalSkinSurface& right) const
{
  return (this == (G4LogicalSkinSurface *) &right);
}

G4int
G4LogicalSkinSurface::operator!=(const G4LogicalSkinSurface& right) const
{
  return (this != (G4LogicalSkinSurface *) &right);
}

//
// Methods
//

size_t G4LogicalSkinSurface::GetNumberOfSkinSurfaces()
{
  return theSurfaceTable.size();
}

G4LogicalSkinSurface*
G4LogicalSkinSurface::GetSurface(const G4LogicalVolume* vol)
{
  for (size_t i=0; i<theSurfaceTable.size(); i++)
  {
    if(theSurfaceTable[i]->GetLogicalVolume() == vol)
      return theSurfaceTable[i];
  }
  return NULL;
}

// Dump info for known surfaces
//
void G4LogicalSkinSurface::DumpInfo() 
{
    G4cout << "***** Surface Table : Nb of Surfaces = "
           << GetNumberOfSkinSurfaces() << " *****" << G4endl;

    for (size_t i=0; i<theSurfaceTable.size(); i++)
    {
      G4LogicalSkinSurface* pSkinSurface = theSurfaceTable[i];
      G4cout << theSurfaceTable[i]->GetName() << " : " << G4endl
             << " Skin of logical volume "
             << pSkinSurface->GetLogicalVolume()->GetName()
             << G4endl;
    }
    G4cout << G4endl;
}

void G4LogicalSkinSurface::CleanSurfaceTable()
{  
  std::vector<G4LogicalSkinSurface*>::iterator pos;
  for(pos=theSurfaceTable.begin(); pos!=theSurfaceTable.end(); pos++)
  {
    if (*pos) delete *pos;
  }
  theSurfaceTable.clear();
}
