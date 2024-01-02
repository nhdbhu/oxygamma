#include "CrystalSD.hh"


CrystalSD::CrystalSD(const G4String& name) : G4VSensitiveDetector(name)
{
    collectionName.insert(name + "HitsCollection");
}


CrystalSD::~CrystalSD()
{}


void CrystalSD::Initialize(G4HCofThisEvent* hce) {
    G4String hcName = SensitiveDetectorName + "HitsCollection";
    hitsCollection = new HitsCollection(SensitiveDetectorName, hcName);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
    hce->AddHitsCollection(hcID, hitsCollection);
}


G4bool CrystalSD::ProcessHits(G4Step *aStep, G4TouchableHistory*)
{
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    //if (preStepPoint->GetStepStatus() == fGeomBoundary && postStepPoint->GetPhysicalVolume()->GetName() != "worldP") {
    if (hitsCollection->GetSize() == 0) {
        //G4cout << "Enters detector " << preStepPoint->GetTouchable()->GetCopyNumber() << G4endl;
        CrystalHit* newHit = new CrystalHit();
        newHit->SetTime(preStepPoint->GetGlobalTime());
        newHit->SetEnergy(preStepPoint->GetKineticEnergy());
        newHit->SetEdep(0);
        newHit->SetPDG(track->GetDefinition()->GetPDGEncoding());
        newHit->SetDet(preStepPoint->GetTouchable()->GetCopyNumber());
        hitsCollection->insert(newHit);
    }

    edep = aStep->GetTotalEnergyDeposit();
    if (edep > 0) {
        CrystalHit* hit = (*hitsCollection)[0];
        if (hit) {
            hit->AddEdep(edep);
        }
    }

    return true;
}

