#include "DetectorConstruction.hh"


DetectorConstruction::DetectorConstruction()
{}


DetectorConstruction::~DetectorConstruction()
{}


G4VPhysicalVolume *DetectorConstruction::Construct()
{
    auto sphi = 0.*deg;
    auto dphi = 360.*deg;

    auto worldR = 300.*m;

    auto tankOuterR = 0.5*6*cm;
    auto tankInnerR = 0.5*6*cm;
    auto tankInnerZ = 1*cm;
    auto tankOuterZ = tankInnerZ+0.2*cm;

    G4NistManager *nist = G4NistManager::Instance();

    auto vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    auto target = nist->FindOrBuildMaterial("G4_WATER");
//    auto target = nist->FindOrBuildMaterial("G4_AIR");
    auto tankMat = nist->FindOrBuildMaterial("G4_Al");

    // THE WORLD
    auto worldS = new G4Box("worldS", worldR, worldR, worldR); // name, x/2, y/2, z/2
    auto worldL = new G4LogicalVolume(worldS, vacuum, "worldL"); // solid, material, name
    auto worldP = new G4PVPlacement(0,                // no rotation
                                    G4ThreeVector(),  // at (0, 0, 0)
                                    worldL,           // its logical volume
                                    "worldP",         // its name
                                    0,                // its mother volume
                                    false,            // no boolean operation
                                    0,                // copy number
                                    true);            // check for overlaps
    worldL->SetVisAttributes(G4VisAttributes::GetInvisible());

    // TANK
    auto tankOuterS = new G4Tubs("tankOuterS", 0, tankOuterR, tankOuterZ/2, sphi, dphi);
    auto tankOuterL = new G4LogicalVolume(tankOuterS, tankMat, "tankOuterL");
    new G4PVPlacement(0,
                      //G4ThreeVector(0, 0, -tankOuterZ/2),
                      G4ThreeVector(0, 0, 0),
                      tankOuterL,
                      "tankOuterP",
                      worldL,
                      false,
                      0,
                      true);
    G4VisAttributes *tankOuterVisAtt = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, .2));
    tankOuterVisAtt->G4VisAttributes::SetForceSolid(true);
    tankOuterL->SetVisAttributes(tankOuterVisAtt);

    // TARGET
    auto tankInnerS = new G4Tubs("tankInnerS", 0, tankInnerR, tankInnerZ/2, sphi, dphi);
    auto tankInnerL = new G4LogicalVolume(tankInnerS, target, "tankInnerL");
    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      tankInnerL,
                      "tankInnerP",
                      tankOuterL,
                      false,
                      0,
                      true);
    G4VisAttributes *tankInnerVisAtt = new G4VisAttributes(G4Colour(0., 0., 1., .7));
    tankInnerVisAtt->G4VisAttributes::SetForceSolid(true);
    tankInnerL->SetVisAttributes(tankInnerVisAtt);

    return worldP;
}


void DetectorConstruction::ConstructSDandField()
{
    auto targetSD = new TargetSD("TargetSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(targetSD);
    SetSensitiveDetector("tankInnerL", targetSD);
}

