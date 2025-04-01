#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

namespace sp
{
    // common funcs

    bool isSquare(int n)
    {
        if (n < 0)
            return false;
        int root = static_cast<int>(sqrt(n));
        return n == root * root;
    }
    //  shortened, see in test file
    int personalNumber(int n, int Y)
    {
        if (n < 0 || Y < 0) return -1; 
        int sum = n + Y;
        return (sum == 0) ? 0 : (sum - 1) % 9 + 1; 
    }

    double euclidDistance(const Position &pos1, const Position &pos2)
    {
        return sqrt(pow(pos1.getRow() - pos2.getRow(), 2) + pow(pos1.getCol() - pos2.getCol(), 2));
    }

    int closestFibonacci(int n)
    { 
        if (n <= 0) return 0;
        int f1 = 0, f2 = 1;
        while (f2 <= n)
        {
            f2 += f1;
            f1 = f2 - f1; 
        }
        return f2;
    }

    bool isSpecialNumber(int n, int k)
    { // đổi số sang hệ k phân
        if (n <= 0)
            return false; //  n > 0
        if (n == 1)
            return true;

        while (n > 0)
        {
            if (n % k > 1)
                return false; // Nếu chữ số nào lớn hơn 1, không thể phân tích
            n /= k;
        }
        return true;
    }

    int sumPointsofUnitList(vector<Vehicle *> vehicles)
    {
        int sum = 0;
        for (int i = 0; i < vehicles.size(); i++)
        {
            sum += vehicles[i]->getAttackScore();
        }
        return sum;
    }
    int sumPointsofUnitList(vector<Infantry *> infantries)
    {
        int sum = 0;
        for (int i = 0; i < infantries.size(); i++)
        {
            sum += infantries[i]->getAttackScore();
        }
        return sum;
    }

    class fightIfDefense_helper_LiberationArmy 
    {
    public:
        vector<Vehicle *> combinationA();
        vector<Infantry *> combinationB();
    };
}

// Implement the method vehicle
int Vehicle::getAttackScore()
{
    double score = vehicleType * 304 + quantity * weight;
    return score / 30.0;
}
string Vehicle::str() const
{
    // format: Vehicle[attr_name=<attr_value>], vehicleType -> quantity, weight, pos
    string re = "";
    re += "Vehicle[";
    re += "vehicleType=" + to_string(vehicleType) + ",";
    re += "quantity=" + to_string(quantity) + ",";
    re += "weight=" + to_string(weight) + ",";
    re += "pos=" + pos.str() + "]";
    return re;
}
// Implement the method Infantry
int Infantry::getAttackScore()
{
    // fomula: score = typeValue ∗ 56 + quantity ∗ weight
    int score = infantryType * 56 + quantity * weight;
    if (infantryType == InfantryType::SPECIALFORCES && sp::isSquare(weight))
        score += 75;
    int personalNumberin1975 = sp::personalNumber(score, 1975);

    if (personalNumberin1975 > 7)
        quantity = ceil(quantity * 1.2);
    else if (personalNumberin1975 < 3)
        quantity = ceil(quantity * 0.9);
    else
        return score;

    return infantryType * 56 + quantity * weight;
}
string Infantry::str() const
{
    // format: Infantry[attr_name=<attr_value>], infantryType -> quantity, weight, pos
    string re = "";
    re += "Infantry[";
    re += "infantryType=" + to_string(infantryType) + ",";
    re += "quantity=" + to_string(quantity) + ",";
    re += "weight=" + to_string(weight) + ",";
    re += "pos=" + pos.str() + "]";
    return re;
}

