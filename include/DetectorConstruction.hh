#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Orb.hh"
#include "G4Torus.hh"

#include "CrystalSD.hh"


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

private:
    void DefineMaterials();

    G4bool fCheckOverlaps;

    G4VPhysicalVolume *ConstructWorld(G4double worldHalfSize);

    void ConstructTank(G4LogicalVolume* motherL);
    void ConstructLaBr3(G4LogicalVolume* motherL, G4double posR, G4double posThetaDeg, G4double crystalD, G4double crystalZ, G4int copyNo);
    void ConstructHPGe(G4LogicalVolume* motherL, G4double posR, G4double posThetaDeg, G4int copyNo);

    G4NistManager *nist;
    G4Material *vacuum, *target, *tankMat, *LaBr3;
    G4Material *mylar, *shellAl, *GeCrystal, *CarbonFiber;
    G4Element *C;

    G4LogicalVolume *activeCrystalL = nullptr;

    virtual void ConstructSDandField();
};

