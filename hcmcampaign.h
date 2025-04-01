/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};
enum UnitClassify
{
    VEHICLE,
    INFANTRY
};

/* Army group*/
class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    virtual void fightIfDefense(Army *enemy) = 0; // có thể thay bằng interface và friend class
    virtual void fightIfAttack(Army *enemy) = 0;
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false);
    virtual string str() const = 0;

    // getter, setter
    int getLF() const { return LF; }
    int getEXP() const { return EXP; }
    UnitList *getUnitList() const { return unitList; }
    BattleField *getBattleField() const { return battleField; }
    void setLF(double LF)
    {
        int temp = ceil(LF);
        this->LF = max(0, min(1000, temp));
    }
    void setEXP(double EXP)
    {
        int temp = ceil(EXP);
        this->EXP = max(0, min(500, temp));
    }
    void resetLF_EXP()
    {
        // Calculate LF and EXP
        LF = 0;
        EXP = 0;
        vector<Unit *> units = unitList->getUnits();
        for (Unit *unit : units)
        {
            if (unit->getUnitClassify() == VEHICLE)
            {
                Vehicle *vehicle = static_cast<Vehicle *>(unit);
                LF += vehicle->getAttackScore();
            }
            else
            {
                Infantry *infantry = static_cast<Infantry *>(unit);
                EXP += infantry->getAttackScore();
            }
        }
        LF = max(0, min(1000, LF));
        EXP = max(0, min(500, EXP));
    }
    string getName() const { return name; }
};

class LiberationArmy : public Army
{
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
        : Army(unitArray, size, name, battleField) {}

    void fightIfAttack(Army *enemy);
    void fightIfDefense(Army *enemy);

    string str() const override;
};
class ARVN : public Army
{
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
        : Army(unitArray, size, name, battleField) {}

    void fightIfAttack(Army *enemy);
    void fightIfDefense(Army *enemy);

    string str() const override;
};
/* Army group */

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0) : r(r), c(c) {}
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const { return r; }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }
    string str() const; // Example: returns "(1,15)"

    // operator
    bool operator==(const Position &pos) const { return r == pos.r && c == pos.c; }
    bool operator!=(const Position &pos) const { return r != pos.r || c != pos.c; }
    Position operator+(const Position &pos) const { return Position(r + pos.r, c + pos.c); }
    Position operator-(const Position &pos) const { return Position(r - pos.r, c - pos.c); }
    Position &operator=(const Position &pos)
    {
        r = pos.r;
        c = pos.c;
        return *this;
    }
};

/* Unit group */

class Unit
{
protected:
    int quantity, weight;
    Position pos;
    UnitClassify unitClassify;

public:
    Unit(int quantity, int weight, Position pos) : quantity(quantity), weight(weight), pos(pos) {}
    virtual ~Unit() {}
    virtual int getAttackScore() = 0;
    virtual string str() const = 0;

    // getter, setter: i miss py :(((
    Position getPos() const { return pos; }
    void setPos(Position pos) { this->pos = pos; }
    Position getCurrentPosition() const { return pos; }
    UnitClassify getUnitClassify() const { return unitClassify; }
    int getQuantity() const { return quantity; }
    int getWeight() const { return weight; }
    void setQuantity(double quantity) { this->quantity = ceil(quantity); }
    void setWeight(double weight) { this->weight = ceil(weight); }
};

class Vehicle : public Unit
{
private:
    VehicleType vehicleType;

public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
        : Unit(quantity, weight, pos), vehicleType(vehicleType) { unitClassify = VEHICLE; }
    int getAttackScore() override;
    string str() const override;
    // getter, setter
    VehicleType getVehicleType() const { return vehicleType; }
};

class Infantry : public Unit
{
private:
    InfantryType infantryType;

public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
        : Unit(quantity, weight, pos), infantryType(infantryType) { unitClassify = INFANTRY; }
    int getAttackScore() override;
    string str() const override;
    // getter, setter
    InfantryType getInfantryType() const { return infantryType; }
};

