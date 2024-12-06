    #pragma once
    #include <string>
    #include <vector>
    #include "Simulation.h"
    #include <iostream>

    
    enum class SettlementType;
    enum class FacilityCategory;

    enum class ActionStatus{
        COMPLETED, ERROR
    };

    class BaseAction{
        public:
            BaseAction();
            ActionStatus getStatus() const;
            virtual void act(Simulation& simulation)=0;//only in the devhired classes 
            virtual const string toString() const=0;
            virtual BaseAction* clone() const = 0;
            virtual ~BaseAction() = default;
            virtual string toPrintLog () const =0; // we added this 
            string StatusToString() const; // we added this 

        protected:
            void complete();
            void error(string errorMsg); 
            const string &getErrorMsg() const; 

        private:
            string errorMsg;
            ActionStatus status;
    };

    class SimulateStep : public BaseAction {

        public:
            SimulateStep(const int numOfSteps);
            void act(Simulation &simulation) override;
            const string toString() const override;
            SimulateStep *clone() const override; 
            string toPrintLog () const override; // we added this 
        private:
            const int numOfSteps;
    };

    class AddPlan : public BaseAction {
        public:
            AddPlan(const string &settlementName, const string &selectionPolicy);
            void act(Simulation &simulation) override;
            const string toString() const override;
            AddPlan *clone() const override;
            string toPrintLog () const override; // we added this 
        private:
            const string settlementName;
            const string selectionPolicy;
    };


    class AddSettlement : public BaseAction {
        public:
            AddSettlement(const string &settlementName,SettlementType settlementType);
            void act(Simulation &simulation) override;
            AddSettlement *clone() const override;
            const string toString() const override;
            string toPrintLog () const override; // we added this 
        private:
            const string settlementName;
            const SettlementType settlementType;
    };



    class AddFacility : public BaseAction {
        public:
            AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore);//yes
            void act(Simulation &simulation) override;
            AddFacility *clone() const override;
            const string toString() const override; 
            string toPrintLog () const override; // we added this 
        private:
            const string facilityName;
            const FacilityCategory facilityCategory;
            const int price;
            const int lifeQualityScore;
            const int economyScore;
            const int environmentScore;

    };

class PrintPlanStatus: public BaseAction {
        public:
            PrintPlanStatus(int planId);
            void act(Simulation &simulation) override;
            PrintPlanStatus *clone() const override;
            const string toString() const override; 
            string toPrintLog () const override; // we added this 
        private:
            const int planId;
    };


    class ChangePlanPolicy : public BaseAction {
        public:
            ChangePlanPolicy(const int planId, const string &newPolicy);
            void act(Simulation &simulation) override;
            ChangePlanPolicy *clone() const override;
            const string toString() const override; 
            string toPrintLog () const override; // we added this 
        private:
            const int planId;
            const string newPolicy;
            string PrevPolicy;//we added this 
    };


    class PrintActionsLog : public BaseAction {
        public:
            PrintActionsLog();//yes 
            void act(Simulation &simulation) override; 
            PrintActionsLog *clone() const override; 
            const string toString() const override; 
            string toPrintLog () const override; // we added this  
        private:
    };

    class Close : public BaseAction {
        public:
            Close(); 
            void act(Simulation &simulation) override;
            Close *clone() const override; 
            const string toString() const override;
            string toPrintLog () const override; // we added this 
        private:
    };

    class BackupSimulation : public BaseAction {
        public:
            BackupSimulation();
            void act(Simulation &simulation) override; 
            BackupSimulation *clone() const override;
            const string toString() const override; 
            string toPrintLog () const override; // we added this 
        private:
    };


    class RestoreSimulation : public BaseAction {
        public:
            RestoreSimulation(); 
            void act(Simulation &simulation) override; 
            RestoreSimulation *clone() const override;
            const string toString() const override; 
            string toPrintLog () const override; // we added this 
        private:
    };