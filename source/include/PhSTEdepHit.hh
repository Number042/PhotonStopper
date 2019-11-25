/*
 * PhSTEdepHit.hh
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#ifndef PhSTEdepHit_HH
#define PhSTEdepHit_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class PhSTEdepHit : public G4VHit {

public:

	// Constructor and Destructor
	//
	PhSTEdepHit();

	virtual ~PhSTEdepHit();
	inline void *operator new(size_t);
	inline void operator delete(void *aHit);

	// Methods
	//
	virtual void Print();

	// energy deposit
	//
	inline void SetDepositedEnergy(G4double energy) { fDepositedEnergy = energy; }
	inline G4double GetDepositedEnergy() const { return fDepositedEnergy; }

private:

	G4double fDepositedEnergy;
};

typedef G4THitsCollection<PhSTEdepHit> PhSTEdepHitsCollection;

extern G4Allocator<PhSTEdepHit> PhSTEdepHitAllocator;

inline void* PhSTEdepHit::operator new(size_t)
{
	void* aHit;
	aHit = (void*)PhSTEdepHitAllocator.MallocSingle();
	return aHit;
}

inline void PhSTEdepHit::operator delete(void* aHit)
{
	PhSTEdepHitAllocator.FreeSingle((PhSTEdepHit*) aHit);
}

#endif
