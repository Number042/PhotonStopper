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
// $Id: PhSTDetectorConstruction.cc 2017-06-02 mlueckho $
//
/// \file PhSTDetectorConstruction.cc
/// \brief Implementation of the PhSTDetectorConstruction class
#include "PhSTDetectorConstruction.hh"
#include "PhSTTrackerSD.hh"
#include "PhSTTargetSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4UniformMagField.hh"
#include "G4Element.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

PhSTDetectorConstruction::PhSTDetectorConstruction(G4double TargetThickness_input, G4String TargetMaterial_input) :
 LogTarget(NULL), TargetMaterial(NULL), TargetThickness(NULL),
 LogTracker(NULL), TrackerMaterial(NULL)
{
    // WORLD - box of equal dimensions
	worldLength = 20*cm;    // box of 10 cm size

	// TARGET - block with thickness, height and breadth
    TargetThickness = TargetThickness_input*cm;
    targetHeight = worldLength;
    targetBreadth = worldLength;

    // DETECTOR - box after block
	detHeight = worldLength;
	detBreadth = worldLength;
	detThickness = 5*mm;

	// Distance target-detector
	detDist = 1*cm;

	// Materials
	//
	DefineMaterials();
	TargetMaterial = G4Material::GetMaterial(TargetMaterial_input);
	TrackerMaterial = vacuumMaterial;

	//fMessenger = new PhSTDetectorMessenger(this);
}

G4VPhysicalVolume* PhSTDetectorConstruction::Construct()
{
	// Clean old geometry, if any
	//
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();


	// 1st: world volume and placement
	//

    // SOLID
	world = new G4Box("world", worldLength, worldLength, worldLength);		// name and size (X,Y,Z)
    // LOGIC
	LogWorld = new G4LogicalVolume(	world,				// its solid
									vacuumMaterial,		// its material
									"WorldLV");			// its name

	// PHYSICAL (placement)
	worldPV = new G4PVPlacement(0,				// no rotation
	                            G4ThreeVector(),// origin (0,0,0)
	                            LogWorld,		// logical volume
	                            "World",		// its name
	                            0,				// mother volume
	                            false,			// no boolean operations
	                            0,				// copy number
	                            true);			// checking overlaps

	// 2nd: target volume and placement
	//

    // SOLID
	target = new G4Box("targetBlock", targetBreadth, targetHeight, TargetThickness);   // its name and size (X,Y,Z)
    // LOGIC
	LogTarget = new G4LogicalVolume(target, 		// logical volume
									TargetMaterial,	// material
									"Target");		// its name
	// PHYSICAL (placement)
	targetPV = new G4PVPlacement(0,						// no rotation
						  	  	 G4ThreeVector(0, 0, 0),// position (x,y,z)
								 LogTarget,				// its logical volume
								 "Target",				// its name
								 LogWorld,				// its mother volume
								 false,					// no boolean operations
								 0,						// copy number
						  	  	 true);					// checking overlaps

	G4cout << " =========== TARGET MESSENGER ============================ " << G4endl;
	G4cout << "		Target is made of " << TargetMaterial->GetName() 		<< G4endl;
	G4cout << " ========================================================= " << G4endl;


	// 3rd: tracker (detector) volume and placement
	//
    // SOLID
	tracker = new G4Box("tracker", detBreadth, detHeight, detThickness);  // name and size (X,Y,Z) (dimensions same as target, except length)
	// LOGIC
	LogTracker = new G4LogicalVolume(tracker,			// logical volume
									 TrackerMaterial,	// material
									 "Tracker");		// its name
	// PHYSICAL (placement)
	trackerPV = new G4PVPlacement(0,												// no rotation
						  	  	  G4ThreeVector(0, 0, TargetThickness + detDist),	// position (x,y,z) !! placement relative to variable target width !!
							  	  LogTracker,										// its logical volume
								  "Tracker",										// its name
								  LogWorld,											// its mother
								  false,											// no bools
								  0,												// copy number
								  true);											// checking overlaps


	G4cout << " =========== DETECTOR MESSENGER ============================ " << G4endl;
	G4cout << "		The detector is made of " << TrackerMaterial->GetName()   << G4endl;
	G4cout << " =========================================================== " << G4endl;

	// Pointer to detector manager
	//
	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	// Set tracker as SD
	//
	G4VSensitiveDetector* targetSD = new PhSTTargetSD("TargetSD_target");
	SDman->AddNewDetector(targetSD);
	LogTarget->SetSensitiveDetector(targetSD);
	G4VSensitiveDetector* detectorSD = new PhSTTrackerSD("TrackerSD_tracker");
	SDman->AddNewDetector(detectorSD);
	LogTracker->SetSensitiveDetector(detectorSD);

	// always return the physical world
	//
	return worldPV;
}

void PhSTDetectorConstruction::DefineMaterials()
{
	// define the materials; derive vacuum (world), aluminium (target) from NIST manager
	//
	G4NistManager* nistMan = G4NistManager::Instance();
    nistMan->SetVerbose(1);

	G4Material* Vacuum = nistMan->FindOrBuildMaterial("G4_Galactic");
	G4Material* Al	= nistMan->FindOrBuildMaterial("G4_Al");
	G4Material* C 	= nistMan->FindOrBuildMaterial("G4_C");
	G4Material* Cu	= nistMan->FindOrBuildMaterial("G4_Cu");
	G4Material* Pb	= nistMan->FindOrBuildMaterial("G4_Pb");
	G4Material* Ti	= nistMan->FindOrBuildMaterial("G4_Ti");

    // Default materials
    //
    vacuumMaterial 	= Vacuum;
    AlMaterial		= Al;
    CMaterial		= C;
    CuMaterial		= Cu;
    PbMaterial		= Pb;
    TiMaterial		= Ti;
    
	// print materials
	//
	G4cout << *(G4Material::GetMaterialTable()) << G4endl << std::flush;
}

void PhSTDetectorConstruction::SetTargetMaterial(G4String materialName)
{
	// Search material by its name
	//
	G4Material* pttoMaterial = G4Material::GetMaterial(materialName);
	if(pttoMaterial) TargetMaterial = pttoMaterial;
}





