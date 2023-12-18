#include <iostream>

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

#include "QGSP_INCLXX_HP.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"


// Note on arguments
namespace
{
    void PrintUsage()
    {
        G4cerr << " Usage: sim [-m macro] [-t nThreads] [-s seed]" << G4endl;
        G4cerr << " Note: -t option is available only for multithreaded mode." << G4endl;
    }
}


int main(int argc, char** argv)
{
    // Evaluate arguments
    if (argc > 7)
    {
        PrintUsage();
        return 1;
    }
    G4String macro;
    G4int nThreads = 0;

    time_t StartTime;
    srand(time(&StartTime));
    auto seed = rand();

    for (G4int i=1; i<argc; i=i+2)
    {
        if (G4String(argv[i]) == "-m") macro = argv[i+1];
        else if (G4String(argv[i]) == "-t") nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
        else if (G4String(argv[i]) == "-s") seed = G4UIcommand::ConvertToInt(argv[i+1]);
        else
        {
            PrintUsage();
            return 1;
        }
    }

    CLHEP::HepRandom::setTheSeed(seed);

    // Construct the default run manager
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    if (nThreads > 0)
    {
        runManager->SetNumberOfThreads(nThreads);
    }


    // Default to interactive mode if no macro provided
    G4UIExecutive* ui = 0;
    if (!macro.size())
    {
        ui = new G4UIExecutive(argc, argv);
    }

    runManager->SetUserInitialization(new DetectorConstruction());
    G4VModularPhysicsList *physicsList = new QGSP_INCLXX_HP;
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization());
//    runManager->Initialize();

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();


    // Get the pointer to the UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    // Start interactive or batch mode
    if (macro.size())
    {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);
    } else {
        UImanager->ApplyCommand("/control/execute ui.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    G4cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⣶⠶⠿⠛⠛⠛⠋⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠙⠛⠛⠛⠿⠶⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⠾⠟⠛⠉⣀⣀⡤⠀⠀⠀⠀⠀⠀⠀⢰⣆⠀⠀⠀⠀⣰⡇⠀⠀⠀⠀⠀⠀⠀⢤⣄⣀⡉⠛⠻⠷⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⢀⣠⡾⠟⠋⢀⣠⣴⣾⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣧⣤⣤⣴⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣷⣦⣄⡀⠙⠻⢷⣤⡀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⢀⣴⠟⠉⢀⣴⣾⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣷⣦⡀⠉⠻⣦⡀⠀⠀⠀\n"
           << "⠀⠀⣴⡿⠁⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠈⢻⣦⠀⠀\n"
           << "⠀⣼⠏⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣤⣤⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠹⣧⠀\n"
           << "⢸⡿⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢿⡇\n"
           << "⢸⡇⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢸⡇\n"
           << "⠸⣿⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⣾⡇\n"
           << "⠀⠹⣧⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⢿⣿⣿⣿⣿⠿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⣿⣿⣿⣿⡿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⣼⡟⠀\n"
           << "⠀⠀⠙⣷⣄⠀⠙⢿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠈⠻⡿⠁⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⢻⠟⠁⠀⠀⠀⠀⠙⣿⣿⣿⣿⡿⠋⠀⣠⣾⠏⠀⠀\n"
           << "⠀⠀⠀⠈⠻⢷⣄⡀⠙⠻⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⠟⠋⠀⣠⣾⠟⠁⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⠀⠙⠻⣶⣤⡀⠉⠛⠦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠴⠛⠉⢀⣤⣶⠟⠋⠀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⢶⣦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⡶⠿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠙⠛⠿⠶⢶⣦⣤⣤⣤⣤⣀⣀⣀⣀⣀⣀⣀⣀⣤⣤⣤⣤⣴⣶⠶⠿⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
           << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << G4endl;


    return 0;
}

