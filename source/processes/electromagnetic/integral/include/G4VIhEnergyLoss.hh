//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4VIhEnergyLoss.hh,v 1.1.2.2 2001/06/28 20:19:19 gunter Exp $
// GEANT4 tag $Name:  $
//
// $Id: 
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//      History: first implementation, based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4VIhEnergyLoss physics process -----------
//                by Laszlo Urban, 30 May 1997 
//
// ************************************************************             
// It is the first implementation of the NEW UNIFIED ENERGY LOSS PROCESS.
// It calculates the continuous energy loss for charged hadrons.
// Processes giving contribution to the continuous loss :
//   ionisation (= cont.ion.loss + delta ray production)
//   can be added more easily ..........
// This class creates static proton/antiproton dE/dx and range tables ,
// which tables can be used by other processes.
// The energy loss for other charged hadrons is calculated from the p/pbar
// tables with scaled kinetic energy.
//
// **************************************************************************** 
// It is assumed that the cut in range is the same for all the charged hadrons! 
// ****************************************************************************
//
// 7/10/98 some bugs fixed + some cleanup , L.Urban 
// 26/10/98 cleanup , L.Urban
//

#ifndef G4VIhEnergyLoss_h
#define G4VIhEnergyLoss_h 1
 
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4IVContinuousDiscreteProcess.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Electron.hh"
#include "G4VParticleChange.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsLinearVector.hh"
 
class G4EnergyLossMessenger;
 
class G4VIhEnergyLoss : public G4IVContinuousDiscreteProcess
 
{
  public:

    G4VIhEnergyLoss(const G4String& );

    virtual ~G4VIhEnergyLoss();

    G4bool IsApplicable(const G4ParticleDefinition&);

    G4double GetContinuousStepLimit(
                                    const G4Track& track,
                                    G4double previousStepSize,
                                    G4double currentMinimumStep,
                                    G4double& currentSafety) ; 

    G4VParticleChange* AlongStepDoIt(const G4Track& track ,const G4Step& Step) ;

    virtual G4VParticleChange* PostStepDoIt(const G4Track& track,
                                               const G4Step& Step) = 0 ;


  protected:


  private:

  // hide  assignment operator 

    G4VIhEnergyLoss(G4VIhEnergyLoss &);
    G4VIhEnergyLoss & operator=(const G4VIhEnergyLoss &right);

    G4double GetConstraints(const G4DynamicParticle *aParticle,
                            G4Material *aMaterial);
                                       
    G4double GetLossWithFluct(const G4DynamicParticle *aParticle,
                              G4Material *aMaterial,
                              G4double ChargeSquare,
                              G4double MeanLoss,
                              G4double Step) ;

// =====================================================================

  public:


  protected:

    G4PhysicsTable* theLossTable ;
   

  private:

    G4double fdEdx;      // computed in GetContraints

    G4double fRangeNow ; // computed in GetContraints

    G4int EnergyBinNumber ;
    G4double RangeCoeffA,RangeCoeffB,RangeCoeffC ;

    // variables for the integration routines
     static G4double Mass,taulow,tauhigh,ltaulow,ltauhigh;

    // data members to speed up the fluctuation calculation
    G4Material *lastMaterial ;
    G4int imat ;
    G4double f1Fluct,f2Fluct,e1Fluct,e2Fluct,rateFluct,ipotFluct;
    G4double e1LogFluct,e2LogFluct,ipotLogFluct;
    const G4double MaxExcitationNumber ;
    const G4double probLimFluct ;
    const long nmaxDirectFluct,nmaxCont1,nmaxCont2 ;

// ====================================================================
//  static part of the class
 
  public:

    //  get the number of processes contributing to the cont.energy loss
    static G4int GetNumberOfProcesses()    { return NumberOfProcesses; }; 

    //  set the number of processes contributing to the cont.energy loss
    static void SetNumberOfProcesses(G4int number)
                                {NumberOfProcesses=number ; }; 

    //  Increment the number of processes contributing to the cont.energy loss
    static void PlusNumberOfProcesses()
                                { NumberOfProcesses++  ; }; 

    //  decrement the number of processes contributing to the cont.energy loss
    static void MinusNumberOfProcesses()
                                { NumberOfProcesses--  ; }; 

    static void SetdRoverRange(G4double value) {dRoverRange = value;}
    static void SetRndmStep     (G4bool   value) {rndmStepFlag   = value;}
    static void SetEnlossFluc   (G4bool   value) {EnlossFlucFlag = value;}
    static void SetStepFunction (G4double c1, G4double c2)
                                      {dRoverRange = c1; finalRange = c2;}

