#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
using std::string;
using std::vector;


    Simulation:: Simulation(const string &configFilePath){// בנאי איך??
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
Simulation& Simulation::operator=(const Simulation &other){ // we added copy assingment operator
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        facilitiesOptions = other.facilitiesOptions;
        plans = other.plans;
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
        return *this;   
    } 
}

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy){
    plans.push_back(Plan(planCounter++, *settlement, selectionPolicy, facilitiesOptions));

bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())) {
        return false; 
    }
    settlements.push_back(settlement);
    return true; 


        void start();
        void addAction(BaseAction *action);

        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);

bool Simulation::isSettlementExists(const string &settlementName){
    for (Settlement* currSet : settlements){
        if (currSet->getName() == settlementName){
            return true;}
    }
    return false;


Settlement* Simulation::getSettlement(const string &settlementName){
    for (Settlement* currSet : this->settlements){
         if (currSet->getName() == settlementName){
             return currSet;
         }
    }
 }

    Plan& Simulation::getPlan(const int planID){
        for (Plan& currPlan : this->plans){
            if (currPlan.getPlanID() == planID){
                return currPlan;
            }
        } 
    }
        void step();
        void close();
        void open();

//desrtactor
Simulation:: ~Simulation(){
    for (BaseAction* action : actionsLog){
        delete action;
    }
    for (Settlement *settlement : settlements){
        delete settlement;
    }
}  
