#include "Settlement.h"


Settlement::Settlement(const string &name, SettlementType type)
:name(name), type(type),limit(){
   switch (type){
        case SettlementType::VILLAGE: limit = 1; break;
        case SettlementType::CITY: limit = 2; break;
        case SettlementType::METROPOLIS: limit = 3;
    }
} 

 const string &Settlement::getName() const{
    return name;
 }

 SettlementType Settlement::getType() const{
    return type;
 }

string SettlementTypeToString(SettlementType type){
    switch (type){
        case SettlementType::VILLAGE: return "0";
        case SettlementType::CITY: return "1";
        case SettlementType::METROPOLIS: return "2";
    }
    return "Unknown";
 }

 const string Settlement::toString() const{
    return "Settlement:" + name + ", Type: " + SettlementTypeToString(type);
 }

  const int Settlement::getLimit() const {//we added
   return limit;
 }