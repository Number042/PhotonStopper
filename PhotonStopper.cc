/*
 * PhotonStopper.cc
 *
 *  Created on: Jun 7, 2017
 *      Author: mlueckhof
 */

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "PhSTDetectorConstruction.hh"
#include "PhSTPrimaryGeneratorAction.hh"
#include "PhSTRunAction.hh"
#include "PhSTEventAction.hh"
#include "PhSTPhysicsList.hh"

#include "G4PhysListFactory.hh"

#include "PhSTAnalysis.hh"

#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include <string> //C++11 std::stoi

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include <unistd.h> //getopt()

int main(int argc, char** argv)
{
	// Command-line arguments
	//
	int getopt_char;
	double target_thickness = 1.0;

	G4String physListName = "PhSTPhysicsList";
	G4int numbEvents = 0;
	G4bool useGUI = true;

	while((getopt_char = getopt(argc, argv, "t:p:n:hb")) != -1) {
		switch(getopt_char) {
		// HELP
		case 'h':
			G4cout << "This is the PhtonStopper Program - Welcome!" << G4endl
				<< G4endl
				<< "Possible options for this program:" << G4endl
				<< "-t <double> : Target thickness [mm], accepts exponential notation. Default/current value = " << target_thickness << G4endl
				<< "-p <string> : Physics list name, default/current = '" << physListName << G4endl
				<< "-n <int> : Enter number of events" << G4endl
				<< "-b : Do not use a GUI" << G4endl << G4endl;
			G4cout << "Using both, -g and -n options, events are ran before initialization of the GUI." << G4endl;
			exit(1);
			break;
		// BATCH MODE
		case 'b':
			useGUI = false;
			break;
		// TARGET THICKNESS
		case 't':
			try {
				target_thickness = std::stod(string(optarg));
			}
			catch(const std::invalid_argument& ia) {
				G4cout 	<< "Reading target thickness - INVALID ARGUMENT!" << G4endl
						<< "Passed: " << optarg << "'" 				<< G4endl
						<< "Expected: floating point number!" 		<< G4endl;
			exit(1);
			}
			break;
		// NUMBER OF EVENTS
		case 'n':
			try {
				numbEvents = std::stoi(string(optarg));
			}
			catch(const std::invalid_argument& ia) {
				G4cout 	<< "Reading event number - INVALID ARGUMENT!" << G4endl
						<< "Passed: " << optarg << "'" << G4endl
						<< "Expected: integer!" << G4endl;
			exit(1);
			}
			break;
		// PHYSICS LIST
		case 'p':
			physListName = G4String(optarg);
			break;
		default:
			G4cout << "Got unknown getopt_char = " << " = '" << char(getopt_char) << "' when parsing cmd line arguments." << G4endl;
		}
	}

	// Remaining arguments passed to Geant as array
	//
	int argc_effective = argc - optind + 1;
	char** argv_effective = new char*[argc_effective];
	argv_effective[0] = argv[0];
	for(int i = optind; i < argc; i++) {
		argv_effective[i + 1 - optind] = argv[i];
	}

	// Print arguments (passed from user and/or defaults)
	//
	G4cout << "These command line arguments were passed to the program: " << G4endl
		<< "target_thickness = " << target_thickness << " [mm]"	<< G4endl
		<< "physListName = '" << physListName << "'" 			<< G4endl
		<< "numbEvents = " << numbEvents 						<< G4endl
		<< "useGUI = " << (useGUI == true ? "yes" : "no") 			<< G4endl;
	G4cout << "Arguments passed on to Geant4: " 					<< G4endl;
	for(int i = 0; i < argc_effective; i ++) {
		G4cout << i << " '" << argv_effective[i] << "'" << G4endl;
	}

	G4cout << "Starting Geant4 . . . " << G4endl << G4endl;

	// Instance of the RunManager
	//
	G4RunManager* runMan = new G4RunManager;

	// Mandatory initialization classes
	//
	PhSTDetectorConstruction* detector = new PhSTDetectorConstruction(target_thickness);
	runMan->SetUserInitialization(detector);
	PhSTPhysicsList* physlist = new PhSTPhysicsList;
	runMan->SetUserInitialization(physlist);

	// User action classes
	//
	PhSTPrimaryGeneratorAction* gen_Act = new PhSTPrimaryGeneratorAction(detector);
	runMan->SetUserAction(gen_Act);
	PhSTRunAction* run_Act = new PhSTRunAction;
	runMan->SetUserAction(run_Act);
	PhSTEventAction* even_Act = new PhSTEventAction(run_Act);
	runMan->SetUserAction(even_Act);

	// Initialization of G4 kernel
	//
	runMan->Initialize();

	// Metadata for ROOT output
	//
	PhSTAnalysis::GetInstance()->SetMetadata(physListName);

#ifdef G4VIS_USE
	// Initialization of visualization
	//
	G4VisManager* visMan = new G4VisExecutive;
	visMan->Initialize();
#endif

	// Pointer to UI manager
	//
	G4UImanager* UIman = G4UImanager::GetUIpointer();

	if(argc_effective != 1) {
		if(useGUI) {
			G4cout << "UseGUI is incompatible with batch mode!" << G4endl;
			exit(1);
		}
		G4String command = "/control/execute ";
		G4String fileName = argv_effective[1];
		UIman->ApplyCommand(command + fileName);
	}
	else if(useGUI == true) {

		// Interactive mode: define UI session
		//
#ifdef G4UI_USE
		G4UIExecutive* ui = new G4UIExecutive(argc_effective, argv_effective);
#ifdef G4VIS_USE
		UIman->ApplyCommand("/control/execute vis.mac");
#endif
	if(numbEvents > 0) {
		G4cout << G4String("'/run/beamOn ") + std::to_string(numbEvents) << "'" << G4endl;
		UIman->ApplyCommand(G4String("/run/beamOn ") + std::to_string(numbEvents));
	}
	if(ui->IsGUI())
		ui->SessionStart();
	delete ui;
#endif
	}

	// Run given the number of events
	//
	if(useGUI == false and numbEvents > 0) {
		G4cout << G4String("'/run/beamOn ") + std::to_string(numbEvents) << "'" << G4endl;
		UIman->ApplyCommand(G4String("/run/beamOn ") + std::to_string(numbEvents));
	}

	// Terminate the job. Free store
	//
#ifdef G4VIS_USE
	delete visMan;
#endif

	// Delete RunManager
	//
	delete runMan;
	G4cout << "RunManager deleted" << G4endl;
}
