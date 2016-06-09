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
// $Id: G4XXXSGSceneHandler.cc,v 1.3 2006/06/29 21:27:39 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $
//
// 
// John Allison  10th March 2006
// A template for a sophisticated graphics driver with a scene graph.
//?? Lines beginning like this require specialisation for your driver.

#ifdef G4VIS_BUILD_XXXSG_DRIVER

#include "G4XXXSGSceneHandler.hh"

#include "G4XXXSGViewer.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Polyline.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Polyhedron.hh"
#include "G4NURBS.hh"
#include "G4VTrajectory.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"

#include <sstream>

G4int G4XXXSGSceneHandler::fSceneIdCount = 0;
// Counter for XXX scene handlers.

G4XXXSGSceneHandler::G4XXXSGSceneHandler(G4VGraphicsSystem& system,
					     const G4String& name):
  G4VSceneHandler(system, fSceneIdCount++, name)
{
  fRoot = fSceneGraph.setRoot("root");
  fPermanentsRoot = fRoot.push_back("permanentsRoot");
  fTransientsRoot = fRoot.push_back("transientsRoot");
}

G4XXXSGSceneHandler::~G4XXXSGSceneHandler() {}

#ifdef G4XXXSGDEBUG
// Useful function...
void G4XXXSGSceneHandler::PrintThings() {
  G4cout <<
    "  with transformation "
         << (void*)fpObjectTransformation;
  if (fpModel) {
    G4cout << " from " << fpModel->GetCurrentDescription()
	   << " (tag " << fpModel->GetCurrentTag()
	   << ')';
  } else {
    G4cout << "(not from a model)";
  }
  G4PhysicalVolumeModel* pPVModel =
    dynamic_cast<G4PhysicalVolumeModel*>(fpModel);
  if (pPVModel) {
    G4cout <<
      "\n  current physical volume: "
           << pPVModel->GetCurrentPV()->GetName() <<
      "\n  current logical volume: "
           << pPVModel->GetCurrentLV()->GetName() <<
      "\n  current depth of geometry tree: "
           << pPVModel->GetCurrentDepth();
  }
  G4cout << G4endl;
}
#endif

void G4XXXSGSceneHandler::CreateCurrentItem(const G4String& header) {
  // Utility for PreAddSolid and BeginPrimitives.
  
  G4PhysicalVolumeModel* pPVModel =
    dynamic_cast<G4PhysicalVolumeModel*>(fpModel);
  
  if (pPVModel) {

    // This call comes from a G4PhysicalVolumeModel.  drawnPVPath is
    // the path of the current drawn (non-culled) volume in terms of
    // drawn (non-culled) ancesters.  Each node is identified by a
    // PVNodeID object, which is a physical volume and copy number.  It
    // is a vector of PVNodeIDs corresponding to the geometry hierarchy
    // actually selected, i.e., not culled.
    typedef G4PhysicalVolumeModel::G4PhysicalVolumeNodeID PVNodeID;
    typedef std::vector<PVNodeID> PVPath;
    const PVPath& drawnPVPath = pPVModel->GetDrawnPVPath();
    //G4int currentDepth = pPVModel->GetCurrentDepth();
    //G4VPhysicalVolume* pCurrentPV = pPVModel->GetCurrentPV();
    G4LogicalVolume* pCurrentLV = pPVModel->GetCurrentLV();
    //G4Material* pCurrentMaterial = pPVModel->GetCurrentMaterial();

    // The simplest algorithm, used by the Open Inventor Driver
    // developers, is to rely on the fact the G4PhysicalVolumeModel
    // traverses the geometry hierarchy in an orderly manner.  The last
    // mother, if any, will be the node to which the volume should be
    // added.  So it is enough to keep a map of scene graph nodes keyed
    // on the volume path ID.  Actually, it is enough to use the logical
    // volume as the key.  (An alternative would be to keep the PVNodeID
    // in the tree and match the PVPath from the root down.)

    // Find mother.  ri points to drawn mother, if any.
    PVPath::const_reverse_iterator ri = ++drawnPVPath.rbegin();
    if (ri != drawnPVPath.rend()) {
      // This volume has a mother.
      G4LogicalVolume* drawnMotherLV =
	ri->GetPhysicalVolume()->GetLogicalVolume();
      LVMapIterator mother = fLVMap.find(drawnMotherLV);
      if (mother != fLVMap.end()) {
	// This adds a child in Troy's tree...
	fCurrentItem = mother->second.push_back(header);
      } else {
	// Mother not previously encountered.  Shouldn't happen, since
	// G4PhysicalVolumeModel sends volumes as it encounters them,
	// i.e., mothers before daughters, in its descent of the
	// geometry tree.  Error!
	G4cout << "ERROR: G4XXXSGSceneHandler::PreAddSolid: Mother "
	       << ri->GetPhysicalVolume()->GetName()
	       << ':' << ri->GetCopyNo()
	       << " not previously encountered."
	  "\nShouldn't happen!  Please report to visualization coordinator."
	       << G4endl;
	// Continue anyway.  Add to root of scene graph tree...
	fCurrentItem = fPermanentsRoot.push_back(header);
      }
    } else {
      // This volume has no mother.  Must be a top level un-culled
      // volume.  Add to root of scene graph tree...
      fCurrentItem = fPermanentsRoot.push_back(header);
    }

    std::ostringstream oss;
    oss << "Path of drawn PVs: ";
    for (PVPath::const_iterator i = drawnPVPath.begin();
	 i != drawnPVPath.end(); ++i) {
      oss << '/' << i->GetPhysicalVolume()->GetName()
	  << ':' << i->GetCopyNo();
    }
    oss << std::endl;
    *fCurrentItem += oss.str();

    // Store for future searches.  Overwrites previous entries for this
    // LV, so entry is always the *last* LV.
    fLVMap[pCurrentLV] = fCurrentItem;

  } else {  // Not from a G4PhysicalVolumeModel.

    // Create a place for current solid in root...
    if (fReadyForTransients) {
      fCurrentItem = fTransientsRoot.push_back(header);
    } else {
      fCurrentItem = fPermanentsRoot.push_back(header);
    }
  }
}

