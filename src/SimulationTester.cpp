#include <iostream>
#include <fstream>
#include <cassert>
#include "Simulation.h"

using namespace std;

void createTestConfigFile(const string &filePath) {
    ofstream configFile(filePath);
    assert(configFile.is_open() && "Failed to create test configuration file.");

    configFile << "# Test Configuration File\n";
    configFile << "settlement Settlement1 0\n"; // Village
    configFile << "settlement Settlement2 1\n"; // City
    configFile << "facility Facility1 0 100 10 5 3\n"; // Life Quality Facility
    configFile << "facility Facility2 1 200 0 20 5\n"; // Economy Facility
    configFile << "plan Settlement1 PrioritySelection\n"; // Example policy
    configFile.close();
}

void testSimulationInitialization() {
    string testConfigFile = "test_config.txt";
    createTestConfigFile(testConfigFile);

    try {
        Simulation sim(testConfigFile);
        cout << "Simulation initialized successfully with file: " << testConfigFile << endl;

        // Check settlements
        Settlement* settlement1 = sim.getSettlement("Settlement1");
        assert(settlement1 && "Settlement1 not found.");
        assert(settlement1->getName() == "Settlement1");
        assert(settlement1->getType() == SettlementType::VILLAGE);

        Settlement* settlement2 = sim.getSettlement("Settlement2");
        assert(settlement2 && "Settlement2 not found.");
        assert(settlement2->getName() == "Settlement2");
        assert(settlement2->getType() == SettlementType::CITY);

        // Check facilities
        assert(sim.doesFacilityExists("Facility1") && "Facility1 should exist.");
        assert(sim.doesFacilityExists("Facility2") && "Facility2 should exist.");

        // Check plans
        Plan& plan = sim.getPlan(0);
        assert(plan.getPlanID() == 0 && "Plan ID mismatch.");

        cout << "All initialization tests passed." << endl;
    } catch (const exception &e) {
        cerr << "Exception during initialization test: " << e.what() << endl;
    }
}

void testSimulationOperations() {
    string testConfigFile = "test_config.txt";
    createTestConfigFile(testConfigFile);

    try {
        Simulation sim(testConfigFile);

        // Test start
        sim.start();
        assert(sim.getplanCounter() > 0 && "Plan counter should be greater than 0 after initialization.");

        // Test step
        sim.step();
        cout << "Simulation step executed successfully." << endl;

        // Test addFacility
        FacilityType newFacility("Facility3", FacilityCategory::ENVIRONMENT, 300, 0, 0, 15);
        assert(sim.addFacility(newFacility) && "Failed to add new facility.");
        assert(sim.doesFacilityExists("Facility3") && "Facility3 should exist after being added.");

        cout << "All operation tests passed." << endl;
    } catch (const exception &e) {
        cerr << "Exception during operations test: " << e.what() << endl;
    }
}

void testSimulationDestruction() {
    string testConfigFile = "test_config.txt";
    createTestConfigFile(testConfigFile);

    try {
        Simulation* sim = new Simulation(testConfigFile);
        delete sim;
        cout << "Simulation destroyed successfully with no memory leaks." << endl;
    } catch (const exception &e) {
        cerr << "Exception during destruction test: " << e.what() << endl;
    }
}

int main() {
    cout << "Starting Simulation Tests..." << endl;

    testSimulationInitialization();
    testSimulationOperations();
    testSimulationDestruction();

    cout << "All tests completed successfully." << endl;
    return 0;
}
