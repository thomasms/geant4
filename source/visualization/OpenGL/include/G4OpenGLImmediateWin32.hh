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
// $Id: G4OpenGLImmediateWin32.hh,v 1.7 2006/06/29 21:17:44 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 
// OpenGLImmediateWin32 graphics system factory.

#if defined (G4VIS_BUILD_OPENGLWIN32_DRIVER) || defined (G4VIS_USE_OPENGLWIN32)

#ifndef G4OPENGLIMMEDIATEWIN32_HH
#define G4OPENGLIMMEDIATEWIN32_HH

#include "G4VGraphicsSystem.hh"

class G4OpenGLImmediateWin32: public G4VGraphicsSystem {
public:
  G4OpenGLImmediateWin32 ();
  G4VSceneHandler* CreateSceneHandler (const G4String& name = "");
  G4VViewer*  CreateViewer  (G4VSceneHandler&, const G4String& name = "");
};

#endif

#endif