void G4XXXSGSceneHandler::PreAddSolid
(const G4Transform3D& objectTransformation,
 const G4VisAttributes& visAttribs)
{  
  G4VSceneHandler::PreAddSolid(objectTransformation, visAttribs);
  CreateCurrentItem(G4String("\nPreAddSolid:\n"));
}

void G4XXXSGSceneHandler::PostAddSolid()
{
  *fCurrentItem += "EndSolid\n";
  G4VSceneHandler::PostAddSolid();
}

void G4XXXSGSceneHandler::BeginPrimitives
(const G4Transform3D& objectTransformation)
{
  G4VSceneHandler::BeginPrimitives(objectTransformation);

  // If thread of control has already passed through PreAddSolid,
  // avoid opening a graphical data base component again.
  if (!fProcessingSolid) {
    CreateCurrentItem(G4String("\nBeginPrimitives:\n"));
  }
}

void G4XXXSGSceneHandler::EndPrimitives ()
{
  if (!fProcessingSolid) {  // Already done if so.
    *fCurrentItem += "EndPrimitives\n";
  }
  G4VSceneHandler::EndPrimitives ();
}

// Note: This function overrides G4VSceneHandler::AddSolid(const
// G4Box&).  You may not want to do this, but this is how it's done if
// you do.  Certain other specific solids may be treated this way -
// see G4VSceneHandler.hh.  The simplest possible driver would *not*
// implement these polymorphic functions, with the effect that the
// default versions in G4VSceneHandler are used, which simply call
// G4VSceneHandler::RequestPrimitives to turn the solid into a
// G4Polyhedron usually.
// Don't forget, solids can be transients too (e.g., representing a hit).
void G4XXXSGSceneHandler::AddSolid(const G4Box& box) {
#ifdef G4XXXSGDEBUG
  G4cout <<
    "G4XXXSGSceneHandler::AddSolid(const G4Box& box) called for "
	 << box.GetName()
	 << G4endl;
#endif
  //?? Process your box...
  std::ostringstream oss;
  oss << "G4Box(" <<
    G4String
    (G4BestUnit
     (G4ThreeVector
      (box.GetXHalfLength(), box.GetYHalfLength(), box.GetZHalfLength()),
      "Length")).strip() << ')' << std::endl;
  *fCurrentItem += oss.str();
}

void G4XXXSGSceneHandler::AddPrimitive(const G4Polyline& polyline) {
#ifdef G4XXXSGDEBUG
  G4cout <<
    "G4XXXSGSceneHandler::AddPrimitive(const G4Polyline& polyline) called.\n"
	 << polyline
	 << G4endl;
#endif
  // Get vis attributes - pick up defaults if none.
  //const G4VisAttributes* pVA =
  //  fpViewer -> GetApplicableVisAttributes (polyline.GetVisAttributes ());
  //?? Process polyline.
  std::ostringstream oss;
  oss << polyline << std::endl;
  *fCurrentItem += oss.str();
}

void G4XXXSGSceneHandler::AddPrimitive(const G4Text& text) {
#ifdef G4XXXSGDEBUG
  G4cout <<
    "G4XXXSGSceneHandler::AddPrimitive(const G4Text& text) called.\n"
	 << text
	 << G4endl;
#endif
  // Get text colour - special method since default text colour is
  // determined by the default text vis attributes, which may be
  // specified independent of default vis attributes of other types of
  // visible objects.
  //const G4Colour& c = GetTextColour (text);  // Picks up default if none.
  //?? Process text.
  std::ostringstream oss;
  oss << text << std::endl;
  *fCurrentItem += oss.str();
}

