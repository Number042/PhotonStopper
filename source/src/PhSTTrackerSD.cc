/*
 * PhSTTrackerSD.cc
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#include "PhSTTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

PhSTTrackerSD::PhSTTrackerSD(const G4String& name) :
	G4VSensitiveDetector(name) {

	collectionName.insert("TrackerCollection");
	fHitsCollectionID = -1;

	G4RunManager* run = G4RunManager::GetRunManager();
	detectorConstruction = (PhSTDetectorConstruction*)run->GetUserDetectorConstruction();
}

PhSTTrackerSD::~PhSTTrackerSD() {}

// Called at beginning of each event
//
void PhSTTrackerSD::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent) {

	// For each event, make new hits collection
	//
	fHitsCollection = new PhSTTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
	if (fHitsCollectionID < 0) {
		fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
	}

	// Add collection to the event
	//
	hitsCollectionOfThisEvent->AddHitsCollection(fHitsCollectionID, fHitsCollection);

}

// Called each step in the scoring LV
//
G4bool PhSTTrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

	// only incoming tracks
	//
	if(aStep->GetPreStepPoint()->GetStepStatus() != fGeomBoundary) {
		G4cout << "PhSTTrackerSD::ProcessHits(): SKIP!" << G4endl;
		return false;
	}

	G4double energy = aStep->GetPreStepPoint()->GetTotalEnergy();
	const G4ThreeVector momentum = aStep->GetPreStepPoint()->GetMomentum();
	const G4ThreeVector& hitPos = aStep->GetPreStepPoint()->GetPosition();

	G4Track* theTrack = aStep->GetTrack();
	G4ParticleDefinition* particleType = theTrack->GetDefinition();
	G4int particleID = particleType->GetPDGEncoding();
	G4int particleCharge = particleType->GetPDGCharge();

	G4double angle = atan2(sqrt(hitPos.x()*hitPos.x() + hitPos.y()*hitPos.y()), detectorConstruction->GetDetectorDistance());

	PhSTTrackerHit* aHit = new PhSTTrackerHit(energy, angle, particleID, particleCharge, momentum);
	aHit->SetType(particleType->GetParticleSubType());
	fHitsCollection->insert(aHit);

	return true;
}


