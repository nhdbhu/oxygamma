#include "TargetSD.hh"


TargetSD::TargetSD(const G4String& name) : G4VSensitiveDetector(name)
{}


TargetSD::~TargetSD()
{}


G4bool TargetSD::ProcessHits(G4Step *aStep, G4TouchableHistory*)
{
    G4Track *track = aStep->GetTrack();
    //G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    auto event = G4EventManager::GetEventManager()->GetConstCurrentEvent();
    if (postStepPoint->GetStepStatus() == fGeomBoundary) {
        //G4cout << "Step ends on geometry boundary" << G4endl;
        auto fPDG = track->GetDefinition()->GetPDGEncoding();
        if (fPDG == 22) {
            G4ThreeVector position = postStepPoint->GetPosition();
            G4ThreeVector momentum = postStepPoint->GetMomentum();
            auto analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillNtupleDColumn(0, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
            analysisManager->FillNtupleDColumn(1, postStepPoint->GetGlobalTime());
            analysisManager->FillNtupleDColumn(2, position.x());
            analysisManager->FillNtupleDColumn(3, position.y());
            analysisManager->FillNtupleDColumn(4, position.z());
            analysisManager->FillNtupleDColumn(5, postStepPoint->GetKineticEnergy());
            analysisManager->FillNtupleDColumn(6, momentum.x());
            analysisManager->FillNtupleDColumn(7, momentum.y());
            analysisManager->FillNtupleDColumn(8, momentum.z());
            analysisManager->FillNtupleIColumn(9, fPDG);
            analysisManager->AddNtupleRow();
        }
        track->SetTrackStatus(fKillTrackAndSecondaries);
    }

    return true;
}