void G4XXXSGSceneHandler::AddPrimitive(const G4Circle& circle) {
#ifdef G4XXXSGDEBUG
  G4cout <<
    "G4XXXSGSceneHandler::AddPrimitive(const G4Circle& circle) called.\n"
	 << circle
	 << G4endl; 
  MarkerSizeType sizeType;
  G4double size = GetMarkerSize (circle, sizeType);
  switch (sizeType) {
  default:
  case screen:
    // Draw in screen coordinates.
    G4cout << "screen";
    break;
  case world:
    // Draw in world coordinates.
    G4cout << "world";
    break;
  }
  G4cout << " size: " << size << G4endl;
#endif
  // Get vis attributes - pick up defaults if none.
  //const G4VisAttributes* pVA =
  //  fpViewer -> GetApplicableVisAttributes (circle.GetVisAttributes ());
  //?? Process circle.
  std::ostringstream oss;
  oss << circle << std::endl;
  *fCurrentItem += oss.str();
}

void G4XXXSGSceneHandler::AddPrimitive(const G4Square& square) {
#ifdef G4XXXSGDEBUG
  G4cout <<
    "G4XXXSGSceneHandler::AddPrimitive(const G4Square& square) called.\n"
	 << square
	 << G4endl;
  MarkerSizeType sizeType;
  G4double size = GetMarkerSize (square, sizeType);
  switch (sizeType) {
  default:
  case screen:
    // Draw in screen coordinates.
    G4cout << "screen";
    break;
  case world:
    // Draw in world coordinates.
    G4cout << "world";
    break;
  }
  G4cout << " size: " << size << G4endl;
#endif
  // Get vis attributes - pick up defaults if none.
  //const G4VisAttributes* pVA =
  //  fpViewer -> GetApplicableVisAttributes (square.GetVisAttributes ());
  //?? Process square.
  std::ostringstream oss;
  oss << square << std::endl;
  *fCurrentItem += oss.str();
}

void G4XXXSGSceneHandler::AddPrimitive(const G4Polyhedron& polyhedron) {
#ifdef G4XXXSGDEBUG
  G4cout <<
 "G4XXXSGSceneHandler::AddPrimitive(const G4Polyhedron& polyhedron) called.\n"
	 << polyhedron
	 << G4endl;
#endif
  //?? Process polyhedron.
  std::ostringstream oss;
  oss << polyhedron;
  *fCurrentItem += oss.str();

  //?? Or... here are some ideas for decomposing into polygons...
  //Assume all facets are convex quadrilaterals.
  //Draw each G4Facet individually
  
  //Get colour, etc..
  if (polyhedron.GetNoFacets() == 0) return;

  // Get vis attributes - pick up defaults if none.
  const G4VisAttributes* pVA =
    fpViewer -> GetApplicableVisAttributes (polyhedron.GetVisAttributes ());

  // Get view parameters that the user can force through the vis
  // attributes, thereby over-riding the current view parameter.
  G4ViewParameters::DrawingStyle drawing_style = GetDrawingStyle (pVA);
  //G4bool isAuxEdgeVisible = GetAuxEdgeVisible (pVA);
  
  //Get colour, etc..
  //const G4Colour& c = pVA -> GetColour ();
  
  // Initial action depending on drawing style.
  switch (drawing_style) {
  case (G4ViewParameters::hsr):
    {
      break;
    }
  case (G4ViewParameters::hlr):
    {
      break;
    }
  case (G4ViewParameters::wireframe):
    {
      break;
    }
  default:
    {
      break;
    }     
  }

  // Loop through all the facets...

  // Look at G4OpenGLSceneHandler::AddPrimitive(const G4Polyhedron&)
  // for an example of how to get facets out of a G4Polyhedron,
  // including how to cope with triangles if that's a problem.
}

void G4XXXSGSceneHandler::AddPrimitive(const G4NURBS& nurbs) {
#ifdef G4XXXSGDEBUG
  G4cout <<
    "G4XXXSGSceneHandler::AddPrimitive(const G4NURBS& nurbs) called."
	 << G4endl;
#endif
  //?? Don't bother implementing this.  NURBS are not functional.
}

void G4XXXSGSceneHandler::ClearStore () {

  G4VSceneHandler::ClearStore ();  // Sets need kernel visit, etc.

  fPermanentsRoot.clear();
  fTransientsRoot.clear();
  fLVMap.clear();
}

void G4XXXSGSceneHandler::ClearTransientStore () {

  G4VSceneHandler::ClearTransientStore ();

  fTransientsRoot.clear();
}

#endif
