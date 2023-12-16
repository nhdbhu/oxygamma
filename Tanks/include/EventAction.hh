#pragma once

#include "G4UserEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include <iomanip>
#include "Randomize.hh"
#include "globals.hh"

#include "CrystalSD.hh"


class EventAction : public G4UserEventAction
{
public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void   EndOfEventAction(const G4Event* event) override;

private:
    HitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
    G4int fCrystalHCID = -1;
};

