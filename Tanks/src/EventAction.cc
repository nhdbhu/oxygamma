#include "EventAction.hh"


EventAction::EventAction()
{}


EventAction::~EventAction()
{}


HitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    auto hitsCollection = static_cast<HitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
    return hitsCollection;
}


void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{}


void EventAction::EndOfEventAction(const G4Event* event)
{
    auto analysisManager = G4AnalysisManager::Instance();

    std::set<G4int> processedCollections;
    const int numDetectors = 16; // ENSURE THIS MATCHES DETECTORCONSTRUCTION

    for (int i = 1; i <= numDetectors; i++) {
        G4String sdName = "CrystalSD" + std::to_string(i);
        G4String collectionName = sdName + "HitsCollection";
        G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName);
        if (processedCollections.find(hcID) == processedCollections.end()) {
            // collection has not been processed yet
            auto hitsCollection = GetHitsCollection(hcID, event);
            if (hitsCollection) {
                for (size_t j = 0; j < hitsCollection->entries(); j++) {
                    if ((*hitsCollection)[j]->GetEdep() > 0) {
                        analysisManager->FillNtupleDColumn(0, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
                        analysisManager->FillNtupleDColumn(1, (*hitsCollection)[j]->GetTime());
                        analysisManager->FillNtupleDColumn(2, (*hitsCollection)[j]->GetEnergy());
                        analysisManager->FillNtupleDColumn(3, (*hitsCollection)[j]->GetEdep());
                        analysisManager->FillNtupleIColumn(4, (*hitsCollection)[j]->GetPDG());
                        analysisManager->FillNtupleIColumn(5, (*hitsCollection)[j]->GetDet());
                        analysisManager->AddNtupleRow();
                    }
                }
                // mark collection as processed
                processedCollections.insert(hcID);
            }
        }
    }


}