// Implement the method Army
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
{
    this->name = name;
    this->battleField = battleField;
    // From the unitArray, create a UnitList
    unitList = new UnitList(this);
    for (int i = 0; i < size; i++)
    {
        unitList->insert(unitArray[i]);
    }
    resetLF_EXP();
}
void Army::fight(Army *enemy, bool defense)
{
    defense ? fightIfDefense(enemy) : fightIfAttack(enemy);
}
// Implement the method LiberationArmy
void LiberationArmy::fightIfAttack(Army *enemy)
{
    this->setEXP(this->getEXP() * 1.5);
    this->setLF(this->getLF() * 1.5);
    double enemyLF = enemy->getLF(), enemyEXP = enemy->getEXP();

    bool victory = false;
    bool battle = true;
    sp::fightIfDefense_helper_LiberationArmy helper;
    vector<Vehicle *> vehicles = helper.combinationA();
    vector<Infantry *> infantries = helper.combinationB();

    if (vehicles.size() == 0 && infantries.size() == 0)
        battle = false;
    else if (vehicles.size() != 0 && infantries.size() != 0)
    {
        this->unitList->remove(vehicles);
        this->unitList->remove(infantries);
        victory = true;
    }
    else
    {
        vector<Vehicle *> thisVehicle = this->unitList->getVehicles();
        vector<Infantry *> thisInfantries = this->unitList->getInfantries();

        vector<Vehicle *> filteredNotInCombinationA;
        for (auto *v : thisVehicle)
        {
            for (auto *i : vehicles)
            {
                if (v == i)
                {
                    filteredNotInCombinationA.push_back(v);
                    continue;
                }
            }
        }
        vector<Infantry *> filteredNotInCombinationB;
        for (auto *i : thisInfantries)
        { // Iterate over thisInfantries instead
            for (auto *j : infantries)
            {
                if (i == j)
                {
                    filteredNotInCombinationB.push_back(i);
                    continue;
                }
            }
        }
        int sumfilteredNotInCombinationA = sp::sumPointsofUnitList(filteredNotInCombinationA);
        int sumfilteredNotInCombinationB = sp::sumPointsofUnitList(filteredNotInCombinationB);

        if (sumfilteredNotInCombinationA > sumfilteredNotInCombinationB)
        {
            victory = true;
            this->unitList->clear();
        }
        else
            battle = false;
    }
    if (victory)
    {
        UnitList *enemyUnitList = enemy->getUnitList();
        // insert enemyUnitList to this->unitList
        for (int i = 0; i < enemyUnitList->getCount(); i++)
        {
            this->unitList->insert(enemyUnitList->getUnit(i));
            this->resetLF_EXP();
        }
    }
    if (!battle)
    {
        for (int i = 0; i < unitList->getCount(); i++)
        {
            Unit *unit = unitList->getUnit(i);
            unit->setWeight(unit->getWeight() * 0.9);
        }
        resetLF_EXP();
    }
}
void LiberationArmy::fightIfDefense(Army *enemy)
{
    this->setEXP(this->getEXP() * 1.3);
    this->setLF(this->getLF() * 1.3);

    while (true)
    {
        if (this->EXP >= enemy->getEXP() && this->LF >= enemy->getLF())
            return;

        if (this->EXP < enemy->getEXP() && this->LF < enemy->getLF())
        {
            for (int i = 0; i < unitList->getCount(); i++)
            {
                if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
                {
                    Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
                    vehicle->setQuantity(sp::closestFibonacci(vehicle->getQuantity()));
                }
                else
                {
                    Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
                    infantry->setQuantity(sp::closestFibonacci(infantry->getQuantity()));
                }
            }
        }
        else
        {
            for (int i = 0; i < unitList->getCount(); i++)
            {
                if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
                {
                    Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
                    vehicle->setQuantity(vehicle->getQuantity() * 0.9);
                }
                else
                {
                    Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
                    infantry->setQuantity(infantry->getQuantity() * 0.9);
                }
            }
            break;
        }
    }
}
string LiberationArmy::str() const
{
    // format: LiberationArmy[name=<name>, LF=<LF>, EXP=<EXP>]
    string re = "LiberationArmy[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) +
                +",unitList=" + unitList->str() + ",battleField=" + battleField->str() + "]";
    return re;
}
// Implement the method ARVN
void ARVN::fightIfAttack(Army *enemy)
{
    /*
    Quân đội Sài Gòn sẽ thất bại và cập nhật lại mất đi 20% số lượng (quantity) ở
mỗi đơn vị quân sự. Cập nhật lại chỉ số chiến đấu tương ứng. Nếu đơn vị quân
sự nào có số lượng (quantity) chỉ còn 1, xoá đơn vị quân sự đó khỏi danhh sách
và cập nhật lại các chỉ số.
    */
    for (int i = 0; i < unitList->getCount(); i++)
    {
        if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
            vehicle->setQuantity(vehicle->getQuantity() * 0.8);
            if (vehicle->getQuantity() < 1)
            {
                unitList->remove(vehicle);
            }
        }
        else
        {
            Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
            infantry->setQuantity(infantry->getQuantity() * 0.8);
            if (infantry->getQuantity() < 1)
            {
                unitList->remove(infantry);
            }
        }
    }
}
void ARVN::fightIfDefense(Army *enemy)
{
    /*
    . Theo mô tả trên, Quân Giải phóng có thể sẽ giao
tranh và giành chiến thắng, lúc này, Quân đội Sài Gòn sẽ bị tịch thu và trưng dụng
các đơn vị quân sự (như mô tả ở phần trên), đồng nghĩa việc cần xoá các đơn vị
này khỏi danh sách của Quân đội Sài Gòn. Do thất bại nên mỗi đơn vị quân sự còn
lại bị mất đi 20% trọng số (weight). Trong trường hợp giao tranh không xảy ra, lực
lượng của Quân đội Sài Gòn được bảo toàn.
    */
    if (unitList->getCount() == 0)
        return;
    double newLF = LF * 1.5, newEXP = EXP * 1.5;
    double enemyLF = enemy->getLF(), enemyEXP = enemy->getEXP();
    for (int i = 0; i < unitList->getCount(); i++)
    {
        if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
            vehicle->setWeight(vehicle->getWeight() * 0.8);
            if (vehicle->getWeight() < 1)
            {
                unitList->remove(vehicle);
            }
        }
        else
        {
            Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
            infantry->setWeight(infantry->getWeight() * 0.8);
            if (infantry->getWeight() < 1)
            {
                unitList->remove(infantry);
            }
        }
    }
    LF = newLF;
    EXP = newEXP;
}
string ARVN::str() const
{
    // format: ARVN[name=<name>, LF=<LF>, EXP=<EXP>]
    string re = "ARVN[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) +
                +",unitList=" + unitList->str() + ",battleField=" + battleField->str() + "]";
    return re;
}
// Implement the method UnitList

