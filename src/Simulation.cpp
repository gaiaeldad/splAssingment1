#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Action.h"
#include <iostream>
using std::string;
using std::vector;
using std::cout;
using std::endl;

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
            int settlementType = std::stoi(args[2]);  // Convert type from string to int
            // Create a new Settlement object
            Settlement* settlement = new Settlement(settlementName, settlementType);
            settlements.push_back(settlement);

        } else if (prefix == "facility") {
            if (args.size() < 6) continue;  // Skip invalid lines
            std::string facilityName = args[1];
            int category = std::stoi(args[2]);
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
                SelectionPolicy* policy = createNewSP(policyType);
                addPlan(settlement, policy);  // This function assumes addPlan is already implemented
            }
        }
    }
     configFile.close();
}

//copy constractor
Simulation::Simulation(const Simulation &other): //we added copy constractor
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    plans(other.plans),
    facilitiesOptions(other.facilitiesOptions){
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for (Settlement* currSet : other.settlements) {
        settlements.push_back(new Settlement(*currSet));
    }
}
Simulation& Simulation::operator = (const Simulation &other){ // we added copy assingment operator
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        facilitiesOptions = other.facilitiesOptions;
        plans = other.plans;
         for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear(); // do we have this method?
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
        return *this;   
    } 
}
 void Simulation:: start() {
    isRunning = true;
    cout<< "The Simulation has started" <<endl;
 }

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy){// not sure we have the method getpolicy 
//check that is a valid selection policy and that the settelment doesnt allready exsits
    std::vector<std::string> validSelectionPolicies = {"nve", "bal", "eco", "env"};
    bool isValidPolicy = std::find(validSelectionPolicies.begin(), validSelectionPolicies.end(), selectionPolicy->getPolicy()) != validSelectionPolicies.end();
    
    if (!isValidPolicy || isSettlementExists(settlement->getName())) {
        throw std::out_of_range("Cannot create this plan");
    }
     plans.push_back(Plan(planCounter++, *settlement, selectionPolicy, facilitiesOptions));
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

     if (doesFacilityExists(facility.getName())){
         throw std::out_of_range("Facility already exists");
     }
             facilitiesOptions.push_back(facility);
            return true;
}

bool Simulation::doesFacilityExists(const string &facilityName){
  
    for (const FacilityType curr : facilitiesOptions){
        if (curr.getName() == facilityName){
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
    for (Plan& currPlan : this->plans){
        if (currPlan.getPlanID() == planID){
            return currPlan;
        }
    } 
    return nullptr;
}

 void Simulation::step(){//not sure if this is correct 
    open();
    for (Plan& planrunning : plans){
         planrunning.step();
    }
    close();
}

 void Simulation::close(){
    for ( const Plan& planrunning : plans){
         cout<< planrunning.toString()<< endl;
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
    for (Settlement *settlement : settlements){
        delete settlement;
    }
}

int Simulation:: getplanCounter() const{
    return planCounter;
}

vector<BaseAction*> Simulation::getActionLog(){
    return actionsLog;
}
