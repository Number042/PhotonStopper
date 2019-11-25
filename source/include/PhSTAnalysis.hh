/*
 * PhSTAnalysis.hh
 *
 *  Created on: Jun 6, 2017
 *      Author: mlueckhof
 */

#ifndef PhSTAnalysis_HH_
#define PhSTAnalysis_HH_

#include "G4Event.hh"
#include "G4Run.hh"
#include "PhSTDetectorConstruction.hh"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <map>

class PhSTAnalysis {

public:

	static PhSTAnalysis* GetInstance() {
		if(PhSTAnalysis::singleton == NULL) PhSTAnalysis::singleton = new PhSTAnalysis();
		return PhSTAnalysis::singleton;
	}

	void makeHistos();
	void fillHistos(const G4Event *anEvent);
	void writeHistos();
	void writePerEvent(const G4Event* event);

private:

	PhSTAnalysis() {};
	static PhSTAnalysis* singleton;

	// Histogram file
	//
	TFile *histFile;

	// Histograms
	//
	TH1D* targetEdep;
	TH1D* tracker_numPhot;
	TH1D* tracker_energPhot;
	TH1D* tracker_energy;

	std::map<G4int,G4int> tracker_particleTypes;
	std::map<G4int,G4String> tracker_particleNames;
	G4int numPhot_total;

	// Generate filenames
	//
	G4String physListName;

public:

	void SetMetadata(const G4String physListName_in);

};

#endif