UnitList::UnitList(Army *army)
{
    int S = army->getEXP() + army->getLF();
    bool isSpecialof3 = sp::isSpecialNumber(S, 3);
    bool isSpecialof5 = sp::isSpecialNumber(S, 5);
    bool isSpecialof7 = sp::isSpecialNumber(S, 7);
    if (isSpecialof3 & isSpecialof5 & isSpecialof7)
        capacity = 12;
    else
        capacity = 8;
}

bool UnitList::insert(Unit *unit)
{
    if (count >= capacity)
        return false;

    auto updateQuantity = [&](auto *existingUnit)
    {
        existingUnit->setQuantity(existingUnit->getQuantity() + unit->getQuantity());
        return true;
    }; // lambda function in python, C++14

    if (unit->getUnitClassify() == UnitClassify::VEHICLE)
    {
        if (auto *vehicle = getVehicleViaType(static_cast<Vehicle *>(unit)->getVehicleType()))
            return updateQuantity(vehicle);
    }
    else
    {
        if (auto *infantry = getInfantryViaType(static_cast<Infantry *>(unit)->getInfantryType()))
            return updateQuantity(infantry);
    }

    try
    {
        auto *newNode = new UnitNode(unit, nullptr, tail);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
        {
            if (tail)
                tail->next = newNode;
            tail = newNode;
            if (!head)
                head = tail;
        }
        else
        {
            newNode->next = head;
            if (head)
                head->prev = newNode;
            head = newNode;
            if (!tail)
                tail = head;
        }
        count++;
        return true;
    }
    catch (const std::exception &)
    {
        return false;
    }
}
bool UnitList::isContain(VehicleType vehicleType)
{
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(current->unit);
            if (vehicle->getVehicleType() == vehicleType)
                return true;
        }
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType)
{
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::INFANTRY)
        {
            Infantry *infantry = static_cast<Infantry *>(current->unit);
            if (infantry->getInfantryType() == infantryType)
                return true;
        }
    }
    return false;
}
Vehicle *UnitList::getVehicleViaType(VehicleType vehicleType)
{
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(current->unit);
            if (vehicle->getVehicleType() == vehicleType)
                return vehicle;
        }
        else
            break;
    }
    return nullptr;
}
Infantry *UnitList::getInfantryViaType(InfantryType infantryType)
{
    for (UnitNode *current = tail; current != nullptr; current = current->prev)
    {
        if (current->unit->getUnitClassify() == UnitClassify::INFANTRY)
        {
            Infantry *infantry = static_cast<Infantry *>(current->unit);
            if (infantry->getInfantryType() == infantryType)
                return infantry;
        }
        else
            break;
    }
    return nullptr;
}
vector<Vehicle *> UnitList::getVehicles() const
{
    vector<Vehicle *> vehicles;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
            vehicles.push_back(static_cast<Vehicle *>(current->unit));
        else
            break;
    }
    return vehicles;
}
vector<Infantry *> UnitList::getInfantries() const
{
    vector<Infantry *> infantries;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::INFANTRY)
            infantries.push_back(static_cast<Infantry *>(current->unit));
        else
            break;
    }
    return infantries;
}
vector<Unit *> UnitList::getUnits() const
{
    vector<Unit *> units;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        units.push_back(current->unit);
    }
    return units;
}
void UnitList::clear()
{
    for (UnitNode *current = head; current != nullptr; current = head)
    {
        head = current->next;
        delete current;
    }
    head = tail = nullptr;
    count = 0;
}

