/*
 * PhSTTrackerSD.hh
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#ifndef PhSTTrackerSD_HH
#define PhSTTrackerSD_HH

#include "PhSTDetectorConstruction.hh"
#include "G4VSensitiveDetector.hh"
#include "PhSTTrackerHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class PhSTTrackerSD : public G4VSensitiveDetector {

public:

	PhSTTrackerSD (const G4String& name);
	virtual ~PhSTTrackerSD();

	// Methods
	//
	virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
	virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
	virtual void EndOfEvent(G4HCofThisEvent*) {};

private:

	PhSTDetectorConstruction* detectorConstruction;

	// Data members
	//
	PhSTTrackerHitsCollection* fHitsCollection;
	G4int fHitsCollectionID;

};

#endif
