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
// $Id: G4VisCommandsScene.cc,v 1.54 2006/06/29 21:29:42 gunter Exp $
// GEANT4 tag $Name: geant4-08-01 $

// /vis/scene commands - John Allison  9th August 1998

#include "G4VisCommandsScene.hh"

#include "G4VisManager.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4ApplicationState.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"
#include <sstream>

G4VVisCommandScene::G4VVisCommandScene () {}

G4VVisCommandScene::~G4VVisCommandScene () {}

G4String G4VVisCommandScene::CurrentSceneName () {
  const G4Scene* pScene = fpVisManager -> GetCurrentScene ();
  G4String currentSceneName;
  if (pScene) currentSceneName = pScene -> GetName ();
  return currentSceneName;
}

////////////// /vis/scene/create ///////////////////////////////////////

G4VisCommandSceneCreate::G4VisCommandSceneCreate (): fId (0) {
  G4bool omitable;
  fpCommand = new G4UIcmdWithAString ("/vis/scene/create", this);
  fpCommand -> SetGuidance
    ("Creates an empty scene.");
  fpCommand -> SetGuidance
    ("Invents a name if not supplied.  This scene becomes current.");
  fpCommand -> SetParameterName ("scene-name", omitable = true);
}

G4VisCommandSceneCreate::~G4VisCommandSceneCreate () {
  delete fpCommand;
}

G4String G4VisCommandSceneCreate::NextName () {
  std::ostringstream oss;
  oss << "scene-" << fId;
  return oss.str();
}

G4String G4VisCommandSceneCreate::GetCurrentValue (G4UIcommand*) {
  return "";
}

void G4VisCommandSceneCreate::SetNewValue (G4UIcommand*, G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4String& newName = newValue;
  G4String nextName = NextName ();

  if (newName == "") {
    newName = nextName;
  }
  if (newName == nextName) fId++;

  G4SceneList& sceneList = fpVisManager -> SetSceneList ();
  G4int iScene, nScenes = sceneList.size ();
  for (iScene = 0; iScene < nScenes; iScene++) {
    if (sceneList [iScene] -> GetName () == newName) break;
  }
  if (iScene < nScenes) {
    if (verbosity >= G4VisManager::warnings) {
      G4cout << "WARNING: Scene \"" << newName << "\" already exists."
	     << G4endl;
    }
  } else {

    // Add empty scene data object to list...
    G4Scene* pScene = new G4Scene (newName);
    sceneList.push_back (pScene);
    fpVisManager -> SetCurrentScene (pScene);

    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "New empty scene \"" << newName << "\" created." << G4endl;
    }
  }
}

////////////// /vis/scene/endOfEventAction ////////////////////////////

G4VisCommandSceneEndOfEventAction::G4VisCommandSceneEndOfEventAction () {
  G4bool omitable;
  fpCommand = new G4UIcmdWithAString ("/vis/scene/endOfEventAction", this);
  fpCommand -> SetGuidance
    ("Accumulate or refresh the viewer for each new event.");
  fpCommand -> SetGuidance
    ("\"accumulate\": viewer accumulates hits, etc., event by event, or");
  fpCommand -> SetGuidance
    ("\"refresh\": viewer shows them at end of event or, for direct-screen"
     "\n  viewers, refreshes the screen just before drawing the next event.");
  fpCommand -> SetGuidance ("The detector remains or is redrawn.");
  fpCommand -> SetParameterName ("action", omitable = true);
  fpCommand -> SetCandidates ("accumulate refresh");
  fpCommand -> SetDefaultValue ("refresh");
}

G4VisCommandSceneEndOfEventAction::~G4VisCommandSceneEndOfEventAction () {
  delete fpCommand;
}

G4String G4VisCommandSceneEndOfEventAction::GetCurrentValue(G4UIcommand*) {
  return "";
}