void UnitList::remove(Unit *unit)
{
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit == unit)
        {
            if (current->prev)
                current->prev->next = current->next;
            if (current->next)
                current->next->prev = current->prev;
            if (current == head)
                head = current->next;
            if (current == tail)
                tail = current->prev;
            delete current;
            count--;
            return;
        }
    }
}
void UnitList::remove(vector<Vehicle *> vehicles)
{
    for (Vehicle *vehicle : vehicles)
    {
        remove(vehicle);
    }
}
void UnitList::remove(vector<Infantry *> infantries)
{
    for (Infantry *infantry : infantries)
    {
        remove(infantry);
    }
}
string UnitList::str() const
{
    // format: UnitList[count_vehicle=<v_c>;count_infantry=<i_c>;<unit_list>]
    string re = "";
    re += "UnitList[count_vehicle=";

    int count_vehicle = 0;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
            count_vehicle++;
    }
    re += to_string(count_vehicle);

    re += ";count_infantry=";
    int count_infantry = count - count_vehicle;
    re += to_string(count_infantry);

    re += ";";
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        re += current->unit->str();
        if (current->next != nullptr)
            re += ",";
    }
    re += "]";

    return re;
}
// Implement the method Position
Position::Position(const string &str)
{
    // format: (x, y)
    // step 1: remove ( and )
    string temp = str.substr(1, str.size() - 2);
    // step 2: split by ,
    size_t pos = temp.find(",");
    string x = temp.substr(0, pos);
    string y = temp.substr(pos + 1);
    // step 3: convert to int
    this->r = stoi(x);
    this->c = stoi(y);
}
string Position::str() const
{
    // format: (x, y)
    stringstream ss;
    ss << "(" << r << ", " << c << ")";
    return ss.str();
}

// Implement the method Mountain
void Mountain::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 4;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        auto unit = ARVN->getUnitList()->getUnit(i);
        int gapToMountain = sp::euclidDistance(unit->getPos(), pos);
        if (gapToMountain <= impactRange)
        {
            if (unit->getUnitClassify() == UnitClassify::VEHICLE)
                ARVN->setLF(ARVN->getLF() - 0.05 * static_cast<Vehicle *>(unit)->getAttackScore());
            else
                ARVN->setEXP(ARVN->getEXP() + 0.2 * static_cast<Infantry *>(unit)->getAttackScore());
        }
    }
}
void Mountain::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    const int impactRange = 2;
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        auto unit = liberationArmy->getUnitList()->getUnit(i);
        int gapToMountain = sp::euclidDistance(unit->getPos(), pos);
        if (gapToMountain <= impactRange)
        {
            if (unit->getUnitClassify() == UnitClassify::VEHICLE)
                liberationArmy->setLF(liberationArmy->getLF() - 0.1 * static_cast<Vehicle *>(unit)->getAttackScore());
            else
                liberationArmy->setEXP(liberationArmy->getEXP() + 0.3 * static_cast<Infantry *>(unit)->getAttackScore());
        }
    }
}
// Implement the method River
void River::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 2;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
            continue;
        if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            ARVN->setEXP(ARVN->getEXP() * 0.9);
    }
}
void River::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    const int impactRange = 2;
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
            continue;
        if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            liberationArmy->setEXP(liberationArmy->getEXP() * 0.9);
    }
}
// IMPLEMENT THE METHOD Urban
void Urban::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 3;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
            continue;
        if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            ARVN->setEXP(ARVN->getEXP() + 0.1 * static_cast<Infantry *>(unit)->getAttackScore());
    }
}
void Urban::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
        {

        }
        else {
            const int impactRange = 5;
            if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            {
                // if unit is infantry and special forces
                Infantry *infantry = static_cast<Infantry *>(unit);
                if (infantry->getInfantryType()==InfantryType::SPECIALFORCES||
                    infantry->getInfantryType()==InfantryType::REGULARINFANTRY)
                {
                    
                }
            }
        }
    }
}
// Implement the method Fortification
void Fortification::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 3;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
            continue;
        if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            ARVN->setEXP(ARVN->getEXP() + 0.2 * static_cast<Infantry *>(unit)->getAttackScore());
    }
}
// Implement the method HCMCampaign

