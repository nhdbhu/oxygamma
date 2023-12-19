#include "RunAction.hh"


RunAction::RunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetNtupleMerging(true);

    analysisManager->CreateNtuple("Detector", "Particles");
    analysisManager->CreateNtupleDColumn("primE");
    analysisManager->CreateNtupleDColumn("T");
    analysisManager->CreateNtupleDColumn("E");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->CreateNtupleIColumn("PDG");
    analysisManager->CreateNtupleIColumn("Det");
    analysisManager->CreateNtupleDColumn("Edepres");
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

