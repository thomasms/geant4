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
// By JPW, working, but to be cleaned up. @@@

#include "G4ProtonInelasticCrossSection.hh"
#include "globals.hh"

   G4double G4ProtonInelasticCrossSection::
   GetCrossSection(const G4DynamicParticle* aPart, 
                   const G4Element* anEle, G4double )
   {
      G4double atomicNumber = anEle->GetN();
      G4double nOfProtons = anEle->GetZ();
      return GetCrossSection(aPart->GetKineticEnergy(), atomicNumber, nOfProtons);
   }
   
   G4double G4ProtonInelasticCrossSection::
   GetCrossSection(G4double kineticEnergy, G4double atomicNumber, G4double nOfProtons)
   {
      G4double nOfNeutrons = atomicNumber-nOfProtons;
      kineticEnergy /=GeV;
      G4double a = atomicNumber;
      const G4double nuleonRadius=1.36E-15;
      const G4double pi=3.14159265;
      G4double fac=pi*nuleonRadius*nuleonRadius;
      G4double b0=2.247-0.915*(1-std::pow(a,-0.3333));
      G4double fac1=b0*(1-std::pow(a,-0.3333));
      G4double fac2=1.;
      if(nOfNeutrons>1.5) fac2=std::log((nOfNeutrons));
      G4double crossSection = 1E31*fac*fac2*(1+std::pow(a,0.3333)-fac1);
// high energy correction
      crossSection = (1-0.15*std::exp(-kineticEnergy))*crossSection/(1.00-0.0007*a);
// first try on low energies: rise
      G4double ff1= 0.70-0.002*a;  // slope of the drop at medium energies.
      G4double ff2= 1.00+1/a;  // start of the slope.
      G4double ff3= 0.8+18/a-0.002*a; // stephight
      fac=1-(1/(1+std::exp(-8*ff1*(std::log10(kineticEnergy)+1.37*ff2))));
      crossSection = crossSection*(1+ff3*fac);
// low energy return to zero
      ff1=1.-1/a-0.001*a; // slope of the rise
      ff2=1.17-2.7/a-0.0014*a; // start of the rise
      fac=-8.*ff1*(std::log10(kineticEnergy)+2.0*ff2);
      fac=1/(1+std::exp(fac));
      crossSection = crossSection*fac;
      return crossSection*millibarn;
    }
