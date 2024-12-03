#pragma once
#include <string>
#include <vector>
#include "Simulation.h"
#include "Action.h"
using std::cout;
using std::endl;

extern Simulation* backup;

BaseAction::BaseAction():errorMsg("No Error"), status("what should we put here?"){}//need to come back and make sure the constructor is correct 

ActionStatus BaseAction::getStatus() const {
    return status;
}
 void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
 }

 void BaseAction::error(string errorMsg) {
    this->errorMsg = errorMsg;  
    status = ActionStatus::ERROR; 
}
const string& BaseAction::getErrorMsg() const {
    return errorMsg;  
}

string BaseAction::StatusToString () const {//our method
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
    }
}


//Simulation Step
SimulateStep::SimulateStep(const int numOfSteps):
    BaseAction(),numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation &simulation){
    if (numOfSteps >0){
        for (int i =0 ;i<numOfSteps; i=i+1)
        simulation.step();
        }
        complete();
    }
const string SimulateStep::toString() const{
    return "Number of Steps down is " + std::to_string(numOfSteps);
}

SimulateStep* SimulateStep :: clone() const{
       return new SimulateStep(*this);
}
 string SimulateStep:: toPrintLog () const {
     return "simulateStep " + std::to_string(numOfSteps) + " " + StatusToString();
 }

// add plan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):BaseAction(), settlementName(settlementName) ,selectionPolicy(selectionPolicy){
}

void AddPlan::act(Simulation &simulation){
    
    Settlement* Set = simulation.getSettlement(settlementName);
    if (Set == nullptr){
        error("Cannot create this plan- settlment not found");
        return;
    }
    SelectionPolicy* Selectp = SelectionPolicy::createNewSP(SelectionPolicyType);// we dont know how to fix this 
    if (Selectp ==nullptr){
         error("Cannot create this plan- selection policy is invalid");
        return;
    }
    simulation.addPlan(Set,Selectp);

    complete();

}

const string AddPlan::toString() const {
    return "Plan: Settlement Name = " + settlementName + ", Selection Policy = " + selectionPolicy;
}

AddPlan* AddPlan::clone() const{
return new AddPlan(*this);
}

string AddPlan:: toPrintLog () const {
     return "AddPlan " + settlementName + " " + selectionPolicy + " " + StatusToString();
 }

//Add Settelment 


AddSettlement:: AddSettlement(const string &settlementName,SettlementType settlementType):
BaseAction(), settlementName(settlementName),settlementType(settlementType) {}
            
void AddSettlement::act(Simulation &simulation){
Settlement* newSet = new Settlement(settlementName, settlementType);
    if (!simulation.addSettlement(newSet)){
        delete newSet;
        error("Settelment allready exsits");
        return;
    }
        complete();
}

AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const{
 return "Settelment" + settlementName + SettlementTypeToString(settlementType);
}

string AddSettlement:: toPrintLog () const{
     return "AddSettelment" + settlementName + SettlementTypeToString(settlementType) + " " + StatusToString();
 }

//AddFacility 

   
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
BaseAction(), facilityName(facilityName) ,facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore),environmentScore(environmentScore){}



