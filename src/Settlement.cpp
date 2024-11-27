#include "Settlement.h"


Settelment::Settlement(const string &name, SettlementType type)
:name(name), type(type){
} //בנאי ריק דיפולטיבי סימןשאלה

 const string &Settlement::getName() const{
    return name;
 }

 SettlementType Settlement::getType() const{
    return type;
 }

 string SettlementTypeToString(SettlementType){
    switch (type){
        case SettlementType::VILLAGE: return "Village";
        case SettlementType::CITY: return "City";
        case SettlementType::METROPOLIS: return "Metropolis";
    }
 }

 const string Settlement::toString() const{
    return "Settlement:" + name + "Type:" + SettlementTypeToString(type);
 }