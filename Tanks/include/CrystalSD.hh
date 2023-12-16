#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#include "CrystalSD.hh"
#include "CrystalHit.hh"

#include <vector>


class G4Step;
class G4HCofThisEvent;

class CrystalSD : public G4VSensitiveDetector
{
public:
    CrystalSD(const G4String& name);
    ~CrystalSD() override;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

    virtual void Initialize(G4HCofThisEvent* hce) override;

    G4int hitIndex;

private:
    G4double fTime, fEnergy, fEdep;
    G4int fPDG, fDet;
    HitsCollection* hitsCollection = nullptr;
};