void G4VisCommandSceneEndOfEventAction::SetNewValue (G4UIcommand*,
						     G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4String action;
  std::istringstream is (newValue);
  is >> action;

  G4Scene* pScene = fpVisManager->GetCurrentScene();
  if (!pScene) {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<	"ERROR: No current scene.  Please create one." << G4endl;
    }
    return;
  }

  G4VSceneHandler* pSceneHandler = fpVisManager->GetCurrentSceneHandler();
  if (!pSceneHandler) {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<	"ERROR: No current sceneHandler.  Please create one." << G4endl;
    }
    return;
  }

  if (action == "accumulate") {
    pScene->SetRefreshAtEndOfEvent(false);
  }
  else if (action == "refresh") {
    pScene->SetRefreshAtEndOfEvent(true);
    pSceneHandler->SetMarkForClearingTransientStore(true);
  }
  else {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<
	"ERROR: unrecognised parameter \"" << action << "\"."
             << G4endl;
    }
    return;
  }

  if (verbosity >= G4VisManager::confirmations) {
    G4cout << "End of event action set to \"";
    if (pScene->GetRefreshAtEndOfEvent()) G4cout << "refresh";
    else G4cout << "accumulate";
    G4cout << "\"" << G4endl;
  }
}

////////////// /vis/scene/endOfRunAction ////////////////////////////

G4VisCommandSceneEndOfRunAction::G4VisCommandSceneEndOfRunAction () {
  G4bool omitable;
  fpCommand = new G4UIcmdWithAString ("/vis/scene/endOfRunAction", this);
  fpCommand -> SetGuidance
    ("Accumulate or refresh the viewer for each new run.");
  fpCommand -> SetGuidance
    ("\"accumulate\": viewer accumulates hits, etc., run by run, or");
  fpCommand -> SetGuidance
    ("\"refresh\": viewer shows them at end of run or, for direct-screen"
     "\n  viewers, refreshes the screen just before drawing the first"
     "\n  event of the next run.");
  fpCommand -> SetGuidance ("The detector remains or is redrawn.");
  fpCommand -> SetParameterName ("action", omitable = true);
  fpCommand -> SetCandidates ("accumulate refresh");
  fpCommand -> SetDefaultValue ("refresh");
}

G4VisCommandSceneEndOfRunAction::~G4VisCommandSceneEndOfRunAction () {
  delete fpCommand;
}

G4String G4VisCommandSceneEndOfRunAction::GetCurrentValue(G4UIcommand*) {
  return "";
}

void G4VisCommandSceneEndOfRunAction::SetNewValue (G4UIcommand*,
						     G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4String action;
  std::istringstream is (newValue);
  is >> action;

  G4Scene* pScene = fpVisManager->GetCurrentScene();
  if (!pScene) {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<	"ERROR: No current scene.  Please create one." << G4endl;
    }
    return;
  }

  G4VSceneHandler* pSceneHandler = fpVisManager->GetCurrentSceneHandler();
  if (!pSceneHandler) {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<	"ERROR: No current sceneHandler.  Please create one." << G4endl;
    }
    return;
  }

  if (action == "accumulate") {
    if (pScene->GetRefreshAtEndOfEvent()) {
      if (verbosity >= G4VisManager::errors) {
	G4cout <<
	  "ERROR: Cannot accumulate runs unless events accumulate too."
	  "\n  Use \"/vis/scene/endOfEventAction accumulate\"."
	       << G4endl;
      }
    }
    else {
      pScene->SetRefreshAtEndOfRun(false);
    }
  }
  else if (action == "refresh") {
    pScene->SetRefreshAtEndOfRun(true);
    pSceneHandler->SetMarkForClearingTransientStore(true);
  }
  else {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<
	"ERROR: unrecognised parameter \"" << action << "\"."
             << G4endl;
    }
    return;
  }

  if (verbosity >= G4VisManager::confirmations) {
    G4cout << "End of run action set to \"";
    if (pScene->GetRefreshAtEndOfRun()) G4cout << "refresh";
    else G4cout << "accumulate";
    G4cout << "\"" << G4endl;
  }
}

////////////// /vis/scene/list ///////////////////////////////////////

G4VisCommandSceneList::G4VisCommandSceneList () {
  G4bool omitable;
  fpCommand = new G4UIcommand ("/vis/scene/list", this);
  fpCommand -> SetGuidance ("Lists scene(s).");
  fpCommand -> SetGuidance
    ("\"help /vis/verbose\" for definition of verbosity.");
  G4UIparameter* parameter;
  parameter = new G4UIparameter ("scene-name", 's', omitable = true);
  parameter -> SetDefaultValue ("all");
  fpCommand -> SetParameter (parameter);
  parameter = new G4UIparameter ("verbosity", 's', omitable = true);
  parameter -> SetDefaultValue ("warnings");
  fpCommand -> SetParameter (parameter);
}

