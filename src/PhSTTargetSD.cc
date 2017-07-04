/*
 * PhSTTargetSD.cc
 *
 *  Created on: Jul 4, 2017
 *      Author: mlueckhof
 */

#include "PhSTTargetSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include <iostream>

PhSTTargetSD::PhSTTargetSD(const G4String& name) : G4VSensitiveDetector(name) {

	collectionName.insert("EdepCollection");
	fHitsCollectionID_edep = -1;
}

PhSTTargetSD::~PhSTTargetSD() {}


// Called at the beginning of each event and creating the hits collection
//
void PhSTTargetSD::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) {

	// Energy deposit
	//
	fHitsCollection_edep = new PhSTEdepHitsCollection(SensitiveDetectorName, collectionName[0]);
	if(fHitsCollectionID_edep < 0) {
		fHitsCollectionID_edep = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection_edep);
	}
	hitsCollectionOfThisEvent->AddHitsCollection(fHitsCollectionID_edep, fHitsCollection_edep);
}

// Called at each step in the scoring logical volume
//
G4bool PhSTTargetSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

	// Energy deposit - always done
	//
	PhSTEdepHit* aHit_edep = new PhSTEdepHit();
	aHit_edep->SetDepositedEnergy(aStep->GetTotalEnergyDeposit());
	fHitsCollection_edep->insert(aHit_edep);

	return true;
}



