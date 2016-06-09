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
// $Id: pyG4VisManager.cc,v 1.4 2006/06/29 15:36:56 gunter Exp $
// $Name: geant4-08-01 $
// ====================================================================
//   pyG4VisManager.cc
//
//                                         2005 Q
// ====================================================================
#include <boost/python.hpp>
#include "pyG4Version.hh"
#include "G4VisManager.hh"
#if G4VERSION_NUMBER >= 800
#include "G4TrajectoryModelFactories.hh"
#endif

using namespace boost::python;

// ====================================================================
// wrappers
// ====================================================================
class PyG4VisManager : public G4VisManager {
public:
  PyG4VisManager() { SetVerboseLevel(quiet); }
  ~PyG4VisManager() { }

  void SetVerbosity(Verbosity verb) { fVerbosity= verb; }

  virtual void RegisterGraphicsSystems() { }

#if G4VERSION_NUMBER >= 800
  virtual void RegisterModelFactories() {
    RegisterModelFactory(new G4TrajectoryDrawByChargeFactory());
    RegisterModelFactory(new G4TrajectoryDrawByParticleIDFactory());
  }
#endif

};

// ====================================================================
// module definition
// ====================================================================
void export_G4VisManager()
{
  scope in_PyG4VisManager =
    class_<PyG4VisManager, boost::noncopyable>
    ("G4VisManager", "visualization manager")
    // ---
    .def("GetConcreteInstance", &PyG4VisManager::GetConcreteInstance,
         "Get an instance of G4VisManager",
         return_value_policy<reference_existing_object>())
    .staticmethod("GetConcreteInstance")
    // ---
    .def("SetVerbosity", &PyG4VisManager::SetVerbosity)
    .def("GetVerbosity", &PyG4VisManager::GetVerbosity)
    .def("Initialize", &PyG4VisManager::Initialize)
    .def("RegisterGraphicsSystem", &PyG4VisManager::RegisterGraphicsSystem)
    ;

  // enum LineStyle
  enum_<G4VisManager::Verbosity>("Verbosity")
    .value("quiet",           G4VisManager::quiet)
    .value("startuo",         G4VisManager::startup)
    .value("errors",          G4VisManager::errors)
    .value("warnings",        G4VisManager::warnings)
    .value("confirmations",   G4VisManager::confirmations)
    .value("parameters",      G4VisManager::parameters)
    .value("all",             G4VisManager::all)
    ;
}

