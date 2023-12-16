#include "DetectorConstruction.hh"


DetectorConstruction::DetectorConstruction() : fCheckOverlaps(true)
{
    nist = G4NistManager::Instance();
}


DetectorConstruction::~DetectorConstruction()
{}


G4VPhysicalVolume *DetectorConstruction::Construct()
{
    DefineMaterials();

	auto theWorldP = this->ConstructWorld(300.*m);
    auto theWorldL = theWorldP->GetLogicalVolume();

    this->ConstructTank(theWorldL);

    this->ConstructLaBr3(theWorldL, 100*mm, 75, 38.1*mm, 50.8*mm, 1);
    this->ConstructLaBr3(theWorldL, 200*mm, 75, 38.1*mm, 50.8*mm, 2);
    this->ConstructLaBr3(theWorldL, 300*mm, 75, 38.1*mm, 50.8*mm, 3);
    this->ConstructLaBr3(theWorldL, 400*mm, 75, 38.1*mm, 50.8*mm, 4);
    this->ConstructLaBr3(theWorldL, 500*mm, 75, 38.1*mm, 50.8*mm, 5);

    this->ConstructLaBr3(theWorldL, 100*mm, -75, 38.1*mm, 38.1*mm, 6);
    this->ConstructLaBr3(theWorldL, 200*mm, -75, 38.1*mm, 38.1*mm, 7);
    this->ConstructLaBr3(theWorldL, 300*mm, -75, 38.1*mm, 38.1*mm, 8);
    this->ConstructLaBr3(theWorldL, 400*mm, -75, 38.1*mm, 38.1*mm, 9);
    this->ConstructLaBr3(theWorldL, 500*mm, -75, 38.1*mm, 38.1*mm, 10);

    this->ConstructLaBr3(theWorldL, 100*mm, 45, 38.1*mm, 25.4*mm, 11);
    this->ConstructLaBr3(theWorldL, 200*mm, 45, 38.1*mm, 25.4*mm, 12);
    this->ConstructLaBr3(theWorldL, 300*mm, 45, 38.1*mm, 25.4*mm, 13);
    this->ConstructLaBr3(theWorldL, 400*mm, 45, 38.1*mm, 25.4*mm, 14);
    this->ConstructLaBr3(theWorldL, 500*mm, 45, 38.1*mm, 25.4*mm, 15);

    this->ConstructHPGe(theWorldL, 175*mm, -45, 16);
    this->ConstructHPGe(theWorldL, 325*mm, -45, 17);
    this->ConstructHPGe(theWorldL, 475*mm, -45, 18);

    return theWorldP;
}


void DetectorConstruction::DefineMaterials()
{ 
    vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    target = nist->FindOrBuildMaterial("G4_WATER");
    //air = nist->FindOrBuildMaterial("G4_AIR");
    tankMat = nist->FindOrBuildMaterial("G4_Al");

    // Lanthanum Bromide, LaBr3
    // https://pdg.lbl.gov/2023/AtomicNuclearProperties/HTML/lanthanum_bromide.html
    LaBr3 = new G4Material("LaBr3", 5.29 * g / cm3, 2);
    LaBr3->AddElement(nist->FindOrBuildElement("La"), 1);
    LaBr3->AddElement(nist->FindOrBuildElement("Br"), 3);
    LaBr3->GetIonisation()->SetMeanExcitationEnergy(454.5 * eV);

    C = new G4Element("Carbon", "C", 6., 12.01*g/mole);
    CarbonFiber = new G4Material("CarbonFiber", 0.145*g/cm3, 1);
    CarbonFiber->AddElement(C, 1);
    mylar = nist->FindOrBuildMaterial("G4_MYLAR");
    shellAl = nist->FindOrBuildMaterial("G4_Al");
    GeCrystal = nist->FindOrBuildMaterial("G4_Ge", false);
}


G4VPhysicalVolume* DetectorConstruction::ConstructWorld(G4double worldHalfSize)
{
    auto worldS = new G4Box("worldS", worldHalfSize, worldHalfSize, worldHalfSize); // name, x/2, y/2, z/2
    auto worldL = new G4LogicalVolume(worldS, vacuum, "worldL"); // solid, material, name
    auto worldP = new G4PVPlacement(0,                // no rotation
                                    G4ThreeVector(),  // at (0, 0, 0)
                                    worldL,           // its logical volume
                                    "worldP",         // its name
                                    0,                // its mother volume
                                    false,            // no boolean operation
                                    0,                // copy number
                                    fCheckOverlaps);  // check for overlaps
    worldL->SetVisAttributes(G4VisAttributes::GetInvisible());

    return worldP;
}