HCMCampaign::HCMCampaign(const string &config_file_path)
{
    config = new Configuration(config_file_path);

    battleField = new BattleField(config->getNumRows(), config->getNumCols(), config->getArrayForest(),
                                  config->getArrayRiver(), config->getArrayFortification(),
                                  config->getArrayUrban(), config->getArraySpecialZone());

    liberationArmy = new LiberationArmy(config->getLiberationUnits(), config->liberUnitsSize(), "LiberationArmy", battleField);
    VNCHARMY = new ARVN(config->getARVNUnits(), config->ARVNUnitsSize(), "ARVN", battleField);
}
string HCMCampaign::printResult()
{
    /*
    format: LIBERATIONARMY[LF=<LF>,EXP=<EXP>]-ARVN[LF=<LF>,EXP=<EXP>]
    */
    int LF_LiberationArmy = liberationArmy->getLF();
    int EXP_LiberationArmy = liberationArmy->getEXP();
    int LF_ARVN = VNCHARMY->getLF();
    int EXP_ARVN = VNCHARMY->getEXP();

    stringstream ss;
    ss << "LIBERATIONARMY[LF=" << LF_LiberationArmy << ",EXP=" << EXP_LiberationArmy
       << "]-ARVN[LF=" << LF_ARVN << ",EXP=" << EXP_ARVN << "]";

    return ss.str();
}
void HCMCampaign::afterfightMethod()
{
    // vn army
    auto removeWeakUnits = [](UnitList *unitList)
    {
        for (int i = 0; i < unitList->getCount(); i++)
        {
            Unit *unit = unitList->getUnit(i);
            if (unit->getAttackScore() <= 5)
                unitList->remove(unit);
        }
    };
    removeWeakUnits(VNCHARMY->getUnitList());
    removeWeakUnits(liberationArmy->getUnitList());
}
void HCMCampaign::run()
{
    int myEventCode = this->config->getEventCode();
    int n_rows = this->config->getNumRows();
    int n_cols = this->config->getNumCols();
    bool Is1975 = config->getEventCode() >= 75;

    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            Position pos(i, j);
            TerrainElement *terrainElement = this->battleField->getTerrain()[i][j];
            terrainElement->getEffect(this->liberationArmy);
            terrainElement->getEffect(this->VNCHARMY);

            if (!Is1975)
            {
                VNCHARMY->fight(liberationArmy, false);
                liberationArmy->fight(VNCHARMY, true);
            }
            VNCHARMY->fight(liberationArmy, true);
            liberationArmy->fight(VNCHARMY, false);
        }
    }
    afterfightMethod();
}

HCMCampaign::~HCMCampaign()
{
    try {
    delete VNCHARMY;
    delete liberationArmy;
    delete battleField;
    delete config;
    }
    catch (const std::exception &e) {}
}
// Implement the method BattleField
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                         vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                         vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
{
    this->n_rows = n_rows;
    this->n_cols = n_cols;
    // TODO
    // create 2d array of TerrainElement with default is Road
    terrain = new TerrainElement **[n_rows];
    for (int i = 0; i < n_rows; i++)
    {
        terrain[i] = new TerrainElement *[n_cols];
        for (int j = 0; j < n_cols; j++)
        {
            terrain[i][j] = new Road(Position(i, j));
        }
    }
    // update the terrain with the given positions
    for (Position *pos : arrayForest)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new Mountain(*pos);
    }
    for (Position *pos : arrayRiver)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new River(*pos);
    }
    for (Position *pos : arrayFortification)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new Fortification(*pos);
    }
    for (Position *pos : arrayUrban)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new Urban(*pos);
    }
    for (Position *pos : arraySpecialZone)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new SpecialZone(*pos);
    }
}
string BattleField::str()
{
    // format: BattleField[attr_name=<attr_value>]
    stringstream ss;
    ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return ss.str();
}
BattleField::~BattleField()
{
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            delete terrain[i][j];
        }
        delete[] terrain[i];
    }
    delete[] terrain;
}

