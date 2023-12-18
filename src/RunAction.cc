#include "RunAction.hh"


RunAction::RunAction()
{
    // set printing event number per each event
    //G4RunManager::GetRunManager()->SetPrintProgress(1);

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetNtupleMerging(true);

    analysisManager->CreateNtuple("Detector", "Particles");
    analysisManager->CreateNtupleDColumn("primE");
    analysisManager->CreateNtupleDColumn("T");
    analysisManager->CreateNtupleDColumn("E");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->CreateNtupleIColumn("PDG");
    analysisManager->CreateNtupleIColumn("Det");
    analysisManager->CreateNtupleIColumn("ID");
    analysisManager->FinishNtuple(0);
}


RunAction::~RunAction()
{}


void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile();
}


void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}

