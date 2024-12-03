#include "SelectionPolicy.h"
#include <vector>
#include <stdexcept>
using std::vector;
#include <climits>


//Naive Selection 
NaiveSelection:: NaiveSelection(): lastSelectedIndex(-1){

}
const FacilityType& NaiveSelection::selectFacility(const vector <FacilityType>& facilitiesOptions){
    if (facilitiesOptions.empty()) {
        throw std::invalid_argument("facilitiesOptions is empty");
    }
    
lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
return facilitiesOptions[lastSelectedIndex];

}

const std::string NaiveSelection::toString() const {
    return "Last selected index is:" + std::to_string(lastSelectedIndex);
}


NaiveSelection* NaiveSelection:: clone() const { ///do need to see how we delet this in all the clones in this class 
    return new NaiveSelection(*this);
}

//NaiveSelection::~NaiveSelection() = default; 


//need to wrtie the balnced 
 BalancedSelection:: BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
 LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){
 }

const FacilityType& BalancedSelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::invalid_argument("facilitiesOptions is empty");}
    int min = INT_MAX;
    const FacilityType* selectedFacility = nullptr;
    for ( const FacilityType &cand : facilitiesOptions){
         int temp = findDiff(cand);
        if( temp < min){
            min = temp;
            selectedFacility = &cand;
        }
    }
    LifeQualityScore += selectedFacility->getLifeQualityScore();
    EconomyScore += selectedFacility->getEconomyScore();
    EnvironmentScore += selectedFacility->getEnvironmentScore();
    return *selectedFacility; //return * or &
}

int BalancedSelection ::findDiff (const FacilityType& FacilityCand){
    int SumLifeQualityScore = this->LifeQualityScore + FacilityCand.getLifeQualityScore();
    int SumEconomyScore = this->EconomyScore + FacilityCand.getEconomyScore();
    int SumEnvironmentScore = this->EnvironmentScore + FacilityCand.getEnvironmentScore();
    int maxScore = std::max(SumLifeQualityScore, std::max(SumEconomyScore, SumEnvironmentScore));
    int minScore = std::min(SumLifeQualityScore, std::min(SumEconomyScore, SumEnvironmentScore));
    return  maxScore - minScore;
}

const std::string BalancedSelection::toString() const {
    return "The LifeQuality score is " + std::to_string(LifeQualityScore) +
     ", The Economy score is " + std::to_string(EconomyScore) +
      ", The Environment score is " + std::to_string(EnvironmentScore);
}

BalancedSelection* BalancedSelection:: clone() const { 
    return new BalancedSelection(*this);
}

//BalancedSelection::~BalancedSelection() = default; //told us we dont need this line a all need to check with people 

void BalancedSelection ::setScores(int Curr_Life, int Curr_Econmy, int Curr_Env){/// צריכה קבל גם מה שבבניה
    LifeQualityScore = Curr_Life;
    EconomyScore =  Curr_Econmy;
    EnvironmentScore = Curr_Env;
}


// EconomySelection 
EconomySelection:: EconomySelection(): lastSelectedIndex(-1){}

const FacilityType& EconomySelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::invalid_argument("facilitiesOptions is empty");
    }
    
    int size = facilitiesOptions.size();
    int currentIndex = (lastSelectedIndex + 1) % size; 

    for (int i = 0; i < size; ++i) {
        if (facilitiesOptions[currentIndex].getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = currentIndex; 
            return facilitiesOptions[currentIndex];
        }
        currentIndex = (currentIndex + 1) % size; 
    }

    // If no ECONOMY facility is found, throw an exception
    throw std::runtime_error("No facility with ECONOMY category found");
}

const std::string EconomySelection::toString() const{
    return "Last selected index is:" + std::to_string(lastSelectedIndex);
}

EconomySelection* EconomySelection:: clone() const { 
    return new EconomySelection(*this);
}

//EconomySelection::~EconomySelection() = default; 
    

// SustainabilitySelection 
SustainabilitySelection:: SustainabilitySelection(): lastSelectedIndex(-1){}

const FacilityType& SustainabilitySelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::invalid_argument("facilitiesOptions is empty");
    }
    
    int size = facilitiesOptions.size();
    int currentIndex = (lastSelectedIndex + 1) % size; 

    for (int i = 0; i < size; ++i) {
        if (facilitiesOptions[currentIndex].getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = currentIndex; 
            return facilitiesOptions[currentIndex];
        }
        currentIndex = (currentIndex + 1) % size; 
    }

    // If no ENVIRONMENT facility is found, throw an exception
    throw std::runtime_error("No facility with ENVIRONMENT category found");
}

const std::string SustainabilitySelection::toString() const{
    return "Last selected index is:" + std::to_string(lastSelectedIndex);
}

SustainabilitySelection* SustainabilitySelection:: clone() const { 
    return new SustainabilitySelection(*this);
}


//SustainabilitySelection::~SustainabilitySelection() = default; 
// helper metodes we added
SelectionPolicy* SelectionPolicy::createNewSP(const string& selectionPolicyType){
    SelectionPolicy* policy = nullptr;
    if (selectionPolicyType == "eco") {
        policy = new EconomySelection();
    } else if (selectionPolicyType == "bal") {
        policy = new BalancedSelection(0, 0, 0);
    } else if (selectionPolicyType == "sus") {
        policy = new SustainabilitySelection();
    } else {

        return nullptr;
    }
    return policy;
}    

