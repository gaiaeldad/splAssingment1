#include "Plan.h"
#include "Facility.h"
#include "SelectionPolicy.h"
#include <vector>
using std::vector;
using std::cout;
using std::endl;



 Plan:: Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
 :plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),status(PlanStatus:: AVALIABLE),facilities(),underConstruction(),facilityOptions(facilityOptions),
 life_quality_score(0), economy_score(0), environment_score(0){}

 const int Plan:: getlifeQualityScore() const{
    return life_quality_score;
 }
const int Plan:: getEconomyScore() const{
    return economy_score;

}
const int  Plan:: getEnvironmentScore() const{
    return environment_score;
}  

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    if (this->selectionPolicy != selectionPolicy) {
        delete this->selectionPolicy;//delete the curr selection policy
        this->selectionPolicy = selectionPolicy;
    }
}
 
void Plan::step(){
    while ( status == PlanStatus::AVALIABLE){
        FacilityType toBuild = selectionPolicy->selectFacility(facilityOptions);
        Facility* buildFac = new Facility(toBuild, settlement.getName());
        addFacility(buildFac);
      
        if (underConstruction.size() == static_cast<size_t>(this->settlement.getLimit())) {
            this->status = PlanStatus::BUSY; 
        }
     }
    // doing a step for all the facilities
    for (size_t i = 0; i < underConstruction.size();){
        Facility* currFac = underConstruction[i];
        FacilityStatus currFacStat = currFac->step();
        if (currFacStat == FacilityStatus::OPERATIONAL){
            addFacility(currFac);
            scoreUpdate(currFac);
            underConstruction.erase(underConstruction.begin() + i);
        }
        else{
            i++;
        }
    }
    //update plan if is AVALIABLE 
    if (underConstruction.size() < static_cast<size_t>(this->settlement.getLimit())){
        this->status = PlanStatus::AVALIABLE;
    }
}

void Plan::printStatus(){
  if (status == PlanStatus::AVALIABLE) {
    cout << "plan id: " + std::to_string(plan_id) + "settlement: " + settlement.getName() +  "Status: Available" << endl;
} else {
    cout << "Status: Busy" << endl;
}
}

const vector<Facility*>& Plan::getFacilities() const{
       return facilities;
}

void  Plan::addFacility(Facility* facility){ 
    if(facility->getStatus() == FacilityStatus::OPERATIONAL){
            facilities.push_back(facility);
    }
    else{
        underConstruction.push_back(facility);
    }  
}

    const string Plan::toString() const {
    string ans = "PlanID: " + std::to_string(plan_id) + "\n";
    ans += "SettlementName: " + settlement.getName() + "\n"; 
    ans += "PlanStatus: ";
    if (status == PlanStatus::AVALIABLE) {
         ans += "AVAILABLE";
    } else if (status == PlanStatus::BUSY) {
         ans += "BUSY";
    }
    ans += "\n";
    ans += "SelectionPolicy: " + selectionPolicy->toString() + "\n"; 
    ans += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    ans += "EconomyScore: " + std::to_string(economy_score) + "\n"; 
    ans += "EnvironmentScore: " + std::to_string(environment_score) + "\n";
    
    for (Facility* facility : underConstruction) {
        ans += "Facility Name: " + facility->getName() + "\n";
        ans += "Facility Status: " + facility->statusToString() + "\n";
    }
    for (Facility* facility : facilities) {
        ans += "Facility Name: " + facility->getName() + "\n";
        ans += "Facility Status: " + facility->statusToString() + "\n";
    }
    return ans;
 }       

 const string Plan::closeToString() const {
    string ans = "PlanID: " + std::to_string(plan_id) + "\n";
    ans += "SettlementName: " + settlement.getName() + "\n"; 
    ans += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    ans += "EconomyScore: " + std::to_string(economy_score) + "\n"; 
    ans += "EnvironmentScore: " + std::to_string(environment_score) + "\n";
    return ans;

 }
 void Plan::scoreUpdate(Facility* facility){
    this->life_quality_score += facility->getLifeQualityScore();
    this->economy_score += facility->getEconomyScore();
    this->environment_score += facility->getEnvironmentScore();
}

const int Plan::getPlanID() const{
    return plan_id;
 }
 Settlement Plan:: getSettlement(){//maybe delet 
    return settlement;
}
 PlanStatus Plan:: getStatus(){//maybe delete 
    return status;
 }
