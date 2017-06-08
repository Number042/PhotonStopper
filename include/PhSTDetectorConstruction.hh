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
// $Id: PhSTDetectorConstruction.hh 2017-06-02 mlueckho $
//
/// \file PhSTDetectorConstruction.hh
/// \brief Definition of the PhSTDetectorConstruction class
/// 		Detector construction class to define materials and geometry

#ifndef PhSTDetetectorConstruction_h
#define PhSTDetetectorConstruction_h 1

#include "globals.hh"						//
#include "G4VUserDetectorConstruction.hh"	// global toolkit header
#include "tls.hh"							//

class G4Box;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;

class PhSTDetectorMessenger;

class PhSTDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	PhSTDetectorConstruction(G4double TargetThickness_input = 1.0);
	~PhSTDetectorConstruction() {};

public:
	void SetTargetMaterial(G4String);
	G4VPhysicalVolume* Construct();

public:
	const G4VPhysicalVolume* GetPhysWorld() { return worldPV; };
	const G4VPhysicalVolume* GetTargetPV()	{ return targetPV; };

	inline G4double GetDetectorDistance()	{ return detDist; };
	inline G4double GetTargetThickness()	{ return TargetThickness; };

private:

	// MATERIALS -- to be changed for collimators
	//
	G4Material* vacuumMaterial;
	G4Material* AlMaterial;
	G4Material* CMaterial;
	G4Material* CuMaterial;
	G4Material* PbMaterial;
	G4Material* TiMaterial;
	G4Material* StainlessSteel;

	G4Material* TargetMaterial;
	G4Material* TrackerMaterial;


	// DIMENSIONS
	//
	G4double worldLength;

    G4double TargetThickness;
    G4double targetHeight;
    G4double targetBreadth;

	G4double detHeight;
	G4double detBreadth;
	G4double detThickness;

	G4double detDist;

	// VOLUMES (pointers)
	//
	G4Box*				world;
	G4LogicalVolume*	LogWorld;
	G4VPhysicalVolume*	worldPV;

	G4Box* 				target;
	G4LogicalVolume* 	LogTarget;
	G4VPhysicalVolume*	targetPV;

	G4Box*				tracker;
	G4LogicalVolume*	LogTracker;
	G4VPhysicalVolume*	trackerPV;

private:
	void DefineMaterials();

};

#endif
