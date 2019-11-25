/* Script to analyse the output of a successfull run
 * of the PhotonStopper program.
 * 
 * Output is given in a .root file that contains some 1D histograms (state as of 04-07-2017)
 * 
 */

// Reading the output
//
TFile f("/home/mlueckhof/Codes/Projects/Geant/Examples/PhotonStopper/build/plots/histo_50.000000mm_PhSTPhysicsList.root");

// Printing the contents of the file
//
f.ls();

// Drawing certain histograms
//
h1 = (TH1D*)f.Get("targetEdep");
h1->Draw();

h2 = (TH1D*)f.Get("energPhot");
h2->Draw();

// Exiting ROOT
//
.q
