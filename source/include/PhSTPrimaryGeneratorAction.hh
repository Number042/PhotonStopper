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
// $Id: PhSTPrimaryGeneratorAction.hh 2017-04-28 mlueckho $
//
/// \file PhSTPrimaryGeneratorAction.hh
/// \brief Definition of the PhSTPrimaryGeneratorAction class
///			primary generator action class with particle gun
///			defining single particle, hitting the tracker perpendicular to
///			input face.
///			Particle type can be changed: G4ParticleGun

#ifndef PhSTPrimaryGeneratorAction_h
#define PhSTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class PhSTDetectorConstruction;

class TH1F;
class TH2D;

#include "TLegend.h"
#include "TLatex.h"
#include "TCanvas.h"

class PhSTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:

	// Constructor and Destructor
	//
	PhSTPrimaryGeneratorAction(PhSTDetectorConstruction*, G4double energy_in = 100e+3); // hard-code the default energy
	virtual ~PhSTPrimaryGeneratorAction(); //virtual

	// methods
	//
	virtual void GeneratePrimaries(G4Event*);

private:
	G4double energy_in;					// declare the energy as private variable
	G4ParticleGun* fParticleGun;		// pointer a to G4 class
	PhSTDetectorConstruction* Detector;	// pointer to the geometry

};

#endif
