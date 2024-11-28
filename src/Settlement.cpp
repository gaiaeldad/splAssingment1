#include "Settlement.h"


Settlement::Settlement(const string &name, SettlementType type)
:name(name), type(type){
   switch (type){
        case SettlementType::VILLAGE: limit = 1; break;
        case SettlementType::CITY: limit = 2; break;
        case SettlementType::METROPOLIS: limit = 3;
    }
} //בנאי ריק דיפולטיבי סימןשאלה

 const string &Settlement::getName() const{
    return name;
 }

 SettlementType Settlement::getType() const{
    return type;
 }

string SettlementTypeToString(SettlementType type){
    switch (type){
        case SettlementType::VILLAGE: return "Village";
        case SettlementType::CITY: return "City";
        case SettlementType::METROPOLIS: return "Metropolis";
    }
    return "Unknown";
 }

 const string Settlement::toString() const{
    return "Settlement:" + name + ", Type: " + SettlementTypeToString(type);
 }

  const int Settlement::getLimit(){//we added
   return limit;
 }