G4VisCommandSceneList::~G4VisCommandSceneList () {
  delete fpCommand;
}

G4String G4VisCommandSceneList::GetCurrentValue (G4UIcommand*) {
  return "";
}

void G4VisCommandSceneList::SetNewValue (G4UIcommand*, G4String newValue) {
  G4String name, verbosityString;
  std::istringstream is (newValue);
  is >> name >> verbosityString;
  G4VisManager::Verbosity verbosity =
    fpVisManager->GetVerbosityValue(verbosityString);
  const G4Scene* currentScene = fpVisManager -> GetCurrentScene ();
  G4String currentName;
  if (currentScene) currentName = currentScene->GetName();

  G4SceneList& sceneList = fpVisManager -> SetSceneList ();
  G4int iScene, nScenes = sceneList.size ();
  G4bool found = false;
  for (iScene = 0; iScene < nScenes; iScene++) {
    G4Scene* pScene = sceneList [iScene];
    const G4String& iName = pScene -> GetName ();
    if (name != "all") {
      if (name != iName) continue;
    }
    found = true;
    if (iName == currentName) {
      G4cout << "  (current)";
    }
    else {
      G4cout << "           ";
    }
    G4cout << " scene \"" << iName << "\"";
    if (verbosity >= G4VisManager::confirmations) {
      G4int i;
      G4cout << "\n  Run-duration models:";
      G4int nRunModels = pScene -> GetRunDurationModelList ().size ();
      if (nRunModels == 0) {
	G4cout << " none.";
      }
      for (i = 0; i < nRunModels; i++) {
	G4VModel* pModel = pScene -> GetRunDurationModelList () [i];
	G4cout << "\n    " << pModel -> GetGlobalDescription ();
      }
      G4cout << "\n  End-of-event models:";
      G4int nEOEModels = pScene -> GetEndOfEventModelList ().size ();
      if (nEOEModels == 0) {
	G4cout << " none.";
      }
      for (i = 0; i < nEOEModels; i++) {
	G4VModel* pModel = pScene -> GetEndOfEventModelList () [i];
	G4cout << "\n    " << pModel -> GetGlobalDescription ();
      }
    }
    if (verbosity >= G4VisManager::parameters) {
      G4cout << "\n  " << *sceneList [iScene];
    }
    G4cout << G4endl;
  }
  if (!found) {
    G4cout << "No scenes found";
    if (name != "all") {
      G4cout << " of name \"" << name << "\"";
    }
    G4cout << "." << G4endl;
  }
}

////////////// /vis/scene/notifyHandlers /////////////////////////

G4VisCommandSceneNotifyHandlers::G4VisCommandSceneNotifyHandlers () {
  G4bool omitable;
  fpCommand = new G4UIcommand ("/vis/scene/notifyHandlers", this);
  fpCommand -> SetGuidance
    ("Notifies scene handlers and forces re-rendering.");
  fpCommand -> SetGuidance
    ("Notifies the handler(s) of the specified scene and forces a"
     "\nreconstruction of any graphical databases."
     "\nClears and refreshes all viewers of current scene."
     "\n  The default action \"refresh\" does not issue \"update\" (see"
     "\n    /vis/viewer/update)."
     "\nIf \"flush\" is specified, it issues an \"update\" as well as"
     "\n  \"refresh\" - \"update\" and initiates post-processing"
     "\n  for graphics systems which need it.");
  fpCommand -> SetGuidance 
    ("The default for <scene-name> is the current scene name.");
  fpCommand -> SetGuidance
    ("This command does not change current scene, scene handler or viewer.");
  G4UIparameter* parameter;
  parameter = new G4UIparameter ("scene-name", 's',
				 omitable = true);
  parameter -> SetCurrentAsDefault(true);
  fpCommand -> SetParameter (parameter);
  parameter = new G4UIparameter ("refresh-flush", 's',
				 omitable = true);
  parameter -> SetDefaultValue("refresh");
  parameter -> SetParameterCandidates("r refresh f flush");
  fpCommand -> SetParameter (parameter);
}

G4VisCommandSceneNotifyHandlers::~G4VisCommandSceneNotifyHandlers () {
  delete fpCommand;
}

G4String G4VisCommandSceneNotifyHandlers::GetCurrentValue(G4UIcommand*) {
  return CurrentSceneName ();
}

