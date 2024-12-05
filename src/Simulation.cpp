#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include "Auxiliary.h"
#include <string>
using std::string;
using std::vector;
using std::cout;
using std::endl;


SettlementType Simulation:: intToSettlementType(int type) {//we added this - local use 
    switch (type) {
        case 0: return SettlementType::VILLAGE;
        case 1: return SettlementType::CITY;
        case 2: return SettlementType::METROPOLIS;
        default: throw std::invalid_argument("Invalid SettlementType: " + std::to_string(type));
    }
}
FacilityCategory Simulation:: intToFacilityCategory(int category) { // we added
    switch (category) {
        case 0: return FacilityCategory::LIFE_QUALITY;
        case 1: return FacilityCategory::ECONOMY;
        case 2: return FacilityCategory::ENVIRONMENT;
        default: throw std::invalid_argument("Invalid FacilityCategory: " + std::to_string(category));
    }
}


Simulation::Simulation(const string &configFilePath){
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Error: Could not open config file at " + configFilePath);
    }

    isRunning = false;
    planCounter = 0;

    std::string line;
    while (std::getline(configFile, line)) {
        // Ignore empty lines or lines starting with #
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse the line using the Auxiliary method
        std::vector<std::string> args = Auxiliary::parseArguments(line);

        // Check the prefix (first element in the vector)
        const std::string& prefix = args[0];

        if (prefix == "settlement") {
            if (args.size() < 3) continue;  // Skip invalid lines
            std::string settlementName = args[1];
            int settlementTypeInt = std::stoi(args[2]);  // Convert type from string to int
            SettlementType settlementType = intToSettlementType(settlementTypeInt);
            // Create a new Settlement object
            Settlement* settlement = new Settlement(settlementName, settlementType);
            settlements.push_back(settlement);

        } else if (prefix == "facility") {
            if (args.size() < 6) continue;  // Skip invalid lines
            std::string facilityName = std::string(args[1]);
            int categoryint = std::stoi(args[2]);
            FacilityCategory category = intToFacilityCategory(categoryint);
            int price = std::stoi(args[3]);
            int lifeqImpact = std::stoi(args[4]);
            int ecoImpact = std::stoi(args[5]);
            int envImpact = std::stoi(args[6]);
            // Create a new FacilityType object
            FacilityType facility(facilityName, category, price, lifeqImpact, ecoImpact, envImpact);
            facilitiesOptions.push_back(facility);

        } else if (prefix == "plan") {
            if (args.size() < 3) continue;  // Skip invalid lines
            std::string settlementName = args[1];
            std::string selectionPolicy = args[2];
            // Find the corresponding Settlement
            Settlement* settlement = getSettlement(settlementName);
            if (settlement != nullptr) {

                // Create the Plan object and add it to the plans vector
                std::string policyType = args[2];
                SelectionPolicy* policy = SelectionPolicy::createNewSP(policyType);
                addPlan(settlement, policy);  // This function assumes addPlan is already implemented
            }
        }
    }
     configFile.close();
}

// copy constructor
Simulation::Simulation(const Simulation &other): 
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    facilitiesOptions(other.facilitiesOptions){ 
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for (Settlement* curr : other.settlements) {
        settlements.push_back(new Settlement(*curr));
    }
    for(Plan otherPlan : other.plans){
        string settlementName = otherPlan.getSettlement().getName();
        Settlement *currSettlement = getSettlement(settlementName); 
        Plan newPlan(otherPlan.getPlanID(), *currSettlement, otherPlan.getSelectionPolicySP(), otherPlan.getFacilityOptions());
        plans.push_back(newPlan);

    }
    planCounter = other.planCounter;
    isRunning = other.isRunning; 
    facilitiesOptions = other.facilitiesOptions;  
}


