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
                auto energy = hit->GetEnergy();
                if (energy > 0.1) {
                    auto edep = hit->GetEdep();
                    auto det = hit->GetDet();
                    analysisManager->FillNtupleDColumn(0, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
                    analysisManager->FillNtupleDColumn(1, hit->GetTime());
                    analysisManager->FillNtupleDColumn(2, energy);
                    analysisManager->FillNtupleDColumn(3, edep);
                    analysisManager->FillNtupleIColumn(4, hit->GetPDG());
                    analysisManager->FillNtupleIColumn(5, hit->GetDet());
                    if (det != 8) { // LaBr3 - ENSURE CONSISTENCY WITH DETECTORCONSTRUCTION
                        auto sigma = - 1.89e-3 * edep + 1.68e-2 * std::sqrt(edep) - 2.47e-3;
                        analysisManager->FillNtupleDColumn(6, G4RandGauss::shoot(edep, sigma));
                    } else { // HPGe - ENSURE CONSISTENCY WITH DETECTORCONSTRUCTION
                        auto fwhm = 0.000643453 + edep * 0.00104647 - edep * edep * 0.000121787;
                        auto sigma = fwhm/2.354820045031;
                        analysisManager->FillNtupleDColumn(6, G4RandGauss::shoot(edep, sigma));
                    }
                    analysisManager->AddNtupleRow();
                }
            }
            processedDetectors[i] = true;
        }
    }
}

