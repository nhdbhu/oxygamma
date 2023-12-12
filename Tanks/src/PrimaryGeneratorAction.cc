#include "PrimaryGeneratorAction.hh"


std::mutex fileMutex;

PrimaryGeneratorAction::PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* generatorAction) : G4UImessenger(), fGeneratorAction(generatorAction) {
    fProfileFileNameCmd = new G4UIcmdWithAString("/generator/setProfileFileName", this);
    fProfileFileNameCmd->SetGuidance("Set the input profile file name");
    fProfileFileNameCmd->SetParameterName("fileName", false);
    fProfileFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fProfileHistNameCmd = new G4UIcmdWithAString("/generator/setProfileHistName", this);
    fProfileHistNameCmd->SetGuidance("Set the input profile histogram name");
    fProfileHistNameCmd->SetParameterName("histName", false);
    fProfileHistNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFluxFileNameCmd = new G4UIcmdWithAString("/generator/setFluxFileName", this);
    fFluxFileNameCmd->SetGuidance("Set the input flux file name");
    fFluxFileNameCmd->SetParameterName("fileName", false);
    fFluxFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFluxHistNameCmd = new G4UIcmdWithAString("/generator/setFluxHistName", this);
    fFluxHistNameCmd->SetGuidance("Set the input flux histogram name");
    fFluxHistNameCmd->SetParameterName("histName", false);
    fFluxHistNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFluxLowerLimitCmd = new G4UIcmdWithADouble("/generator/setFluxLowerLimit", this);
    fFluxLowerLimitCmd->SetGuidance("Set the input flux lower limit in MeV");
    fFluxLowerLimitCmd->SetParameterName("energyInMeV", false);
    fFluxLowerLimitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFluxUpperLimitCmd = new G4UIcmdWithADouble("/generator/setFluxUpperLimit", this);
    fFluxUpperLimitCmd->SetGuidance("Set the input flux upper limit in MeV");
    fFluxUpperLimitCmd->SetParameterName("energyInMeV", false);
    fFluxUpperLimitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTOFLengthCmd = new G4UIcmdWithADouble("/generator/setTOFLength", this);
    fTOFLengthCmd->SetGuidance("Set the TOF length in m)");
    fTOFLengthCmd->SetParameterName("lengthInm", false);
    fTOFLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}


PrimaryGeneratorAction::PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger() {
    delete fProfileFileNameCmd;
    delete fProfileHistNameCmd;
    delete fFluxFileNameCmd;
    delete fFluxHistNameCmd;
    delete fFluxLowerLimitCmd;
    delete fFluxUpperLimitCmd;
}


void PrimaryGeneratorAction::PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fProfileFileNameCmd) {
        fGeneratorAction->SetProfileFileName(newValue);
    } else if (command == fProfileHistNameCmd) {
        fGeneratorAction->SetProfileHistName(newValue);
    } else if (command == fFluxFileNameCmd) {
        fGeneratorAction->SetFluxFileName(newValue);
    } else if (command == fFluxHistNameCmd) {
        fGeneratorAction->SetFluxHistName(newValue);
    } else if (command == fFluxLowerLimitCmd) {
        G4double newLimit = std::stod(newValue);
        fGeneratorAction->SetFluxLowerLimit(newLimit*1e6); // newLimit in MeV -> eV
    } else if (command == fFluxUpperLimitCmd) {
        G4double newLimit = std::stod(newValue);
        fGeneratorAction->SetFluxUpperLimit(newLimit*1e6); // newLimit in MeV -> eV
    } else if (command == fTOFLengthCmd) {
        G4double newZ = std::stod(newValue);
        fGeneratorAction->SetTOFLength(newZ);
    }
}


PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fMessenger(nullptr) {
    G4int nofParticles = 1;
    particleGun = new G4ParticleGun(nofParticles);

    auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    particleGun->SetParticleDefinition(particleDefinition);

    std::lock_guard<std::mutex> lock(fileMutex);

    // default parameters
    fProfileFileName = "EAR1_196.0_E1_1.e-3_E2_1.e9.root";
    fProfileHistName = "histfluka";
    fFluxFileName = "plots_Comp.root";
    fFluxHistName = "hFlux_EAR1";
    fLowerLimit = 80e6;
    fUpperLimit = 90e6;
    fTOFLength = 196;

    fMessenger = new PrimaryGeneratorActionMessenger(this);

    InitializeProfile();
    InitializeFlux();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        profileFile->Close();
        delete profileFile;
        delete fluxFile;
    }
    delete particleGun;
}


void PrimaryGeneratorAction::InitializeProfile() {
    profileFile = new TFile(fProfileFileName, "READ");
    profile2D = dynamic_cast<TH2D*>(profileFile->Get(fProfileHistName));
}


void PrimaryGeneratorAction::InitializeFlux() {
    fluxFile = new TFile(fFluxFileName, "READ");
    flux = dynamic_cast<TH1D*>(fluxFile->Get(fFluxHistName));
    fluxRange = dynamic_cast<TH1D*>(flux->Clone("fluxRange"));
    for (int i = 1; i <= fluxRange->GetNbinsX(); ++i) {
        double binCenter = fluxRange->GetBinCenter(i);
        if (binCenter < fLowerLimit || binCenter > fUpperLimit) {
            fluxRange->SetBinContent(i, 0.0);
        }
    }
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    double x, y, z;
    profile2D->GetRandom2(x, y);
    z = fTOFLength * m;
    double energy = fluxRange->GetRandom();
    //G4cout << energy << G4endl;

    particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
    particleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    particleGun->SetParticleEnergy(energy * eV);
    particleGun->GeneratePrimaryVertex(anEvent);
}


void PrimaryGeneratorAction::SetProfileFileName(const G4String& fileName) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fProfileFileName = fileName.c_str();
    InitializeProfile();
}


void PrimaryGeneratorAction::SetProfileHistName(const G4String& histName) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fProfileHistName = histName.c_str();
    InitializeProfile();
}


void PrimaryGeneratorAction::SetFluxFileName(const G4String& fileName) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fFluxFileName = fileName.c_str();
    InitializeFlux();
}


void PrimaryGeneratorAction::SetFluxHistName(const G4String& histName) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fFluxHistName = histName.c_str();
    InitializeFlux();
}


void PrimaryGeneratorAction::SetFluxLowerLimit(G4double newLimit) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fLowerLimit = newLimit;
    InitializeFlux();
}


void PrimaryGeneratorAction::SetFluxUpperLimit(G4double newLimit) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fUpperLimit = newLimit;
    InitializeFlux();
}


void PrimaryGeneratorAction::SetTOFLength(G4double newZ) {
    fTOFLength = newZ;
}

