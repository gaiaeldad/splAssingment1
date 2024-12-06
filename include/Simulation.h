#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Auxiliary.h"
#include <fstream>
using std::string;
using std::vector;
using std::cout;
using std::endl;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        SettlementType intToSettlementType(int type); //we added
        FacilityCategory intToFacilityCategory(int category); //we added
        void start();
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool isSettlementExists(const string &settlementName);// we added this method 
        bool addFacility(FacilityType facility);
        bool doesFacilityExists(const string &facilityName);//we added this method 
        Settlement *getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        int getplanCounter() const;//we added this 
        vector<BaseAction*> getActionLog();//we added this 
        //Rule of 5:
        Simulation(const Simulation &other); //we added copy constructor
        Simulation& operator=(const Simulation &other);// we added copy assingment operator
        Simulation(Simulation &&other); //  we added move constructor
        Simulation& operator=(Simulation &&other); // we added move assignment operator
        ~Simulation(); //we added destructor


    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};