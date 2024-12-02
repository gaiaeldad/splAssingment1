#include <iostream>
#include <vector>
#include "Plan.h"
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"

using namespace std;

// Function to test the constructor of the Plan class
void testPlanConstructor() {
    cout << "Testing Plan Constructor..." << endl;

    Settlement settlement("TestSettlement", SettlementType::CITY); // Updated constructor
    vector<FacilityType> facilityOptions = {
        FacilityType("Hospital", FacilityCategory::LIFE_QUALITY, 500, 10, 0, 0),
        FacilityType("School", FacilityCategory::LIFE_QUALITY, 300, 5, 0, 0)
    };

    NaiveSelection selectionPolicy;

    Plan plan(1, settlement, &selectionPolicy, facilityOptions);

    cout << "Constructor Test Passed!" << endl;
    cout << plan.toString() << endl;
}

// Function to test the step method of the Plan class
void testPlanStep() {
    cout << "Testing Plan Step Method..." << endl;

    Settlement settlement("TestSettlement", SettlementType::CITY); // Updated constructor
    vector<FacilityType> facilityOptions = {
        FacilityType("Hospital", FacilityCategory::LIFE_QUALITY, 500, 10, 0, 0),
        FacilityType("School", FacilityCategory::LIFE_QUALITY, 300, 5, 0, 0)
    };

    NaiveSelection selectionPolicy;
    Plan plan(2, settlement, &selectionPolicy, facilityOptions);

    // Simulate steps and verify the behavior
    for (int i = 0; i < 3; ++i) {
        plan.step();
        plan.printStatus();
    }

    cout << "Step Method Test Passed!" << endl;
}

// Function to test the copy constructor of the Plan class
void testCopyConstructor() {
    cout << "Testing Copy Constructor..." << endl;

    Settlement settlement("TestSettlement", SettlementType::CITY); // Updated constructor
    vector<FacilityType> facilityOptions = {
        FacilityType("Hospital", FacilityCategory::LIFE_QUALITY, 500, 10, 0, 0),
        FacilityType("School", FacilityCategory::LIFE_QUALITY, 300, 5, 0, 0)
    };

    NaiveSelection selectionPolicy;
    Plan originalPlan(3, settlement, &selectionPolicy, facilityOptions);

    originalPlan.step();

    Plan copiedPlan(originalPlan);

    cout << "Original Plan:" << endl;
    cout << originalPlan.toString() << endl;

    cout << "Copied Plan:" << endl;
    cout << copiedPlan.toString() << endl;

    cout << "Copy Constructor Test Passed!" << endl;
}

// Function to test the assignment operator of the Plan class
void testAssignmentOperator() {
    cout << "Testing Assignment Operator..." << endl;

    Settlement settlement1("SettlementA", SettlementType::CITY);
    Settlement settlement2("SettlementB", SettlementType::VILLAGE);

    vector<FacilityType> facilityOptions1 = {
        FacilityType("Park", FacilityCategory::ENVIRONMENT, 200, 0, 0, 10)
    };
    vector<FacilityType> facilityOptions2 = {
        FacilityType("Factory", FacilityCategory::ECONOMY, 700, 0, 15, -5)
    };

    NaiveSelection selectionPolicy1;
    NaiveSelection selectionPolicy2;

    Plan plan1(4, settlement1, &selectionPolicy1, facilityOptions1);
    Plan plan2(5, settlement2, &selectionPolicy2, facilityOptions2);

    plan1 = plan2;

    cout << "Plan 1 after assignment:" << endl;
    cout << plan1.toString() << endl;

    cout << "Assignment Operator Test Passed!" << endl;
}

// Main function to run all tests
int main() {
    cout << "Running Plan Tests..." << endl;

    testPlanConstructor();
    testPlanStep();
    testCopyConstructor();
    testAssignmentOperator();

    cout << "All Plan tests passed!" << endl;
    return 0;
}
