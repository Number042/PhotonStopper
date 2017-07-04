/*
 * PhSTMomentumHit.hh
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#ifndef PhSTMomentumHit_HH
#define PhSTMomentumHit_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4ThreeVector.hh"

class G4AttDef;
class G4AttValue;

class PhSTMomentumHit : public G4VHit {

public:

	// Constructor
	//
	PhSTMomentumHit(G4ThreeVector momentum);

	// Destructor
	//
	virtual ~PhSTMomentumHit();
	inline void *operator new(size_t);
	inline void operator delete(void *aHit);

	// Methods
	//
	virtual void Print();

	inline const G4ThreeVector& GetMomentum() const { return trackMomentum; }

private:

	G4ThreeVector trackMomentum;
};

typedef G4THitsCollection<PhSTMomentumHit> PhSTMomentumHitsCollection;

extern G4Allocator<PhSTMomentumHit> PhSTMomentumHitAllocator;

inline void* PhSTMomentumHit::operator new(size_t) {
	void* aHit;
	aHit = (void*)PhSTMomentumHitAllocator.MallocSingle();
	return aHit;
}

inline void PhSTMomentumHit::operator delete(void* aHit) {
	PhSTMomentumHitAllocator.FreeSingle((PhSTMomentumHit*) aHit);
}

#endif
