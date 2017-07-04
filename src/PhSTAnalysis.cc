/*
 * PhSTAnalysis.cc
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#include "PhSTAnalysis.hh"
#include "PhSTEdepHit.hh"
#include "PhSTTrackerHit.hh"

#include "G4SDManager.hh"
#include "G4Track.hh"

#include "G4RunManager.hh"
#include "PhSTDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

#include <iostream>
#include <iomanip>

using namespace std;

PhSTAnalysis* PhSTAnalysis::singleton = 0;

void PhSTAnalysis::makeHistos()
{
	G4RunManager* run = G4RunManager::GetRunManager();
	PhSTDetectorConstruction* detCon = (PhSTDetectorConstruction*)run->GetUserDetectorConstruction();

	// set root files in dedicated subdir 'plots/'
	//
	G4String rootFileName = "plots/histo_" + std::to_string(detCon->GetTargetThickness()/mm) + "mm_" + physListName + ".root";
	G4cout << "Opening ROOT file '" << rootFileName << "'" << G4endl;
	histFile = new TFile(rootFileName, "RECREATE");

	// Fill the histos with data
	//

	// ENERGY DEPOSIT -- overall
	//
	targetEdep = new TH1D("targetEdep", "targetEdep", 1000, 0, 6);
	targetEdep->GetXaxis()->SetTitle("Total energy deposit/event [MeV]");

	// NUMBER AND ENERGY -- photon specific (PDG photon: 22)
	//
	tracker_numPhot = new TH1D("numPhot", "numPhot", 100, -0.5, 100.5);
	tracker_numPhot->GetXaxis()->SetTitle("Number of photons passing the target");

	tracker_energPhot = new TH1D("energPhot", "energPhot", 1000, 0, 10.0);
	tracker_energPhot->GetXaxis()->SetTitle("Energy of photons passing the target");

	tracker_energy = new TH1D("energy", "energy", 10000, 0, 10.0);
	tracker_energy->GetXaxis()->SetTitle("Energy of outgoing particles [MeV]");

	numPhot_total = 0;
}

void PhSTAnalysis::writePerEvent(const G4Event* event)
{
	G4HCofThisEvent* HCE = event->GetHCofThisEvent();
	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	// Data from targetSD
	//
	G4int PhSTTargetEdep_CollID = SDman->GetCollectionID("EdepCollection");
	if(PhSTTargetEdep_CollID >= 0) {
		PhSTEdepHitsCollection* targetEdepHitsCollection = NULL;
		targetEdepHitsCollection = (PhSTEdepHitsCollection*) (HCE->GetHC(PhSTTargetEdep_CollID));

		if(targetEdepHitsCollection != NULL) {
			G4int nEntries = targetEdepHitsCollection->entries();
			G4double edep = 0.0;

			for(G4int i = 0; i < nEntries; i++) {
				edep += (*targetEdepHitsCollection)[i]->GetDepositedEnergy();
			}
			// Overall energ deposit
			//
			targetEdep->Fill(edep/MeV);
		}
		else {
			G4cout << "targetEdepHitsCollection was NULL!" << G4endl;
		}
	}
	else{
		G4cout << "PhSTTargetEdep_CollID was " << PhSTTargetEdep_CollID << " < 0!" << G4endl;
	}

	// Data from trackerSD
	//
	G4int PhSTTrackerSD_CollID = SDman->GetCollectionID("TrackerCollection");
	if(PhSTTrackerSD_CollID >= 0) {
		PhSTTrackerHitsCollection* trackerHitsCollection = NULL;
		trackerHitsCollection = (PhSTTrackerHitsCollection*) (HCE->GetHC(PhSTTrackerSD_CollID));

		if(trackerHitsCollection != NULL) {
			G4int nEntries = trackerHitsCollection->entries();

			// Loop over entries, read specific numbers
			//
			for(G4int i = 0; i < nEntries; i++) {
				const G4double 	energy 	= (*trackerHitsCollection)[i]->GetTrackEnergy();
				const G4int 	PDG 	= (*trackerHitsCollection)[i]->GetPDG();
				//const G4int		charge 	= (*trackerHitsCollection)[i]->GetCharge();
				//const G4String& type	= (*trackerHitsCollection)[i]->GetType();

				// Overall histograms
				//
				tracker_energy->Fill(energy/MeV);

				// Per particletype histograms --> photons
				//
				if(PDG == 22) {
					tracker_energPhot->Fill(energy/MeV);
					numPhot_total += 1;
				}
			}

			tracker_numPhot->Fill(numPhot_total);
		}
	}
}

void PhSTAnalysis::writeHistos()
{
	// target
	//
	targetEdep->Write();
	delete targetEdep; targetEdep = NULL;

	// tracker
	//
	tracker_numPhot->Write();
	delete tracker_numPhot; tracker_numPhot = NULL;
	tracker_energPhot->Write();
	delete tracker_energPhot; tracker_energPhot = NULL;
	tracker_energy->Write();
	delete tracker_energy; tracker_energy = NULL;

	G4cout << "Got types at tracker: " << G4endl;
	for(std::map<G4int, G4int>::iterator it = tracker_particleTypes.begin(); it != tracker_particleTypes.end(); it++) {
		G4cout << std::setw(15) << it->first << " = "
			<< std::setw(15) << tracker_particleNames[it->first] << " : "
			<< std::setw(15) << it->second << " = ";
		G4cout << endl;
	}

	tracker_particleTypes.clear();

	// write out the histograms, close and clean
	//
	histFile->Write();
	histFile->Close();
	delete histFile; histFile = NULL;
}

void PhSTAnalysis::SetMetadata(const G4String physListName_in) {
	this->physListName = physListName_in;
}