void G4VisCommandSceneNotifyHandlers::SetNewValue (G4UIcommand*,
						   G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4String sceneName, refresh_flush;
  std::istringstream is (newValue);
  is >> sceneName >> refresh_flush;
  G4bool flush = false;
  if (refresh_flush(0) == 'f') flush = true;

  const G4SceneList& sceneList = fpVisManager -> GetSceneList ();
  G4SceneHandlerList& sceneHandlerList =
    fpVisManager -> SetAvailableSceneHandlers ();

  // Check scene name.
  const G4int nScenes = sceneList.size ();
  G4int iScene;
  for (iScene = 0; iScene < nScenes; iScene++) {
    G4Scene* scene = sceneList [iScene];
    if (sceneName == scene -> GetName ()) break;
  }
  if (iScene >= nScenes ) {
    if (verbosity >= G4VisManager::warnings) {
      G4cout << "WARNING: Scene \"" << sceneName << "\" not found."
	"\n  /vis/scene/list to see scenes."
	     << G4endl;
    }
    return;
  }

  // Store current context...
  G4VSceneHandler* pCurrentSceneHandler =
    fpVisManager -> GetCurrentSceneHandler();
  G4VViewer* pCurrentViewer = fpVisManager -> GetCurrentViewer();
  G4Scene* pCurrentScene = fpVisManager -> GetCurrentScene();
  G4VisManager::Verbosity currentVerbosity = fpVisManager -> GetVerbosity();

  // Suppress messages during this process (only print errors)...
  //fpVisManager -> SetVerboseLevel(G4VisManager::errors);

  // For each scene handler, if it contains the scene, clear and
  // rebuild the graphical database, then for each viewer set (make
  // current), clear, (re)draw, and show.
  const G4int nSceneHandlers = sceneHandlerList.size ();
  for (G4int iSH = 0; iSH < nSceneHandlers; iSH++) {
    G4VSceneHandler* aSceneHandler = sceneHandlerList [iSH];
    G4Scene* aScene = aSceneHandler -> GetScene ();
    if (aScene) {
      const G4String& aSceneName = aScene -> GetName ();
      if (sceneName == aSceneName) {
	// Clear store and force a rebuild of graphical database...
	//aSceneHandler -> ClearStore (); // Not nec??  Done below
	//with NeedKernelVisit and DrawView.  JA.
	G4ViewerList& viewerList = aSceneHandler -> SetViewerList ();
	const G4int nViewers = viewerList.size ();
	for (G4int iV = 0; iV < nViewers; iV++) {
	  G4VViewer* aViewer = viewerList [iV];
	  aSceneHandler -> SetCurrentViewer (aViewer);
	  // Ensure consistency of vis manager...
	  fpVisManager -> SetCurrentViewer(aViewer);
	  fpVisManager -> SetCurrentSceneHandler(aSceneHandler);
	  fpVisManager -> SetCurrentScene(aScene);
	  // Re-draw, forcing rebuild of graphics database, if any...
	  aViewer -> NeedKernelVisit();
	  aViewer -> SetView ();
	  aViewer -> ClearView ();
	  aViewer -> DrawView ();
	  if (flush) aViewer -> ShowView ();
	  if (verbosity >= G4VisManager::confirmations) {
	    G4cout << "Viewer \"" << aViewer -> GetName ()
		   << "\" of scene handler \"" << aSceneHandler -> GetName ()
		   << "\"\n  ";
	    if (flush) G4cout << "flushed";
	    else G4cout << "refreshed";
	    G4cout << " at request of scene \"" << sceneName
		   << "\"." << G4endl;
	  }
	}
      }
    }
    else {
      if (verbosity >= G4VisManager::warnings) {
	G4cout << "WARNING: G4VisCommandSceneNotifyHandlers: scene handler \""
	       << aSceneHandler->GetName()
	       << "\" has a null scene."
	       << G4endl;
      }
    }
  }

  // Reclaim original context - but set viewer first, then scene
  // handler, because the latter might have been created very recently
  // and, not yet having a viewer, the current viewer will,
  // temporarily, refer to another scene handler.  SetCurrentViewer
  // actually resets the scene handler, which is what we don't want,
  // so we set it again on the next line...
  fpVisManager -> SetCurrentViewer(pCurrentViewer);
  fpVisManager -> SetCurrentSceneHandler(pCurrentSceneHandler);
  fpVisManager -> SetCurrentScene(pCurrentScene);
  fpVisManager -> SetVerboseLevel(currentVerbosity);
  // Take care of special case of scene handler with no viewer yet.  
  if (pCurrentSceneHandler) {
    G4ViewerList& viewerList = pCurrentSceneHandler -> SetViewerList ();
    const G4int nViewers = viewerList.size ();
    if (nViewers) {
      pCurrentSceneHandler -> SetCurrentViewer (pCurrentViewer);
      if (pCurrentViewer && pCurrentSceneHandler->GetScene()) {
	pCurrentViewer -> SetView ();
      }
    }
  }
}

