#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;
class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);//yes but problem
        Simulation(const Simulation &other); //we added copy constractor//yes
        Simulation& operator=(const Simulation &other);// we added copy assingment operator//yes
        void start();//yes
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);//yes
        void addAction(BaseAction *action);//no
        bool addSettlement(Settlement *settlement);//yes
        bool addFacility(FacilityType facility);//yes
        bool doesFacilityExists(const string &facilityName);//we added this // yes 
        bool isSettlementExists(const string &settlementName);//yes
        Settlement *getSettlement(const string &settlementName);//yes
        Plan &getPlan(const int planID);//yes
        void step();//yes// need to make sure these three functions 
        void close();//yes
        void open();//yes
        ~Simulation(); //we added distractor//yes
        int getplanCounter() const;//we added this 
        vector<BaseAction*> getActionLog();//we added this 


    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};