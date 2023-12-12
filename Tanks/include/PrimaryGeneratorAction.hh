#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIdirectory.hh"
#include <mutex>


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* anEvent) override;

    void SetProfileFileName(const G4String& fileName);
    void SetProfileHistName(const G4String& histName);
    void SetFluxFileName(const G4String& fileName);
    void SetFluxHistName(const G4String& histName);
    void SetFluxLowerLimit(G4double newLimit);
    void SetFluxUpperLimit(G4double newLimit);
    void SetTOFLength(G4double newZ);

    G4String fProfileFileName;
    G4String fProfileHistName;
    G4String fFluxFileName;
    G4String fFluxHistName;
    double fLowerLimit;
    double fUpperLimit;
    double fTOFLength;

private:
    G4ParticleGun* particleGun = nullptr;

    TFile* profileFile;
    TH2D* profile2D;
    TFile* fluxFile;
    TH1D* flux;
    TH1D* fluxRange;

    class PrimaryGeneratorActionMessenger;
    PrimaryGeneratorActionMessenger* fMessenger;
    void InitializeProfile();
    void InitializeFlux();
};


class PrimaryGeneratorAction::PrimaryGeneratorActionMessenger : public G4UImessenger {
public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* generatorAction);
    ~PrimaryGeneratorActionMessenger() override;

    void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    PrimaryGeneratorAction* fGeneratorAction;
    G4UIcmdWithAString* fProfileFileNameCmd;
    G4UIcmdWithAString* fProfileHistNameCmd;
    G4UIcmdWithAString* fFluxFileNameCmd;
    G4UIcmdWithAString* fFluxHistNameCmd;
    G4UIcmdWithADouble* fFluxLowerLimitCmd;
    G4UIcmdWithADouble* fFluxUpperLimitCmd;
    G4UIcmdWithADouble* fTOFLengthCmd;
};

