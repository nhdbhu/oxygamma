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

#include "TargetSD.hh"

#include <vector>


class G4Step;
class G4HCofThisEvent;

class TargetSD : public G4VSensitiveDetector
{
public:
    TargetSD(const G4String& name);
    ~TargetSD() override;

    // methods from base class
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
};

