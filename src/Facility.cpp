#include "Facility.h"
#include <string>
#include <vector>
using std::string;
using std::vector; 

//Facility type
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
:name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){
}

const string &FacilityType::getName() const{
    return name;
}
 
int FacilityType::getCost() const{
    return price;
}

int FacilityType::getLifeQualityScore() const{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const{
    return environment_score;
}

int FacilityType::getEconomyScore() const{
    return economy_score;
}

 FacilityCategory FacilityType::getCategory() const{
    return category;
 }

  //Rule Of 3:
// Copy constructor
FacilityType::FacilityType(const FacilityType &other)//shaked changed this 
:name(other.name), category(other.category), price(other.price), 
          lifeQuality_score(other.lifeQuality_score), 
          economy_score(other.economy_score), 
          environment_score(other.environment_score) {}

// Copy assignment operator
FacilityType &FacilityType::operator=(const FacilityType &other){
    return *this;
}
// Destructor: 
FacilityType::~FacilityType(){
}

//faciltiy
 Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
:FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}

Facility::Facility( const FacilityType &type, const string &settlementName):
FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()){
}

const string &Facility:: getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

FacilityStatus Facility:: step(){
    if (timeLeft > 0){
        timeLeft--;
    }
    if (timeLeft==0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

void Facility:: setStatus(FacilityStatus status){
    this->status = status;
}


const FacilityStatus& Facility::getStatus() const {
    return status;
}

const string Facility::toString() const{
    return "Facility Name:" + name; 
 }

const string Facility ::statusToString() const {//we added this method 
    string statusToString;
    switch (this->status) {
         case FacilityStatus::OPERATIONAL:
            statusToString = "OPERATIONAL";
            break;
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            statusToString = "UNDER_CONSTRUCTIONS";
            break;
    }
    return statusToString;
}

//Rule Of 3:
// Copy constructor
Facility::Facility(const Facility &other)
:Facility(other.name, other.settlementName,other.category, other.price, other.lifeQuality_score,other.economy_score,other.environment_score){}

// Copy assignment operator
Facility &Facility::operator=(const Facility &other){
    return *this;
}
