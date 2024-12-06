#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
# include <iostream>
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const string closeToString() const ; // we added
        void scoreUpdate(Facility* facility);// we added
        const int getPlanID() const; //we added
        Settlement getSettlement(); //we added
        PlanStatus getStatus(); // we added 
        string getSelectionPolicy() const;//we added this 
        bool ChangeSelectionPolicy(const string newPolicy);//we added this 
        const vector<FacilityType> &getFacilityOptions() const; //we added 
        //rule of 5 
        Plan(const Plan &other); //we added copy Constructor
        Plan& operator=(const Plan &other);// we added copy assingment operator
        Plan(Plan &&other);// we added Move Constructor
        Plan& operator=(Plan &&other);// we added Move Assignment Operator
        ~Plan(); //we added destractor
         Plan(const Settlement &settlement, const Plan &other);// we added this Copy Constructor for copy simulation


    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};