////////////// /vis/scene/select ///////////////////////////////////////

G4VisCommandSceneSelect::G4VisCommandSceneSelect () {
  G4bool omitable;
  fpCommand = new G4UIcmdWithAString ("/vis/scene/select", this);
  fpCommand -> SetGuidance ("Selects a scene");
  fpCommand -> SetGuidance 
    ("Makes the scene current.  \"/vis/scene/list\" to see"
     "\n possible scene names.");
  fpCommand -> SetParameterName ("scene-name", omitable = false);
}

G4VisCommandSceneSelect::~G4VisCommandSceneSelect () {
  delete fpCommand;
}

G4String G4VisCommandSceneSelect::GetCurrentValue (G4UIcommand*) {
  return "";
}

void G4VisCommandSceneSelect::SetNewValue (G4UIcommand*, G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4String& selectName = newValue;
  G4SceneList& sceneList = fpVisManager -> SetSceneList ();
  G4int iScene, nScenes = sceneList.size ();
  for (iScene = 0; iScene < nScenes; iScene++) {
    if (sceneList [iScene] -> GetName () == selectName) break;
  }
  if (iScene >= nScenes) {
    if (verbosity >= G4VisManager::warnings) {
      G4cout << "WARNING: Scene \"" << selectName
	     << "\" not found - \"/vis/scene/list\" to see possibilities."
	     << G4endl;
    }
    return;
  }

  if (verbosity >= G4VisManager::confirmations) {
    G4cout << "Scene \"" << selectName
	   << "\" selected." << G4endl;
  }
  UpdateVisManagerScene (selectName);

}

////////////// /vis/scene/transientsAction ////////////////////////////

G4VisCommandSceneTransientsAction::G4VisCommandSceneTransientsAction () {
  G4bool omitable;
  fpCommand = new G4UIcmdWithAString ("/vis/scene/transientsAction", this);
  fpCommand -> SetGuidance
    ("Rerun events to get transienst (trajectories, etc.), when needed.");
  fpCommand -> SetGuidance
    ("Note: ineffective in absence of instantiated run manager.");
  fpCommand -> SetParameterName ("action", omitable = true);
  fpCommand -> SetCandidates ("rerun none");
  fpCommand -> SetDefaultValue ("rerun");

}

G4VisCommandSceneTransientsAction::~G4VisCommandSceneTransientsAction () {
  delete fpCommand;
}

G4String G4VisCommandSceneTransientsAction::GetCurrentValue(G4UIcommand*) {
  return "";
}

void G4VisCommandSceneTransientsAction::SetNewValue (G4UIcommand*,
						     G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4String action;
  std::istringstream is (newValue);
  is >> action;

  G4Scene* pScene = fpVisManager->GetCurrentScene();
  if (!pScene) {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<	"ERROR: No current scene.  Please create one." << G4endl;
    }
    return;
  }

  G4VSceneHandler* pSceneHandler = fpVisManager->GetCurrentSceneHandler();
  if (!pSceneHandler) {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<	"ERROR: No current sceneHandler.  Please create one." << G4endl;
    }
    return;
  }

  if (action == "rerun") {
    pScene->SetRecomputeTransients(true);
  }
  else if (action == "none") {
    pScene->SetRecomputeTransients(false);
  }
  else {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<
	"ERROR: unrecognised parameter \"" << action << "\"."
             << G4endl;
    }
    return;
  }

  if (verbosity >= G4VisManager::confirmations) {
    G4cout << "Transients action set to \"";
    if (pScene->GetRecomputeTransients()) G4cout << "rerun";
    else G4cout << "none";
    G4cout << "\"" << G4endl;
  }
}
