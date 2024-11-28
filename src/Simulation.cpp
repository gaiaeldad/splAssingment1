#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
using std::string;
using std::vector;


    Simulation:: Simulation(const string &configFilePath){

    }

        //copy constractor
Simulation::Simulation(const Simulation &other):
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    plans(other.plans),
    facilitiesOptions(other.facilitiesOptions),
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }


        void start();
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);


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