Simulation& Simulation::operator= (const Simulation &other){ // we added copy assingment operator
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        facilitiesOptions = other.facilitiesOptions;
         for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear(); 
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        } 
         for (Settlement* currSet : settlements) {
            delete currSet;
        }
        settlements.clear(); 
        for (Settlement* currSet : other.settlements) {
            settlements.push_back(new Settlement(*currSet));
        } 
        for(Plan otherPlan : other.plans){
            string settlementName = otherPlan.getSettlement().getName();
            Settlement *currSettlement = getSettlement(settlementName); 
            Plan newPlan(otherPlan.getPlanID(), *currSettlement, otherPlan.getSelectionPolicySP(), otherPlan.getFacilityOptions());
            plans.push_back(newPlan);
        }     
        return *this;   
    } 
}

 void Simulation::start() {
    cout << "The Simulation has started" << endl;
    isRunning = true;

    while (isRunning) {
        try {
            cout << "-> "; // Prompt the user
            std::string input;
            std::getline(std::cin, input); // Read user input
            std::istringstream iss(input);
            std::string command;
            iss >> command; // Extract the first token as the command

            if (command == "step") {
                int numOfSteps;
                iss >> numOfSteps;
                if (iss.fail() || numOfSteps <= 0) {
                    throw std::runtime_error("Invalid input for step");
                }
                SimulateStep action(numOfSteps);
                action.act(*this);
            } 
            else if (command == "plan") {
                std::string settlementName, selectionPolicy;
                iss >> settlementName >> selectionPolicy;
                if (settlementName.empty() || selectionPolicy.empty()) {
                    throw std::runtime_error("Invalid input for plan");
                }
                AddPlan action(settlementName, selectionPolicy);
                action.act(*this);
            } 
            else if (command == "settlement") {
                std::string settlementName;
                int settlementType;
                iss >> settlementName >> settlementType;
                if (settlementName.empty() || iss.fail()||settlementType<0|| settlementType>2) {
                    throw std::runtime_error("Invalid input for settlement");
                }
                AddSettlement action(settlementName, static_cast<SettlementType>(settlementType));
                action.act(*this);
            } 
            else if (command == "facility") {
                std::string facilityName;
                int category, price, lifeQ, economy, environment;
                iss >> facilityName >> category >> price >> lifeQ >> economy >> environment;
                if (facilityName.empty() || iss.fail()|| price<0||lifeQ<0||economy<0||environment<0) {
                    throw std::runtime_error("Invalid input for facility");
                }
                AddFacility action(facilityName, static_cast<FacilityCategory>(category), price, lifeQ, economy, environment);
                action.act(*this);
            } 
            else if (command == "planStatus") {
                int planId;
                iss >> planId;
                if (iss.fail()) {
                    throw std::runtime_error("Invalid input for planStatus");
                }
                PrintPlanStatus action(planId);
                action.act(*this);
            } 
            else if (command == "changePolicy") {
                int planId;
                std::string newPolicy;
                iss >> planId >> newPolicy;
                if (iss.fail() || newPolicy.empty()) {
                    throw std::runtime_error("Invalid input for changePolicy");
                }
                ChangePlanPolicy action(planId, newPolicy);
                action.act(*this);
            } 
            else if (command == "log") {
                PrintActionsLog action;
                action.act(*this);
            } 
            else if (command == "backup") {
                BackupSimulation action;
                action.act(*this);
            } 
            else if (command == "restore") {
                RestoreSimulation action;
                action.act(*this);
            } 
            else if (command == "close") {
                Close action;
                action.act(*this);
                isRunning = false; // Exit the loop
            } 
            else {
                throw std::runtime_error("Unknown command");
            }
        } catch (const std::runtime_error &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy){
     plans.push_back(Plan(planCounter, *settlement, selectionPolicy, facilitiesOptions));
     planCounter++;
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}


bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())) {
        return false; 
    }
    settlements.push_back(settlement);
    return true; 
}
bool Simulation::isSettlementExists(const string &settlementName){
    for (Settlement* currSet : settlements){
        if (currSet->getName() == settlementName){
            return true;}
    }
    return false;
}

 bool Simulation::addFacility(FacilityType facility){

     if (!doesFacilityExists(facility.getName())){
        facilitiesOptions.push_back(facility);
        return true;
     }
    return false;
}

bool Simulation::doesFacilityExists(const string &facilityName){
  
    for (const FacilityType currFac : facilitiesOptions){
        if (currFac.getName() == facilityName){
        return true;
        }
    }
    return false; 

}

Settlement* Simulation::getSettlement(const string &settlementName){
    for (Settlement* currSet : this->settlements){
         if (currSet->getName() == settlementName){
             return currSet;
         }
    }
    return nullptr;
 }

Plan& Simulation::getPlan(const int planID){
     return plans[planID];
}

 void Simulation::step(){
    for (Plan& planrunning : plans){
         planrunning.step();
    }
}

 void Simulation::close(){
    for ( const Plan& planrunning : plans){
         cout<< planrunning.closeToString()<< endl;
    }
    isRunning = false;
}


void Simulation::open(){
    isRunning = true; 
}


//desrtactor
Simulation:: ~Simulation(){
    for (BaseAction* action : actionsLog){
        delete action;
    }
     actionsLog.clear();
    for (Settlement *settlement : settlements){
        delete settlement;
    }
    settlements.clear();
}


int Simulation:: getplanCounter() const{
    return planCounter;
}

vector<BaseAction*> Simulation::getActionLog(){
    return actionsLog;
}

