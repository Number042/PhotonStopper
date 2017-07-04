/*
 * PhSTTargetSD.hh
 *
 *  Created on: Jul 4, 2017
 *      Author: mlueckhof
 */

#ifndef TargetSD
#define TargetSD

#include "G4VSensitiveDetector.hh"
//#include "PhSTMomentumHit.hh"
#include "PhSTEdepHit.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class PhSTTargetSD : public G4VSensitiveDetector {

public:

	PhSTTargetSD(const G4String& name);
	virtual ~PhSTTargetSD();

	// Methods
	//
	virtual void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);
	virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
	virtual void EndOfEvent(G4HCofThisEvent*) {};

private:

	// Data members
	//
	PhSTEdepHitsCollection* fHitsCollection_edep;
	G4int fHitsCollectionID_edep;
};

#endif