// Implement the method Configuration
Configuration::Configuration(const string &config_file_path)
{
    vector<Unit *> liberUnits, VNCHUnits; // tốn stack

    ifstream file(config_file_path);
    string line;
    while (getline(file, line))
    {
        if (line.find("NUM_ROWS") != string::npos)
            num_rows = stoi(line.substr(line.find("=") + 1));
        else if (line.find("NUM_COLS") != string::npos)
            num_cols = stoi(line.substr(line.find("=") + 1));
        else if (line.find("EVENT_CODE") != string::npos)
        {
            eventCode = stoi(line.substr(line.find("=") + 1));
            eventCode = max(0, min(99, eventCode));
        }
        // array list
        else if (line.find("ARRAY_FOREST") != string::npos) // input format: ARRAY_FOREST=[(1,2),(3,5)]
        {
            size_t pos = 0;
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // (1,2),(3,5)
            while ((pos = temp.find("),")) != string::npos)
            {
                string pos_str = temp.substr(0, pos + 1); // (1,2)
                arrayForest.push_back(new Position(pos_str));
                temp.erase(0, pos + 2);
            }
            if (!temp.empty())
                arrayForest.push_back(new Position(temp));
        }
        else if (line.find("ARRAY_RIVER") != string::npos)
        {
            size_t pos = 0;
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // (1,2),(3,5)
            while ((pos = temp.find("),")) != string::npos)
            {
                string pos_str = temp.substr(0, pos + 1); // (1,2)
                arrayRiver.push_back(new Position(pos_str));
                temp.erase(0, pos + 2);
            }
            if (!temp.empty())
                arrayRiver.push_back(new Position(temp));
        }
        else if (line.find("ARRAY_FORTIFICATION") != string::npos)
        {
            size_t pos = 0;
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // (1,2),(3,5)
            while ((pos = temp.find("),")) != string::npos)
            {
                string pos_str = temp.substr(0, pos + 1); // (1,2)
                arrayFortification.push_back(new Position(pos_str));
                temp.erase(0, pos + 2);
            }
            if (!temp.empty())
                arrayFortification.push_back(new Position(temp));
        }
        else if (line.find("ARRAY_URBAN") != string::npos)
        {
            size_t pos = 0;
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // (1,2),(3,5)
            while ((pos = temp.find("),")) != string::npos)
            {
                string pos_str = temp.substr(0, pos + 1); // (1,2)
                arrayUrban.push_back(new Position(pos_str));
                temp.erase(0, pos + 2);
            }
            if (!temp.empty())
                arrayUrban.push_back(new Position(temp));
        }
        else if (line.find("ARRAY_SPECIAL_ZONE") != string::npos)
        {
            size_t pos = 0;
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // (1,2),(3,5)
            while ((pos = temp.find("),")) != string::npos)
            {
                string pos_str = temp.substr(0, pos + 1); // (1,2)
                arraySpecialZone.push_back(new Position(pos_str));
                temp.erase(0, pos + 2);
            }
            if (!temp.empty())
                arraySpecialZone.push_back(new Position(temp));
        }
        // unit list
        else if (line.find("UNIT_LIST=") != string::npos)
        {
            // format: UNIT_LIST=[TANK(5,2,(1,2),0),TANK(5,2,(3,2),1),REGULARINFANTRY(5,2,(1,1),0),REGULARINFANTRY(5,2,(3,3),1)]
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // TANK(5,2,(1,2),0),TANK(5,2,(3,2),1),REGULARINFANTRY(5,2,(1,1),0),REGULARINFANTRY(5,2,(3,3),1)
            /*
            UNIT_NAME(quantity,weight,position,armyBelong Với armyBelong=0 tương ứng với
            Quân Giải phóng, armyBelong=1 tương ứng với Quân đội chính quyền Sài Gòn
            */
            size_t pos = 0;
            while ((pos = temp.find("),")) != string::npos)
            {
                string pos_str = temp.substr(0, pos + 1); // TANK(5,2,(1,2),0)
                extractUnits(pos_str,liberUnits, VNCHUnits);
                temp.erase(0, pos + 2); // TANK(5,2,(3,2),1),REGULARINFANTRY(5,2,(1,1),0),REGULARINFANTRY(5,2,(3,3),1)
            }
            if (!temp.empty())
                extractUnits(temp, liberUnits, VNCHUnits);
        }
    }
    file.close();

    // create the units base on vector liberUnits and ARVNUnits
    this->count_liberUnit = VNCHUnits.size();
    this->count_ARVNUnit= liberUnits.size();

    this->liberationUnits = new Unit*[count_liberUnit];
    this->ARVNUnits = new Unit*[count_ARVNUnit];

    for (int i = 0; i < count_liberUnit; i++) this->liberationUnits[i] = VNCHUnits[i];
    for (int i = 0; i < count_ARVNUnit; i++) this->ARVNUnits[i] = liberUnits[i];
}
void Configuration::extractUnits(string input, vector<Unit*>& liberUnits, vector<Unit*>& ARVNUnits)
{
    // format: TANK(5,2,(1,2),0)
    // step 1: remove ( and ) but hold the string TANK
    string temp = input.substr(0, input.find("(")); // TANK
    // step 2: remove the string TANK and ( but hold the string 5,2,(1,2),0)
    string temp2 = input.substr(input.find("(") + 1); // 5,2,(1,2),0
    // step 3: split by , and save int, int, pos, int
    size_t pos = 0;
    int quantity = stoi(temp2.substr(0, temp2.find(",")));
    temp2.erase(0, temp2.find(",") + 1);
    int weight = stoi(temp2.substr(0, temp2.find(",")));
    temp2.erase(0, temp2.find(",") + 1);
    Position pos(temp2.substr(0, temp2.find(",")));
    temp2.erase(0, temp2.find(",") + 1);
    int armyBelong = stoi(temp2);
    // step 4: create the unit
    hashUnits(quantity, weight, pos, armyBelong, temp, liberUnits, ARVNUnits);
}
void Configuration::hashUnits(int quantity, int weight, Position pos, int armyBelong, string unitName, vector<Unit*>& liberUnits, vector<Unit*>& ARVNUnits)
{
    Unit *unit = nullptr;
    if (unitName == "TRUCK") unit = new Vehicle(quantity, weight, pos, VehicleType::TRUCK);
    else if (unitName == "MORTAR") unit = new Vehicle(quantity, weight, pos, VehicleType::MORTAR);
    else if (unitName == "ANTIAIRCRAFT") unit = new Vehicle(quantity, weight, pos, VehicleType::ANTIAIRCRAFT);
    else if (unitName == "ARMOREDCAR") unit = new Vehicle(quantity, weight, pos, VehicleType::ARMOREDCAR);
    else if (unitName == "APC") unit = new Vehicle(quantity, weight, pos, VehicleType::APC);
    else if (unitName == "ARTILLERY") unit = new Vehicle(quantity, weight, pos, VehicleType::ARTILLERY);
    else if (unitName == "TANK") unit = new Vehicle(quantity, weight, pos, VehicleType::TANK);
    else if (unitName == "SNIPER") unit = new Infantry(quantity, weight, pos, InfantryType::SNIPER);
    else if (unitName == "ANTIAIRCRAFTSQUAD") unit = new Infantry(quantity, weight, pos, InfantryType::ANTIAIRCRAFTSQUAD);
    else if (unitName == "MORTARSQUAD") unit = new Infantry(quantity, weight, pos, InfantryType::MORTARSQUAD);
    else if (unitName == "ENGINEER") unit = new Infantry(quantity, weight, pos, InfantryType::ENGINEER);
    else if (unitName == "SPECIALFORCES") unit = new Infantry(quantity, weight, pos, InfantryType::SPECIALFORCES);
    else if (unitName == "REGULARINFANTRY") unit = new Infantry(quantity, weight, pos, InfantryType::REGULARINFANTRY);
    
    if (armyBelong == 0) liberUnits.push_back(unit);
    else ARVNUnits.push_back(unit);
}
Configuration::~Configuration()
{
    for (Position *pos : arrayForest) delete pos;
    for (Position *pos : arrayRiver) delete pos;
    for (Position *pos : arrayFortification) delete pos;
    for (Position *pos : arrayUrban) delete pos;
    for (Position *pos : arraySpecialZone) delete pos;

    for (int i = 0; i < count_liberUnit; i++) delete liberationUnits[i];
    for (int i = 0; i < count_ARVNUnit; i++) delete ARVNUnits[i];
    delete[] liberationUnits;
    delete[] ARVNUnits;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////