  protected:

    static void BuildDEDXTable(const G4ParticleDefinition& aParticleType);

  private:

    static void BuildRangeTable(const G4ParticleDefinition& aParticleType);

    static void BuildInverseRangeTable(
                                const G4ParticleDefinition& aParticleType);

    static void BuildTimeTables(const G4ParticleDefinition& aParticleType);

    static void BuildLabTimeVector(G4int materialIndex,
                          G4PhysicsLogVector* rangeVector);

    static void BuildProperTimeVector(G4int materialIndex,
                          G4PhysicsLogVector* rangeVector);

    static void InvertRangeVector(G4int materialIndex,
                          G4PhysicsLogVector* rangeVector);

    static void BuildRangeVector(G4int materialIndex,
                          G4PhysicsLogVector* rangeVector);

    static G4double LabTimeIntLog(G4PhysicsVector* physicsVector
                                                        ,G4int nbin);

    static G4double ProperTimeIntLog(G4PhysicsVector* physicsVector,
                                                         G4int nbin);

    static G4double RangeIntLin(G4PhysicsVector* physicsVector
                                                        ,G4int nbin);

    static G4double RangeIntLog(G4PhysicsVector* physicsVector
                                                        ,G4int nbin);

    static void BuildRangeCoeffATable(
                          const G4ParticleDefinition& aParticleType);
    static void BuildRangeCoeffBTable(
                          const G4ParticleDefinition& aParticleType);
    static void BuildRangeCoeffCTable(
                          const G4ParticleDefinition& aParticleType);



// ====================================================================


  public:


  protected:

    static G4PhysicsTable* theDEDXpTable ;
    static G4PhysicsTable* theDEDXpbarTable ;
    static G4PhysicsTable* theRangepTable ;
    static G4PhysicsTable* theRangepbarTable ;

    //inverse of the range tables
    static G4PhysicsTable* theInverseRangepTable ;
    static G4PhysicsTable* theInverseRangepbarTable ;

    //lab and proper time tables
    static G4PhysicsTable* theLabTimepTable ;
    static G4PhysicsTable* theLabTimepbarTable ;

    static G4PhysicsTable* theProperTimepTable ;
    static G4PhysicsTable* theProperTimepbarTable ;

    //  processes inherited from G4VIhEnergyLoss 
    //   register themselves  in the static array Recorder
    static G4PhysicsTable** RecorderOfpProcess;
    static G4PhysicsTable** RecorderOfpbarProcess;
    static G4int CounterOfpProcess ;
    static G4int CounterOfpbarProcess ;

    // particle mass
    static G4double ParticleMass ;

    static const G4Proton* theProton ;
    static const G4AntiProton* theAntiProton ;

    // cut in range
    static G4double CutInRange;

    static G4double LowestKineticEnergy;
    static G4double HighestKineticEnergy;
    static G4int TotBin; // number of bins in table,
                         // calculated in BuildPhysicsTable
    static G4double RTable,LOGRTable; // LOGRTable=log(HighestKineticEnergy
                                      //          /LowestKineticEnergy)/TotBin
                                      //   RTable = exp(LOGRTable)

  private:

    static G4PhysicsTable* theDEDXTable;

    static G4PhysicsTable* theRangeTable;
    static G4PhysicsTable* theInverseRangeTable;

    static G4PhysicsTable* theLabTimeTable;
    static G4PhysicsTable* theProperTimeTable;

    static G4PhysicsTable** RecorderOfProcess;
    static G4int CounterOfProcess;


    static G4PhysicsTable* thepRangeCoeffATable;
    static G4PhysicsTable* thepRangeCoeffBTable;
    static G4PhysicsTable* thepRangeCoeffCTable;
    static G4PhysicsTable* thepbarRangeCoeffATable;
    static G4PhysicsTable* thepbarRangeCoeffBTable;
    static G4PhysicsTable* thepbarRangeCoeffCTable;

    static G4PhysicsTable* theRangeCoeffATable;
    static G4PhysicsTable* theRangeCoeffBTable;
    static G4PhysicsTable* theRangeCoeffCTable;

    static G4double dRoverRange ; // maximum allowed deltarange/range
                                  //  in one step  

    static G4double finalRange ;  // last step before stop

    static G4bool rndmStepFlag ;
    static G4bool EnlossFlucFlag ;

    static G4int NumberOfProcesses ;


};
 
#include "G4VIhEnergyLoss.icc"

#endif
 