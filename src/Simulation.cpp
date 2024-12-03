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


SettlementType intToSettlementType(int type) {//we added this - local use 
    switch (type) {
        case 0: return SettlementType::VILLAGE;
        case 1: return SettlementType::CITY;
        case 2: return SettlementType::METROPOLIS;
        default: throw std::invalid_argument("Invalid SettlementType: " + std::to_string(type));
    }
}
FacilityCategory intToFacilityCategory(int category) {
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
 void Simulation:: start() {// need to write this 
    isRunning = true;
    cout<< "The Simulation has started" <<endl;
    //need to loop here
    //try chatch - חריגות אם מריצים ובמקום שהכל יקרוס שיגד לי מה הבעיה בתוך הלולאה יש משהו שנקרא טרי קאטץ לעשות בכל שלב שאם לא תקין שיזרוק הערה  
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
     throw std::out_of_range("Plan with the given ID not found.");
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

