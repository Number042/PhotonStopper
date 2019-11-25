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
// $Id: PhSTPrimaryGeneratorAction.cc 2017-05-02 mlueckho $
//
/// \file PhSTPrimaryGeneratorAction.cc
/// \brief Implementation of the PhSTPrimaryGeneratorAction class

#include "PhSTPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

PhSTPrimaryGeneratorAction::PhSTPrimaryGeneratorAction(PhSTDetectorConstruction* DC, G4double energy_in) : Detector(DC)
{
	G4String particleName = "gamma";
	G4int numbOfParticles = 1;
	this->energy_in = energy_in;						// this points to current object to let the program know where to take the (copy of?) variable from from
	fParticleGun = new G4ParticleGun(numbOfParticles);

	// default photon with 100 keV
    //
	G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	fParticleGun->SetParticleDefinition(particle);
}

PhSTPrimaryGeneratorAction::~PhSTPrimaryGeneratorAction()
{
	delete fParticleGun;
}

void PhSTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// Set energy, position and momentum
	// particle kinematics: definition, default momentum direction and energy
	//

	fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -20*cm));
	fParticleGun->SetParticleEnergy(energy_in*keV);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
