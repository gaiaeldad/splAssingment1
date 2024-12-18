#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;
        const int getLimit() const; //we added this method
    

        private:
            const string name;
            SettlementType type;
            int limit; //we added this 
};
 string SettlementTypeToString(SettlementType type);