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
// $Id: G4RayTracerViewer.cc,v 1.16 2006/06/29 21:24:15 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $

#include "G4RayTracerViewer.hh"

#include "G4ios.hh"
#include <sstream>

#include "G4VSceneHandler.hh"
#include "G4Scene.hh"
#include "G4TheRayTracer.hh"
#include "G4UImanager.hh"

G4RayTracerViewer::G4RayTracerViewer
(G4VSceneHandler& sceneHandler,
 const G4String& name,
 G4TheRayTracer* aTracer):
  G4VViewer(sceneHandler, sceneHandler.IncrementViewCount(), name),
  fFileCount(0)
{
  theTracer = aTracer;
  if (!aTracer) theTracer = new G4TheRayTracer;
  theTracer->SetNColumn(fVP.GetWindowSizeHintX());
  theTracer->SetNRow(fVP.GetWindowSizeHintY());
}

G4RayTracerViewer::~G4RayTracerViewer() {}

void G4RayTracerViewer::SetView()
{
  // Get radius of scene, etc.  (See G4OpenGLViewer::SetView().)
  // Note that this procedure properly takes into account zoom, dolly and pan.
  const G4Point3D& targetPoint
    = fSceneHandler.GetScene()->GetStandardTargetPoint()
    + fVP.GetCurrentTargetPoint();
  G4double radius =  // See G4ViewParameters for following procedure.
    fSceneHandler.GetScene()->GetExtent().GetExtentRadius();
  if(radius<=0.) radius = 1.;
  const G4double cameraDistance = fVP.GetCameraDistance(radius);
  const G4Point3D cameraPosition =
    targetPoint + cameraDistance * fVP.GetViewpointDirection().unit();
  const G4double nearDistance  = fVP.GetNearDistance(cameraDistance,radius);
  const G4double frontHalfHeight = fVP.GetFrontHalfHeight(nearDistance,radius);
  const G4double frontHalfAngle = std::atan(frontHalfHeight / nearDistance);

  // Calculate and set ray tracer parameters.
  theTracer->
    SetViewSpan(200. * frontHalfAngle / theTracer->GetNColumn());
  theTracer->SetTargetPosition(targetPoint);
  theTracer->SetEyePosition(cameraPosition);
  theTracer->SetHeadAngle(fVP.GetViewpointDirection().phi());
  const G4Vector3D
    actualLightpointDirection(-fVP.GetActualLightpointDirection());
  theTracer->SetLightDirection(actualLightpointDirection);
  theTracer->SetBackgroundColour(fVP.GetBackgroundColour());
}


void G4RayTracerViewer::ClearView() {}

void G4RayTracerViewer::DrawView()
{
  if (fVP.GetFieldHalfAngle() == 0.) { // Orthogonal (parallel) projection.
    G4double fieldHalfAngle = perMillion;
    fVP.SetFieldHalfAngle(fieldHalfAngle);
    G4cout << 
      "WARNING: G4RayTracerViewer::DrawView: true orthogonal projection"
      "\n  not yet implemented.  Doing a \"long shot\", i.e., a perspective"
      "\n  projection with a half field angle of "
	   << fieldHalfAngle <<
      " radians."
	   << G4endl;
    SetView();  // Special graphics system - bypass ProcessView().
    fVP.SetFieldHalfAngle(0.);
  }
  else {
    SetView();  // Special graphics system - bypass ProcessView().
  }
  std::ostringstream filename;
  filename << "g4RayTracer." << fShortName << '_' << fFileCount++ << ".jpeg";
  theTracer->Trace(filename.str());
}
