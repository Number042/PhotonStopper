/*
 * PhSTTrackerHit.hh
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#ifndef PhSTTrackerHit_HH
#define PhSTTrackerHit_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4ThreeVector.hh"

class G4AttDef;
class G4AttValue;

class PhSTTrackerHit : public G4VHit {

public:

	// Constructor
	//
	PhSTTrackerHit();
	PhSTTrackerHit(G4double energy, G4double angle, G4int id, G4int charge, G4ThreeVector momentum);

	// Destructor
	//
	virtual ~PhSTTrackerHit();
	inline void *operator new(size_t);
	inline void operator delete(void *aHit);

	// Methods
	//
	virtual void Print();

	inline G4double GetTrackEnergy()			const { return trackEnergy; }
	inline G4double GetTrackAngle()				const { return trackAngle; }
	inline G4int GetPDG()						const { return PDG; }
	inline G4int GetCharge()					const { return particleCharge; }
	inline const G4ThreeVector& GetMoementum() 	const { return trackMomentum; }

	inline void SetType(G4String type) { particleType = type; }
	inline const G4String& GetType() const { return particleType; }

private:

	// Data members
	//
	G4double trackEnergy;
	G4double trackAngle;
	G4int PDG;
	G4int particleCharge;

	G4ThreeVector trackMomentum;

	G4String particleType;

};

typedef G4THitsCollection<PhSTTrackerHit> PhSTTrackerHitsCollection;

extern G4Allocator<PhSTTrackerHit> PhSTTrackerHitAllocator;

inline void* PhSTTrackerHit::operator new(size_t) {
	void* aHit;
	aHit = (void*)PhSTTrackerHitAllocator.MallocSingle();
	return aHit;
}

inline void PhSTTrackerHit::operator delete(void* aHit) {
	PhSTTrackerHitAllocator.FreeSingle((PhSTTrackerHit*) aHit) ;
}

#endif