class UnitList
{
private:
    int capacity;
    int count = 0;
    struct UnitNode
    {
        Unit *unit;
        UnitNode *next, *prev;
        UnitNode(Unit *unit, UnitNode *next = nullptr, UnitNode *prev = nullptr)
            : unit(unit), next(next), prev(prev) {}
    } *head, *tail;

public:
    UnitList(Army *army);
    bool insert(Unit *unit);                   // return true if insert successfully, ??? quantity = 0
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    void remove(Unit *unit);
    void remove(vector<Vehicle *> vehicles);
    void remove(vector<Infantry *> infantries);
    void clear();
    // getter, setter
    int getCapacity() const { return capacity; }
    int getCount() const { return count; }
    Vehicle *getVehicleViaType(VehicleType vehicleType);
    Infantry *getInfantryViaType(InfantryType infantryType);
    Unit *getUnit(int index) const
    { // đang fix
        UnitNode *current = head;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current->unit;
    }
    vector<Vehicle *> getVehicles() const;
    vector<Infantry *> getInfantries() const;
    vector<Unit *> getUnits() const;
};

/* Unit group */

/* Terrain group */
class TerrainElement
{
protected:
    virtual void getEffectOnLibArmy(LiberationArmy *liberationArmy) = 0;
    virtual void getEffectOnARVN(ARVN *ARVN) = 0;
    Position pos;

public:
    TerrainElement(Position pos) : pos(pos) {}
    ~TerrainElement() {}
    // virtual void getEffect(Army *army) = 0;
    void getEffect(Army *army)
    {
        if (army->getName() == "LiberationArmy")
        {
            getEffectOnLibArmy(static_cast<LiberationArmy *>(army));
        }
        else
        {
            getEffectOnARVN(static_cast<ARVN *>(army));
        }
    }
};

class Road : public TerrainElement
{
public:
    Road(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override {}
    void getEffectOnARVN(ARVN *ARVN) override {}
};
class Mountain : public TerrainElement
{
public:
    Mountain(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class River : public TerrainElement
{
public:
    River(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class Urban : public TerrainElement
{
public:
    Urban(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class Fortification : public TerrainElement
{
public:
    Fortification(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class SpecialZone : public TerrainElement
{
public:
    SpecialZone(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
/* Terrain group */

class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement ***terrain;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    string str();
    // getter, setter
    TerrainElement ***getTerrain() const { return terrain; }
    TerrainElement *getTerrainElement(int r, int c) const { return terrain[r][c]; }
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *VNCHARMY;

    void afterfightMethod();

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
    ~HCMCampaign();
};

class Configuration
{
private:
    int num_rows, num_cols;
    vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    Unit **liberationUnits, **ARVNUnits; // what the fuck Unit*, thầy ko trỏ vô, fix tự do thôi
    int eventCode;
    int count_liberUnit, count_ARVNUnit;

    // support function
    void Configuration::extractUnits(string input, vector<Unit*>& liberUnits, vector<Unit*>& ARVNUnits);
    void hashUnits(int quantity, int weight, Position pos, int armyBelong, string unitName, vector<Unit*>& liberUnits, vector<Unit*>& ARVNUnits);

public:
    Configuration(const string &config_file_path);
    ~Configuration();
    string str() const;
    // getter, setter
    int getNumRows() const { return num_rows; }
    int getNumCols() const { return num_cols; }

    vector<Position *> getArrayForest() const { return arrayForest; }
    vector<Position *> getArrayRiver() const { return arrayRiver; }
    vector<Position *> getArrayFortification() const { return arrayFortification; }
    vector<Position *> getArrayUrban() const { return arrayUrban; }
    vector<Position *> getArraySpecialZone() const { return arraySpecialZone; }

    Unit **getLiberationUnits() const { return liberationUnits; }
    Unit **getARVNUnits() const { return ARVNUnits; }
    int getEventCode() const { return eventCode; }
    int liberUnitsSize() const { return count_liberUnit; }
    int ARVNUnitsSize() const { return count_ARVNUnit; }
};

#endif