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
            BaseAction();// need to implement and need to see how 
            ActionStatus getStatus() const;//yes 
            virtual void act(Simulation& simulation)=0;//only in the devhired classes 
            virtual const string toString() const=0;
            virtual BaseAction* clone() const = 0;
            virtual ~BaseAction() = default;
            virtual string toPrintLog () const =0; // we added
            string StatusToString() const; // we added

        protected:
            void complete();//yes 
            void error(string errorMsg);//yes 
            const string &getErrorMsg() const;//yes 

        private:
            string errorMsg;
            ActionStatus status;
    };

    class SimulateStep : public BaseAction {

        public:
            SimulateStep(const int numOfSteps);//yes 
            void act(Simulation &simulation) override;//yes 
            const string toString() const override;//yes 
            SimulateStep *clone() const override;//yes 
            string toPrintLog () const override; // we added
        private:
            const int numOfSteps;
    };

    class AddPlan : public BaseAction {
        public:
            AddPlan(const string &settlementName, const string &selectionPolicy);//yes
            void act(Simulation &simulation) override;//dont know how 
            const string toString() const override;//yes
            AddPlan *clone() const override;//yes 
            string toPrintLog () const override; // we added
        private:
            const string settlementName;
            const string selectionPolicy;
    };


    class AddSettlement : public BaseAction {
        public:
            AddSettlement(const string &settlementName,SettlementType settlementType);//yes
            void act(Simulation &simulation) override;//yes
            AddSettlement *clone() const override;//yes
            const string toString() const override;//yes
            string toPrintLog () const override; // we added
        private:
            const string settlementName;
            const SettlementType settlementType;
    };



    class AddFacility : public BaseAction {
        public:
            AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore);//yes
            void act(Simulation &simulation) override;//we have a problem with the constructor because dont have settelement 
            AddFacility *clone() const override;//yes 
            const string toString() const override;//yes 
            string toPrintLog () const override; // we added
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
            PrintPlanStatus(int planId);// yes
            void act(Simulation &simulation) override; // yes
            PrintPlanStatus *clone() const override; // yes
            const string toString() const override; // yes
            string toPrintLog () const override; // we added
        private:
            const int planId;
    };


    class ChangePlanPolicy : public BaseAction {
        public:
            ChangePlanPolicy(const int planId, const string &newPolicy);//yes
            void act(Simulation &simulation) override;//yes 
            ChangePlanPolicy *clone() const override;//yes
            const string toString() const override;//yes 
            string toPrintLog () const override; // we added
        private:
            const int planId;
            const string newPolicy;
            string PrevPolicy;//we added this 
    };


    class PrintActionsLog : public BaseAction {
        public:
            PrintActionsLog();//yes 
            void act(Simulation &simulation) override; //. yes
            PrintActionsLog *clone() const override; // yes
            const string toString() const override; //yes
            string toPrintLog () const override; // we added 
        private:
    };

    class Close : public BaseAction {
        public:
            Close(); // yes
            void act(Simulation &simulation) override; // yes
            Close *clone() const override; // yes
            const string toString() const override; // yes
            string toPrintLog () const override; // we added
        private:
    };

    class BackupSimulation : public BaseAction {
        public:
            BackupSimulation();// yes
            void act(Simulation &simulation) override; //yes
            BackupSimulation *clone() const override; //yes
            const string toString() const override; // yes
            string toPrintLog () const override; // we added
        private:
    };


    class RestoreSimulation : public BaseAction {
        public:
            RestoreSimulation(); //yes
            void act(Simulation &simulation) override; //yes
            RestoreSimulation *clone() const override;//yes
            const string toString() const override; //yes
            string toPrintLog () const override; // we added
        private:
    };