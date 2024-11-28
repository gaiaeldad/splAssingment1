#include "Facility.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
#include <iostream>
#include <cassert>

// יצירת MockFacilityType לצורך הבדיקות (מימוש למחלקה FacilityType)
class MockFacilityType : public FacilityType {
public:
    MockFacilityType(const std::string& name, FacilityCategory category, int lifeQuality, int economy, int environment)
        : FacilityType(name, category, 100, lifeQuality, economy, environment) {}

    // אפשר להוסיף פונקציות נוספות אם צריך
};

int main() {
    // יצירת מתקנים לדוגמה
    std::vector<FacilityType> facilities;
    facilities.push_back(MockFacilityType("Factory", FacilityCategory::ECONOMY, 60, 80, 70));
    facilities.push_back(MockFacilityType("Park", FacilityCategory::LIFE_QUALITY, 80, 60, 50));
    facilities.push_back(MockFacilityType("Solar Plant", FacilityCategory::ENVIRONMENT, 70, 90, 95));
    
    // Testing NaiveSelection
    NaiveSelection naiveSelection;
    const FacilityType& selectedNaive = naiveSelection.selectFacility(facilities);
    std::cout << "NaiveSelection toString: " << naiveSelection.toString() << std::endl;  // רק מדפיסים את התוצאה של toString
    
    // Testing EconomySelection
    EconomySelection economySelection;
    const FacilityType& selectedEconomy = economySelection.selectFacility(facilities);
    std::cout << "EconomySelection toString: " << economySelection.toString() << std::endl;

    // Testing SustainabilitySelection (בוחן על פי קטגוריית ENVIRONMENT)
    SustainabilitySelection sustainabilitySelection;
    const FacilityType& selectedSustainability = sustainabilitySelection.selectFacility(facilities);
    std::cout << "SustainabilitySelection toString: " << sustainabilitySelection.toString() << std::endl;

    // Testing BalancedSelection עם ציון מותאם אישית
    BalancedSelection balancedSelection(70, 60, 50);
    const FacilityType& selectedBalanced = balancedSelection.selectFacility(facilities);
    std::cout << "BalancedSelection toString: " << balancedSelection.toString() << std::endl;
    
    // Testing clone() method
    NaiveSelection* clonedNaiveSelection = naiveSelection.clone();
    std::cout << "Cloned NaiveSelection toString: " << clonedNaiveSelection->toString() << std::endl;
    delete clonedNaiveSelection; // ניקוי המופע המועתק

    // Testing exception for empty facility list
    try {
        std::vector<FacilityType> emptyFacilities;
        naiveSelection.selectFacility(emptyFacilities);
        assert(false);  // לא אמור להגיע לפה, צריך לזרוק חריגה
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for empty facility list: " << e.what() << std::endl;
    }

    // Testing exception for EconomySelection with no matching facility
    try {
        std::vector<FacilityType> noEconomyFacilities = {MockFacilityType("Park", FacilityCategory::LIFE_QUALITY, 80, 60, 50)};
        economySelection.selectFacility(noEconomyFacilities);
        assert(false);  // לא אמור להגיע לפה, צריך לזרוק חריגה
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }

    // אם כל הבדיקות עברו:
    std::cout << "All tests passed!" << std::endl;

    return 0;
}