string Plan::getSelectionPolicy() const {
    if (dynamic_cast<NaiveSelection*>(selectionPolicy)) {
        return "nve";
    } else if (dynamic_cast<BalancedSelection*>(selectionPolicy)) {
        return "bal";
    } else if (dynamic_cast<EconomySelection*>(selectionPolicy)) {
        return "eco";
    } else if (dynamic_cast<SustainabilitySelection*>(selectionPolicy)) {
        return "env";
    }
    return "unknown selection Policy";
}
bool Plan::ChangeSelectionPolicy(const string newPolicy){//we added this method 
    if (newPolicy == "eco") {
        setSelectionPolicy(new EconomySelection());
        return true; 
        } else if (newPolicy == "nve") {
        setSelectionPolicy(new NaiveSelection());
        return true;
    } else if (newPolicy == "env") {
        setSelectionPolicy(new SustainabilitySelection());
        return true;
    } else if (newPolicy == "bal") {
        int newEcoScore = getEconomyScore();
        int newLifeScore = getlifeQualityScore();
        int newEnvDcore = getEnvironmentScore();
        for (Facility* FcilityUnderConstruction : underConstruction){
            newLifeScore += FcilityUnderConstruction->getLifeQualityScore();
            newEcoScore += FcilityUnderConstruction->getEconomyScore();
            newEnvDcore += FcilityUnderConstruction->getEnvironmentScore();
        }
        setSelectionPolicy (new BalancedSelection (newLifeScore, newEcoScore, newEnvDcore));
        return true;    
    }else {
        return false;
    }
}

const vector<FacilityType>& Plan::getFacilityOptions() const {//maybe delete 
    return facilityOptions;
}

// Rule of 5:
//copy Constructor
Plan::Plan(const Plan &other):
    plan_id(other.plan_id),
    settlement(other.settlement),
    selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),// Deep copy of selectionPolicy
    status(other.status),
    facilities(),
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.environment_score){ 
    // Deep copy of facilities and underConstruction
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
}


//Copy Assingment Operator
Plan& Plan::operator=(const Plan &other) { 
    if (this != &other) {
        // Allocate new resources for  selectionPolicy
        SelectionPolicy* tempPolicy = other.selectionPolicy->clone();
        delete selectionPolicy;// Delete old resources
        selectionPolicy = tempPolicy;// Assign new resources
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        // Clear and deep copy underConstruction facilities
        for (Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();
        for (Facility* facility : other.underConstruction) {
            underConstruction.push_back(new Facility(*facility));
        } 
        // Clear and deep copy operational facilities
        for (Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();
        for (Facility* facility : other.facilities) {
            facilities.push_back(new Facility(*facility));
        }
    }
    return *this;
}

// move Constructor//i changed this 
Plan::Plan(Plan &&other)
    : plan_id(other.plan_id),
      settlement(std::move(other.settlement)),
      selectionPolicy(other.selectionPolicy),
      status(other.status),
      facilities(std::move(other.facilities)), 
      underConstruction(std::move(other.underConstruction)),
      facilityOptions(std::move(other.facilityOptions)),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score){
    other.selectionPolicy = nullptr; // Nullify to prevent double deletion
}


// Move Assignment Operator
Plan& Plan::operator=(Plan &&other) {
    if (this != &other) {
        plan_id = other.plan_id;
        status = other.status;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        
        if (selectionPolicy) {
            delete selectionPolicy;
        }
        selectionPolicy = other.selectionPolicy;
        other.selectionPolicy = nullptr;

        facilities.clear();
        for (Facility* facility : other.facilities) {
            addFacility(facility);
        }
        other.facilities.clear();
        underConstruction.clear();
        for (Facility* facility : other.underConstruction) {
            addFacility(facility);
        }
        other.underConstruction.clear();
    }
    return *this;
}


//Destractor
Plan::~Plan() {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy;
    }
    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear(); 
}


//copy constructor helper from copy constructor of simulation 
Plan::Plan(const Settlement &settlement, const Plan &other):Plan(other.plan_id, settlement, other.selectionPolicy->clone(), other.facilityOptions){
    status = other.status;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();
     // Deep copy of facilities
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility)); // Copy each Facility
    }

    // Deep copy of underConstruction
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility)); // Copy each Facility
    }

}
