#include "Plan.h"
#include "Facility.h"
#include "SelectionPolicy.h"


 Plan:: Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
 :plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy),status(PlanStatus:: AVALIABLE),facilityOptions(facilityOptions),
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

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    this->selectionPolicy = selectionPolicy;
}   

void Plan::step(){//need to check that this is correct and that we have all steps 
    while ( status == PlanStatus::AVALIABLE){
        // if (auto balancedSelection = dynamic_cast<BalancedSelection*>(this->selectionPolicy)){
        //     balancedSelection->setScores(life_quality_score, economy_score, environment_score);
        // } לא צריך כי כבר מעדכן את עצמו
      const FacilityType &buildThis = this->selectionPolicy->selectFacility(facilityOptions); //* or &?
     underConstruction.push_back(new Facility( buildThis, this->settlement->getName()));//we need new here so need to delet after 
        //כל עוד טפשר בחרנו פסיליטי והכנסנו לבנייה
        if(underConstruction.size() == this->settlement->getLimit()){
             this->status = PlanStatus::BUSY;
         }
    }
    for (Facility* iter = underConstructionas.begin(); iter != underConstruction.end();) {
        Facility& currFac = *iter; 
        currFac.step(); 
    //maybe the facility has finishes building
        if (currFac.getStatus() == FacilityStatus::OPERATIONAL) {
            facilities.push_back(&currFac);  
            iter = underConstruction.erase(iter); 
            Plan::scoreUpdate(&currFac);
        }else {++iter;}          

    if(underConstructionas.size()< this->settlement->getLimit()){
        this->status = PlanStatus::AVALIABLE;
    }

}
//need to see what we want here 
void Plan::scoreUpdate(Facility* facility){
    this->life_quality_score += facility->getLifeQualityScore();
    this->economy_score += facility->getEconomyScore();
    this->environment_score += facility->getEnvironmentScore();
}

 void Plan::printStatus(){
  if (status == PlanStatus::AVALIABLE) {
    cout << "plan id: " << plan_id << "settlement: " << settlement <<  "Status: Available" << endl;
} else {
    cout << "Status: Busy" << endl;
}
 }
// we need to decide what we want to print out 

const vector<Facility*>& Plan::getFacilities() const{
       return facilities;
}

void  Plan::addFacility(Facility* facility){ //  האם עוד מישהו משתמש בפונקציה ויכול למחוק את הפוינטר?
//לוודא לאיזה רשימה מוסםיפים
    facilities.push_back(facility);
}
          
    string Plan::toString() const {
    string ans = "PlanID: " + std::to_string(plan_id) + "\n";
    ans += "SettlementName: " + settlement->getName() + "\n"; 
    ans += "PlanStatus: " + (status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") + "\n";
    ans += "SelectionPolicy: " + selectionPolicy->toString() + "\n"; 
    ans += "LifeQualityScore: " + std::to_string(life_quality_score) + "\n";
    ans += "EconomyScore: " + std::to_string(economy_score) + "\n";
    ans += "EnvironmentScore: " + std::to_string(environment_score) + "\n";

    // Print  The Facility details
    for (const Facility* facility: facilities) {
        ans += "FacilityName: " + facility->toString() + "\n";  
        ans += "FacilityStatus: " + (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTION ? "UNDER_CONSTRUCTION" : "OPERATIONAL") + "\n"; 
    }

    return ans;
        }       

    int Plan::getPlanID(){
        return plan_id;
    }

// role of 3:
    //copy constractur
Plan::Plan(const Plan &other):
    plan_id(other.plan_id),
      facilityOptions(other.facilityOptions),
      status(other.status),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    settlement = new Settlement(*other.settlement); // Deep copy of settlement
    selectionPolicy = other.selectionPolicy->clone(); // Deep copy of selectionPolicy

    // Deep copy of facilities and underConstruction
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
}
// copy assingment
Plan &Plan::operator=(const Plan &other) {
    if (this != &other) {

    plan_id = other.plan_id;
    status = other.status;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;
    facilityOptions = other.facilityOptions;
    delete this->settelment;
    settlement = new Settlement(*other.settlement);
    delete selectionPolicy;
    selectionPolicy = other.selectionPolicy->clone();
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear(); 
    for (Facility* facility : other.underConstruction) {
         underConstruction.push_back(new Facility(*facility));
    } 
    for (Facility* facility : facilities) {
         delete facility;
    }
    facilities.clear();  // נקה את הוקטור כדי להימנע מהפניות ישנות
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));

    }
//Destractor
Plan:: ~Plan(){
    delete settlement;
    delete selectionPolicy;
    for ( Facility *facility : facilities){
        delete facility;
    }
    for (Facility *facility : underConstruction){
        delete facility;
    }
}  


//make sure the rule of 5 is implemented 