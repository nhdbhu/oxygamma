#include "CrystalSD.hh"


CrystalSD::CrystalSD(const G4String& name) : G4VSensitiveDetector(name)
{
    collectionName.insert(name + "HitsCollection");
}


CrystalSD::~CrystalSD()
{
    //delete hitsCollection;
}


void CrystalSD::Initialize(G4HCofThisEvent* hce) {
    G4String hcName = SensitiveDetectorName + "HitsCollection";
    hitsCollection = new HitsCollection(SensitiveDetectorName, hcName);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
    hce->AddHitsCollection(hcID, hitsCollection);

    // TO DO: move to ProcessHits then use timing to create hits & allocate hitIndex
    hitsCollection->insert(new CrystalHit());
    hitIndex = 0;
}


G4bool CrystalSD::ProcessHits(G4Step *aStep, G4TouchableHistory*)
{
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    if (preStepPoint->GetStepStatus() == fGeomBoundary && postStepPoint->GetPhysicalVolume()->GetName() != "worldP") {
        //G4cout << "Enters detector " << preStepPoint->GetTouchable()->GetCopyNumber() << G4endl;
        CrystalHit* hit = (*hitsCollection)[hitIndex];
        if (hit) {
            hit->SetTime(preStepPoint->GetGlobalTime());
            hit->SetEnergy(preStepPoint->GetKineticEnergy());
            hit->SetEdep(0);
            hit->SetPDG(track->GetDefinition()->GetPDGEncoding());
            hit->SetDet(preStepPoint->GetTouchable()->GetCopyNumber());
        }
    }

    edep = aStep->GetTotalEnergyDeposit();
    if (edep > 0) {
        CrystalHit* hit = (*hitsCollection)[hitIndex];
        if (hit) {
            hit->AddEdep(edep);
        }
    }

    return true;
}


