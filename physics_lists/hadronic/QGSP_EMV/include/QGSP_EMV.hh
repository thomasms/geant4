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
// $Id: QGSP_EMV.hh,v 1.3 2006/06/29 18:04:28 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
//---------------------------------------------------------------------------
//
// ClassName: QGSP_EMV
//
// Author: 2005 V.Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//

#ifndef TQGSP_EMV_h
#define TQGSP_EMV_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "CompileTimeConstraints.hh"

template<class T>
class TQGSP_EMV: public T
{
public:
  TQGSP_EMV(G4int ver = 1);
  virtual ~TQGSP_EMV();
  
public:
  // SetCuts() 
  virtual void SetCuts();

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };
};
#include "QGSP_EMV.icc"
typedef TQGSP_EMV<G4VModularPhysicsList> QGSP_EMV;

// 2002 by J.P. Wellisch

#endif