void DetectorConstruction::ConstructTank(G4LogicalVolume* motherL)
{
    auto sphi = 0.*deg;
    auto dphi = 360.*deg;
    auto tankInnerR = 0.5*6*cm;
    auto tankInnerZ = 5*cm;
    auto tankOuterR = tankInnerR+0.4*cm;
    auto tankOuterZ = tankInnerZ+0.2*cm;

    // TANK
    auto tankOuterS = new G4Tubs("tankOuterS", 0, tankOuterR, tankOuterZ/2, sphi, dphi);
    auto tankOuterL = new G4LogicalVolume(tankOuterS, tankMat, "tankOuterL");
    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      tankOuterL,
                      "tankOuterP",
                      motherL,
                      false,
                      0,
                      fCheckOverlaps);
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
                      fCheckOverlaps);
    G4VisAttributes *tankInnerVisAtt = new G4VisAttributes(G4Colour(0., 0., 1., .7));
    tankInnerVisAtt->G4VisAttributes::SetForceSolid(true);
    tankInnerL->SetVisAttributes(tankInnerVisAtt);
}


void DetectorConstruction::ConstructLaBr3(G4LogicalVolume* motherL, G4double posR, G4double posThetaDeg, G4double crystalD, G4double crystalZ, G4int copyNo)
{
    // LABR3 SCINTILLATOR
    G4String detName = "detector" + std::to_string(copyNo);

    // posR: distance from (0, 0, 0) to center of scintillator
    // postThetaDeg: angle between -z and posR line
    auto posTheta = posThetaDeg * M_PI / 180.0;

    auto sphi = 0.*deg;
    auto dphi = 360.*deg;

    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateX(0*deg);
    rotation->rotateY((-posThetaDeg+180)*deg);
    rotation->rotateZ(0*deg);

    auto crystalS = new G4Tubs("crystalS", 0, crystalD/2, crystalZ/2, sphi, dphi);
    activeCrystalL = new G4LogicalVolume(crystalS, LaBr3, detName);
    new G4PVPlacement(rotation,
                      G4ThreeVector(posR*std::sin(posTheta), 0, posR*std::cos(posTheta)),
                      activeCrystalL,
                      "crystalP",
                      motherL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    G4VisAttributes *crystalVisAtt = new G4VisAttributes(G4Colour(1., 0.4, 0., 0.8));
    crystalVisAtt->G4VisAttributes::SetForceSolid(true);
    activeCrystalL->SetVisAttributes(crystalVisAtt);
}


void DetectorConstruction::ConstructHPGe(G4LogicalVolume *motherL, G4double posR, G4double posThetaDeg, G4int copyNo)
{
    // N-TYPE HPGE DETECTOR
    G4String detName = "detector" + std::to_string(copyNo);

    // posR: distance from (0, 0, 0) to center of scintillator
    // postThetaDeg: angle between -z and posR line
    auto posTheta = posThetaDeg * M_PI / 180.0;

    auto sphi = 0.*deg;
    auto dphi = 360.*deg;

    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateX(0*deg);
    rotation->rotateY((-posThetaDeg+180)*deg);
    rotation->rotateZ(0*deg);

    auto crystalRadius = 0.5*57.1*mm; // A
    auto crystalHalfLength = 0.5*60.6*mm; // B
    auto innerDeadLayerThick = 0.7*mm; // N
    auto holeRadius = 0.5*(9.6+0.7) *mm; // C
    auto holeDepth = 52.3*mm; // D
    auto cupLength = 105.*mm; // F
    auto endGap = 11.0*mm; // G
    auto cupTopThick = 0.03*mm; // H
    auto mylarLayerThick = 0.03*mm; // H
    auto shellThick = 1.6*mm; // I
    auto crystalEndRadius = 8.*mm; // J
    auto cupThick = 0.8*mm; // K
    auto outerDeadLayerThick = 0.3*um; // M
    auto shellRadius = 0.5*76.*mm;
    auto shellLength = 120.*mm;
    auto cupBottomThick = 3.*mm;

    // DETECTOR VACUUM
    G4VSolid *HPGe = new G4Tubs("HPGe", 0.*mm, shellRadius, 0.5*shellLength, sphi, dphi);
    G4LogicalVolume *HPGeL = new G4LogicalVolume(HPGe, vacuum, "HPGeL", 0, 0, 0);
    new G4PVPlacement(rotation,
                      G4ThreeVector(posR*std::sin(posTheta), 0, posR*std::cos(posTheta)),
                      HPGeL,
                      "HPGeP",
                      motherL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    G4VisAttributes *HPGeVisAtt = new G4VisAttributes(G4Colour(1., 1., 1., .9));
    HPGeVisAtt->G4VisAttributes::SetForceSolid(true);
    HPGeL->SetVisAttributes(HPGeVisAtt);

    // DETECTOR GEOMETRY CONSTRUCTION
    // full crystal
    G4VSolid *crystal1 = new G4Tubs("cyl1", 0.*mm, crystalRadius-crystalEndRadius, crystalHalfLength, sphi, dphi);
    G4VSolid *crystal2 = new G4Tubs("cyl2", 0.*mm, crystalRadius, crystalHalfLength-0.5*crystalEndRadius, sphi, dphi);
    G4VSolid *tor1 = new G4Torus("tor1", 0.*mm, crystalEndRadius, crystalRadius-crystalEndRadius, sphi, dphi);
    G4VSolid *crystal3 = new G4UnionSolid("cry3", crystal1,crystal2,0,G4ThreeVector(0., 0., -0.5*crystalEndRadius));
    G4VSolid *crystal4 = new G4UnionSolid("cry4", crystal3,tor1,0,G4ThreeVector(0., 0., crystalHalfLength-crystalEndRadius));
    // active crystal
    G4double activeRadius = crystalRadius-outerDeadLayerThick;
    G4double activeHalfLength = crystalHalfLength-0.5*outerDeadLayerThick;
    G4double activeEndRadius = crystalEndRadius-outerDeadLayerThick;
    G4VSolid *activeCrystal1 = new G4Tubs("acyl1", 0.*mm, activeRadius-activeEndRadius, activeHalfLength, sphi, dphi);
    G4VSolid *activeCrystal2 = new G4Tubs("acyl2", 0.*mm, activeRadius, activeHalfLength-0.5*activeEndRadius, sphi, dphi);
    G4VSolid *activeTor1 = new G4Torus("activeTor1", 0.*mm, activeEndRadius, activeRadius-activeEndRadius, sphi, dphi);
    G4VSolid *activeCrystal3 = new G4UnionSolid("cry3", activeCrystal1, activeCrystal2, 0, G4ThreeVector(0., 0., -0.5*activeEndRadius));
    G4VSolid *activeCrystal4 = new G4UnionSolid("cry4", activeCrystal3, activeTor1, 0, G4ThreeVector(0., 0., activeHalfLength-activeEndRadius));
    // outer dead layer
    G4VSolid *outerDeadLayer = new G4SubtractionSolid("outerDeadLayer", crystal4, activeCrystal4 , 0, G4ThreeVector(0., 0., -0.5*outerDeadLayerThick));
    // hole
    G4VSolid *hole1 = new G4Tubs("hole1", 0.*mm, holeRadius, 0.5*(holeDepth-holeRadius), sphi, dphi);
    G4VSolid *hole2 = new G4Orb("hole2",holeRadius);
    G4VSolid *hole = new G4UnionSolid("hole", hole1, hole2, 0, G4ThreeVector(0., 0., 0.5*(holeDepth-holeRadius)));
    // inner dead layer
    G4VSolid *innerDead1 = new G4Tubs("innerDead1", 0.*mm, holeRadius+innerDeadLayerThick, 0.5*(holeDepth-holeRadius), sphi, dphi);
    G4VSolid *innerDead2 = new G4Orb("innerDead2", holeRadius+innerDeadLayerThick);
    G4VSolid *innerDead3 = new G4UnionSolid("innerDead3", innerDead1,innerDead2, 0, G4ThreeVector(0., 0., 0.5*(holeDepth-holeRadius)));
    G4VSolid *innerDeadLayer = new G4SubtractionSolid("innerDeadLayer", innerDead3, hole, 0, G4ThreeVector(0., 0., 0.));
    // final detector shape
    G4VSolid *activeCrystal = new G4SubtractionSolid("activeCrystal", activeCrystal4, innerDead3, 0, G4ThreeVector(0., 0., -activeHalfLength+0.5*(holeDepth-holeRadius)));
    G4LogicalVolume *outerDeadLayerL = new G4LogicalVolume(outerDeadLayer, GeCrystal, "outerDeadLayerL", 0, 0, 0);
    G4LogicalVolume *innerDeadLayerL = new G4LogicalVolume(innerDeadLayer, GeCrystal, "innerDeadLayerL", 0, 0, 0);
    activeCrystalL = new G4LogicalVolume(activeCrystal, GeCrystal, detName, 0, 0, 0);
    // mylar layer
    G4VSolid *mylarLayer = new G4Tubs("mylarLayer", 0.*mm, cupThick+crystalRadius, 0.5*mylarLayerThick, sphi, dphi);
    G4LogicalVolume *mylarLayerL = new G4LogicalVolume(mylarLayer, mylar ,"mylarLayerL", 0, 0, 0);
    // cup
    G4VSolid *cup1 = new G4Tubs("cup1", 0.*mm, cupThick+crystalRadius, 0.5*cupLength, sphi, dphi);
    G4VSolid *cup2 = new G4Tubs("cup2", 0.*mm, crystalRadius, 0.5*(cupLength-cupTopThick-cupBottomThick), sphi, dphi);
    G4VSolid *cup = new G4SubtractionSolid("cup", cup1, cup2, 0, G4ThreeVector(0., 0., 0.5*(cupBottomThick-cupTopThick)));
    G4LogicalVolume *cupL = new G4LogicalVolume(cup, shellAl, "cupL", 0, 0, 0);
    // shell
    G4VSolid *shell1 = new G4Tubs("shell1", 0.*mm, shellRadius, 0.5*shellLength, sphi, dphi);
    G4VSolid *shell2 = new G4Tubs("shell2", 0.*mm, shellRadius-shellThick, 0.5*shellLength-shellThick, sphi, dphi);
    G4VSolid *shell = new G4SubtractionSolid("shell", shell1, shell2, 0, G4ThreeVector(0., 0., 0.));
    G4LogicalVolume *shellL = new G4LogicalVolume(shell, CarbonFiber, "shellL", 0, 0, 0);

    // DETECTOR PLACEMENT
    // outer dead layer
    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., 0.5*shellLength-crystalHalfLength-shellThick-endGap-cupTopThick),
                      outerDeadLayerL,
                      "outerDeadLayerP",
                      HPGeL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    // active crystal
    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., 0.5*shellLength-activeHalfLength-shellThick-endGap-cupTopThick-outerDeadLayerThick),
                      activeCrystalL,
                      "activeCrystalP",
                      HPGeL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    // inner dead layer
    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., 0.5*(shellLength-holeDepth+holeRadius)-shellThick-endGap-cupTopThick-(2.*crystalHalfLength-holeDepth+holeRadius)),
                      innerDeadLayerL,
                      "innerDeadLayerP",
                      HPGeL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    // mylar
    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., 0.5*(shellLength+mylarLayerThick)-shellThick-endGap),
                      mylarLayerL,
                      "mylarLayerP",
                      HPGeL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    // cup
    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., 0.5*(shellLength-cupLength)-shellThick-endGap),
                      cupL,
                      "cupP",
                      HPGeL,
                      false,
                      copyNo,
                      fCheckOverlaps);
    // shell
    new G4PVPlacement(0,
                      G4ThreeVector(0. ,0. , 0.),
                      shellL,
                      "shellP",
                      HPGeL,
                      false,
                      copyNo,
                      fCheckOverlaps);

    // DETECTOR VISUALIZATION ATTRIBUTES
    // active crystal
    G4VisAttributes *activeCrystalVisAtt = new G4VisAttributes(G4Colour(0., 1., 1., 0.3));
    activeCrystalVisAtt->G4VisAttributes::SetForceSolid(true);
    activeCrystalL->SetVisAttributes(activeCrystalVisAtt);
    // outer dead layer
    G4VisAttributes *outerDeadLayerVisAtt = new G4VisAttributes(G4Colour(1., .0, 1., 0.7));
    outerDeadLayerVisAtt->G4VisAttributes::SetForceSolid(true);
    outerDeadLayerL->SetVisAttributes(outerDeadLayerVisAtt);
    // inner dead layer
    G4VisAttributes *innerDeadLayerVisAtt = new G4VisAttributes(G4Colour(1., .0, 1., 0.7));
    innerDeadLayerVisAtt->G4VisAttributes::SetForceSolid(true);
    innerDeadLayerL->SetVisAttributes(innerDeadLayerVisAtt);
    // mylar layer
    G4VisAttributes *mylarLayerVisAtt = new G4VisAttributes(G4Colour(1., 0., 0., .5));
    mylarLayerVisAtt->G4VisAttributes::SetForceSolid(true);
    mylarLayerL->SetVisAttributes(mylarLayerVisAtt);
    // cup
    G4VisAttributes *cupVisAtt = new G4VisAttributes(G4Colour(1., 0., 0., .2));
    cupVisAtt->G4VisAttributes::SetForceSolid(true);
    cupL->SetVisAttributes(cupVisAtt);
    // shell
    G4VisAttributes *shellVisAtt = new G4VisAttributes(G4Colour(.9, .2, .2, .1));
    shellVisAtt->G4VisAttributes::SetForceSolid(true);
    shellL->SetVisAttributes(shellVisAtt);
}


void DetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    const int numDetectors = 16; // ENSURE THIS MATCHES EVENTACTION

    for (int i = 1; i <= numDetectors; i++) {
        G4String sdName = "CrystalSD" + std::to_string(i);
        auto crystalSD = new CrystalSD(sdName);
        sdManager->AddNewDetector(crystalSD);
        SetSensitiveDetector("detector" + std::to_string(i), crystalSD);
    }
}