void AddFacility::act(Simulation &simulation){///guy will send this to us need to fix everything 
    const string &SettlementName = simulation.
FacilityType newFac = Facility(facilityName, Settlementname-"what to put here?" , facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
}


AddFacility* AddFacility::clone() const {
    return new AddFacility(*this);
}


const string AddFacility::toString() const {
    return "Facility: " + facilityName +
           ", Category: " + std::to_string(static_cast<int>(facilityCategory)) +
           ", Price: " + std::to_string(price) +
           ", Life Quality Impact: " + std::to_string(lifeQualityScore) +
           ", Economic Impact: " + std::to_string(economyScore) +
           ", Environmental Impact: " + std::to_string(environmentScore);
}

string AddFacility:: toPrintLog () const {
     return "Addfacility " + facilityName + " " +std::to_string(static_cast<int>(facilityCategory)) + " " + std::to_string(price) +" " + std::to_string(lifeQualityScore) +  " " +  std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + StatusToString();
 }


    //PrintPlanStatus
    
PrintPlanStatus::PrintPlanStatus(int planId): BaseAction(), planId(planId){}

void PrintPlanStatus::act(Simulation &simulation){
    int size = simulation.getplanCounter();
    if (planId <0 || planId >= size){
    error("Plan doesn't exsits");
    return;
    }
    Plan& CurrPlan = simulation.getPlan(planId);
     
    cout <<CurrPlan.toString()<< endl;
    complete();
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const{
   return "Plan Id" + planId;

}

string PrintPlanStatus:: toPrintLog ()const {
     return "PrintPlanStatus Plan Id " + planId; " " + StatusToString();
 }

//Change Plan Policy 

ChangePlanPolicy:: ChangePlanPolicy(const int planId, const string &newPolicy):BaseAction(), planId(planId), newPolicy(newPolicy), PrevPolicy(""){}

void ChangePlanPolicy::act(Simulation &simulation){
    int size = simulation.getplanCounter();
    if (planId <0 || planId >= size){
         error("Plan ID invalid");
         return;
    }
     Plan& currplan = simulation.getPlan(planId);
     string CurrPoli = currplan.getSelectionPolicy();

    if (CurrPoli == newPolicy ){ 
        error("Cannot change selection Policy ");
        return;
    }
    if (!currplan.ChangeSelectionPolicy(newPolicy)){
        error("Selection Policy invalid");
    }
    currplan.ChangeSelectionPolicy(newPolicy);//might need to update last selected index 
    PrevPolicy = CurrPoli;
    complete();
}
ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy:: toString() const{
    return "ChangePlanPolicy: PlanID = " + std::to_string(planId) + 
           ", Previous Policy = " + PrevPolicy + 
           ", New Policy = " + newPolicy;
}
string ChangePlanPolicy:: toPrintLog () const {
     return "change policy " + std::to_string(planId) + " "  + newPolicy + StatusToString();
 }

//Print Action Log 

PrintActionsLog::PrintActionsLog():BaseAction(){}

void PrintActionsLog:: act(Simulation &simulation){
    for (BaseAction* currAction : simulation.getActionLog()){
        cout << currAction->toPrintLog() << endl;
    }
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
 
 const string PrintActionsLog:: toString() const {
    return  toPrintLog();
 }

 string PrintActionsLog:: toPrintLog()const {
    return "PrintActionsLog" + StatusToString();
 }

 //close
 Close::Close():BaseAction(){}

 void Close:: act(Simulation &simulation){
    simulation.close(); // make shure memory is free
 }

 Close* Close::clone() const {
    return new Close(*this);
}
const string Close:: toString() const {
 return toPrintLog();
}

string  Close:: toPrintLog () const{
      return "close " + StatusToString();
}

 // BackupSimulation
BackupSimulation::BackupSimulation():BaseAction() {}

void BackupSimulation::act(Simulation &simulation) {
    if (backup != nullptr) {
        delete backup;
    }
    backup = new Simulation(simulation);
    complete();
}

 BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

 const string BackupSimulation::toString() const{
  return BackupSimulation:: toPrintLog ();
 }

string BackupSimulation:: toPrintLog () const {
     return "backup " + StatusToString();
 }
    
// RestoreSimulation
RestoreSimulation:: RestoreSimulation(): BaseAction(){}

void RestoreSimulation:: act(Simulation &simulation){
       if (backup == nullptr) {
        error("No backup available");
        return;
    }
    simulation = *backup; 
    complete();
}

 RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation:: toString() const {
    return toPrintLog();
}

string RestoreSimulation:: toPrintLog () const {
     return "backup " + StatusToString();
}
    



    

    
        