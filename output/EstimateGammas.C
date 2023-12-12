// Usage: .x EstimateGammas.C(inputFile, flightL, detPosR, detPosPolDeg, detPosAziDeg, lowerNeutronE, upperNeutronE, lowerGammaE, upperGammaE)

// Input parameter details listed below each function

const double c = 299792458; // speed of light in m/s
const double neutronMass = 939.5654133e6; // neutron mass in eV/c^2

double CalculateNeutronTOF(double KE, double length) {
    // Calculates the TOF of a neutron

    // Input parameters: KE (MeV), flight length (m)

    KE *= 1e6; // eV

    double totalE = neutronMass + KE;
    double velocity = c * TMath::Sqrt(1 - TMath::Power(neutronMass / totalE, 2));
    double TOF = length / velocity; // s

    return TOF * 1e9; // ns
}

void GetExitAngles(double x, double y, double z, double px, double py, double pz, double r, double &theta, double &phi) {
    // Calculates the exit angles of within from a sphere

    // Input parameters: particle position (mm), particle momentum (MeV), sphere radius (mm), angles (rads)

    TVector3 position(x, y, z);
    TVector3 momentum(px, py, pz);
    momentum.Unit();

    double a = momentum.Mag2();
    double b = 2.0 * (position.Dot(momentum));
    double c = position.Mag2() - r * r;

    double discriminant = b * b - 4 * a * c;

    // no intersection if negative, shouldn't happen since particle inside sphere
    if (discriminant < 0) {
        std::cout << "The particle does not intersect with the sphere." << std::endl;
        return;
    }

    double t1 = (- b + sqrt(discriminant)) / (2.0 * a);
    double t2 = (- b - sqrt(discriminant)) / (2.0 * a);
    double t = std::min(t1, t2);

    TVector3 intersection = position + t * momentum;
    theta = intersection.Theta();
    phi = intersection.Phi();

    // TO DO(?): Add correction to TOF (negligible)
}

void EstimateGammas(TString inputFile, double detPosR, double flightL, double detPosPolDeg, double detPosAziDeg, double lowerNeutronE, double upperNeutronE, double lowerGammaE, double upperGammaE) {
    // Calculates the no. of gammas that will enter through the frontal face of a detector using the simulation output
    // Water vessel is centered at (0, 0, 0) and the beam enters through a face along the negative z axis
    // Position of the detector is determined by input parameters
    // Does not account gammas that enter through the detector from its side, nor the path length in the detector

    // Input Paramaters:
    // inputFile, input ROOT file
    // flightL, flight length (m)
    // detPosR, radius from tank center to front center of detector (mm)
    // detPosPolDeg, polar angle of detector (deg)
    // detPosAziDeg, azimuthal angle of detecor (deg)
    // lowerNeutronE, lower neutron energy bound for bin (MeV)
    // upperNeutronE, upper neutron energy bound for bin (MeV)
    // lowerGammaE, lower gamma energy bound to filter (MeV)
    // upperGammaE, upper gamma energy bound to filter (MeV)
/*
    TString inputFile = "EAR1_183.5.root";
    double detPosR = 100;
    double flightL = 183.5;
    double detPosPolDeg = 45;
    double detPosAziDeg = 0;
    double lowerNeutronE = 5;
    double upperNeutronE = 30;
    double lowerGammaE = 2;
    double upperGammaE = 8;
*/

    double lowerTOF = CalculateNeutronTOF(upperNeutronE, flightL);
    double upperTOF = CalculateNeutronTOF(lowerNeutronE, flightL);

    if (detPosAziDeg != 0 && detPosAziDeg != 180) {
        cout << "Detector azimuthal angle should be 0 degree or 180 degrees!" << endl; // TO DO(?): check compatibility for other angles
        return 0;
    }

    double detPosPol = detPosPolDeg * TMath::Pi() / 180.0;
    double detPosAzi = detPosAziDeg * TMath::Pi() / 180.0;

    double detD = 38.1; // detector diameter (mm)
    double angR = TMath::Sqrt( TMath::Power(detPosR, 2) + TMath::Power(detD, 2)); // distance from (0, 0, 0) to frontal edge of the detector (mm)
    double angCoverage = TMath::ATan(0.5 * detD / detPosR); // angle between detPosR and detD (rads)


    double primE, T, X, Y, Z, E, PX, PY, PZ;
    TFile *f = new TFile(inputFile, "READ");
    TTree *t = (TTree*) f->FindObjectAny("Target");

    //t->SetBranchAddress("primE", &primE); // MeV, primary neutron energy (not used)

    // Note: simulation currently filtered to only save information of PDG == 22
    // Parameters of the particle upon exiting the water vessel:
    t->SetBranchAddress("T", &T); // ns, global time
    t->SetBranchAddress("X", &X); // mm, x position
    t->SetBranchAddress("Y", &Y); // mm, y position
    t->SetBranchAddress("Z", &Z); // mm, z position
    t->SetBranchAddress("E", &E); // MeV, kintetic energy
    t->SetBranchAddress("PX", &PX); // MeV, x momentum
    t->SetBranchAddress("PY", &PY); // MeV, y momentum
    t->SetBranchAddress("PZ", &PZ); // MeV, z momentum

    int counts = 0;
    double particlePol, particleAzi;
    for (int i = 0; i < t->GetEntries(); i++) {
        t->GetEntry(i);
        if ((T > lowerTOF) && (T < upperTOF) && (E > lowerGammaE) && (E < upperGammaE)) {
            GetExitAngles(X, Y, Z, PX, PY, PZ, angR, particlePol, particleAzi);
            if (detPosAziDeg == 0) {
                if ( ((detPosPol - angCoverage) < particlePol) && (particlePol < (detPosPol + angCoverage)) && (TMath::Abs(particleAzi) < TMath::Abs(angCoverage)) ) {
                    counts++;
                }
            } else if (detPosAziDeg == 180) {
                if ( ((detPosPol - angCoverage) < particlePol) && (particlePol < (detPosPol + angCoverage)) && (TMath::Abs(particleAzi) > TMath::Abs(TMath::Pi() - angCoverage)) ) {
                    counts++;
                }
            }
        }
    }

    cout << "Gammas: " << counts << endl;
}

