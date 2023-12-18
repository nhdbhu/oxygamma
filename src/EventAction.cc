#include "EventAction.hh"


EventAction::EventAction()
{
    processedDetectors.resize(numDetectors, false);
}


EventAction::~EventAction()
{}


HitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    auto hitsCollection = static_cast<HitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
    return hitsCollection;
}


void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    std::fill(processedDetectors.begin(), processedDetectors.end(), false);
}


void EventAction::EndOfEventAction(const G4Event* event)
{
    auto analysisManager = G4AnalysisManager::Instance();

    for (int i = 1; i <= numDetectors; i++) {
        G4String sdName = "CrystalSD" + std::to_string(i);
        G4String collectionName = sdName + "HitsCollection";
        G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName);
        if (processedDetectors[i]) continue;
        auto hitsCollection = GetHitsCollection(hcID, event);
        if (hitsCollection) {
            for (size_t j = 0; j < hitsCollection->entries(); j++) {
                auto hit = (*hitsCollection)[j];
                if (hit->GetEdep() > 1.5) {
                    analysisManager->FillNtupleDColumn(0, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
                    analysisManager->FillNtupleDColumn(1, hit->GetTime());
                    analysisManager->FillNtupleDColumn(2, hit->GetEnergy());
                    analysisManager->FillNtupleDColumn(3, hit->GetEdep());
                    analysisManager->FillNtupleIColumn(4, hit->GetPDG());
                    analysisManager->FillNtupleIColumn(5, hit->GetDet());
                    analysisManager->FillNtupleIColumn(6, event->GetEventID());
                    analysisManager->AddNtupleRow();
                }
            }
            // mark collection as processed
            processedDetectors[i] = true;
        }
    }
}

