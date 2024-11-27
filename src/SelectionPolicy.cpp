#include "SelectionPolicy.h"
#include <vector>
#include <stdexcept>
using std::vector;


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
const string NaiveSelection::toString(){
    return "Last selected index is:" + sdt::to_string(lastSelectedIndex);

NaiveSelection* NaiveSelection:: clone() const{ ///overide?
    return new NaiveSelection(*this);
}

NaiveSelection::~NaiveSelection() = default; //overaid?


//need to wrtie the balnced 



// EconomySelection 
EconomySelection:: EconomySelection(): lastSelectedIndex(-1){}

const FacilityType& EconomySelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) override{
    if (facilitiesOptions.empty()) {
        throw std::invalid_argument("facilitiesOptions is empty");
    }
    
    int size = facilitiesOptions.size();
    int currentIndex = (lastSelectedIndex + 1) % size; 

    for (int i = 0; i < size; ++i) {
        if (facilitiesOptions[currentIndex].getCategory() == ECONOMY) {
            lastSelectedIndex = currentIndex; 
            return facilitiesOptions[currentIndex];
        }
        currentIndex = (currentIndex + 1) % size; 
    }

    // If no ECONOMY facility is found, throw an exception
    throw std::runtime_error("No facility with ECONOMY category found");
}

    

}


