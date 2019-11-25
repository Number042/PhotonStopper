/*
 * PhSTTrackerHit.cc
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#include "PhSTTrackerHit.hh"

G4Allocator<PhSTTrackerHit> PhSTTrackerHitAllocator;

PhSTTrackerHit::PhSTTrackerHit() :
		trackEnergy(0.0), trackAngle(0.0), PDG(0) {
}

PhSTTrackerHit::PhSTTrackerHit(G4double energy, G4double angle, G4int id, G4int charge, G4ThreeVector momentum) :
		trackEnergy(energy), trackAngle(angle), PDG(id), particleCharge(charge), trackMomentum(momentum) {}

PhSTTrackerHit::~PhSTTrackerHit() {}

void PhSTTrackerHit::Print() {}



