#include <smart_food/core/ingredient.hpp>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <random>
#include <json/json.hpp>

using json = nlohmann::json;

namespace smart_food {
namespace core {

Ingredient::Ingredient()
    : quantity_(0.0)
    , unit_(Unit::GRAM)
    , unitPrice_(0.0) {
    generateId();
}

Ingredient::Ingredient(const std::string& name)
    : name_(name)
    , quantity_(0.0)
    , unit_(Unit::GRAM)
    , unitPrice_(0.0) {
    generateId();
}

Ingredient::Ingredient(const std::string& name, double quantity, Unit unit)
    : name_(name)
    , quantity_(quantity)
    , unit_(unit)
    , unitPrice_(0.0) {
    generateId();
    if (quantity < 0) {
        throw std::invalid_argument("Quantity cannot be negative");
    }
}

// Getters
const std::string& Ingredient::getId() const { return id_; }
const std::string& Ingredient::getName() const { return name_; }
double Ingredient::getQuantity() const { return quantity_; }
Ingredient::Unit Ingredient::getUnit() const { return unit_; }
double Ingredient::getUnitPrice() const { return unitPrice_; }
const std::chrono::system_clock::time_point& Ingredient::getExpiryDate() const { return expiryDate_; }
const std::map<std::string, double>& Ingredient::getNutritionalInfo() const { return nutritionalInfo_; }

// Setters
void Ingredient::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Ingredient name cannot be empty");
    }
    name_ = name;
}

void Ingredient::setQuantity(double quantity) {
    if (quantity < 0) {
        throw std::invalid_argument("Quantity cannot be negative");
    }
    quantity_ = quantity;
}

void Ingredient::setUnit(Unit unit) {
    unit_ = unit;
}

void Ingredient::setUnitPrice(double price) {
    if (price < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }
    unitPrice_ = price;
}

void Ingredient::setExpiryDate(const std::chrono::system_clock::time_point& date) {
    expiryDate_ = date;
}

// Operations
void Ingredient::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    quantity_ *= factor;
}

void Ingredient::addNutritionalInfo(const std::string& nutrient, double value) {
    if (value < 0) {
        throw std::invalid_argument("Nutritional value cannot be negative");
    }
    nutritionalInfo_[nutrient] = value;
}

void Ingredient::removeNutritionalInfo(const std::string& nutrient) {
    nutritionalInfo_.erase(nutrient);
}

double Ingredient::calculateCost() const {
    return quantity_ * unitPrice_;
}

bool Ingredient::isExpired() const {
    return std::chrono::system_clock::now() > expiryDate_;
}

bool Ingredient::isLowQuantity() const {
    // This could be made configurable per ingredient type
    static const std::map<Unit, double> lowThresholds = {
        {Unit::GRAM, 100.0},
        {Unit::KILOGRAM, 0.1},
        {Unit::MILLILITER, 100.0},
        {Unit::LITER, 0.1},
        {Unit::PIECE, 2.0}
    };

    auto it = lowThresholds.find(unit_);
    if (it != lowThresholds.end()) {
        return quantity_ <= it->second;
    }
    return false;
}

void Ingredient::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    ss << "ing_";
    for (int i = 0; i < 8; i++) {
        ss << std::hex << dis(gen);
    }
    id_ = ss.str();
}

std::string Ingredient::unitToString(Unit unit) {
    switch (unit) {
        case Unit::GRAM: return "g";
        case Unit::KILOGRAM: return "kg";
        case Unit::MILLILITER: return "ml";
        case Unit::LITER: return "l";
        case Unit::PIECE: return "pc";
        case Unit::TEASPOON: return "tsp";
        case Unit::TABLESPOON: return "tbsp";
        case Unit::CUP: return "cup";
        case Unit::OUNCE: return "oz";
        case Unit::POUND: return "lb";
        default: throw std::invalid_argument("Unknown unit");
    }
}

Ingredient::Unit Ingredient::stringToUnit(const std::string& unitStr) {
    if (unitStr == "g") return Unit::GRAM;
    if (unitStr == "kg") return Unit::KILOGRAM;
    if (unitStr == "ml") return Unit::MILLILITER;
    if (unitStr == "l") return Unit::LITER;
    if (unitStr == "pc") return Unit::PIECE;
    if (unitStr == "tsp") return Unit::TEASPOON;
    if (unitStr == "tbsp") return Unit::TABLESPOON;
    if (unitStr == "cup") return Unit::CUP;
    if (unitStr == "oz") return Unit::OUNCE;
    if (unitStr == "lb") return Unit::POUND;
    throw std::invalid_argument("Unknown unit string: " + unitStr);
}

std::string Ingredient::serialize() const {
    json j;
    j["id"] = id_;
    j["name"] = name_;
    j["quantity"] = quantity_;
    j["unit"] = static_cast<int>(unit_);
    j["unitPrice"] = unitPrice_;
    j["expiryDate"] = std::chrono::system_clock::to_time_t(expiryDate_);
    j["nutritionalInfo"] = nutritionalInfo_;
    
    return j.dump();
}

Ingredient Ingredient::deserialize(const std::string& data) {
    json j = json::parse(data);
    
    Ingredient ingredient(j["name"].get<std::string>());
    ingredient.id_ = j["id"].get<std::string>();
    ingredient.setQuantity(j["quantity"].get<double>());
    ingredient.setUnit(static_cast<Unit>(j["unit"].get<int>()));
    ingredient.setUnitPrice(j["unitPrice"].get<double>());
    ingredient.setExpiryDate(std::chrono::system_clock::from_time_t(j["expiryDate"].get<time_t>()));
    
    for (const auto& [nutrient, value] : j["nutritionalInfo"].items()) {
        ingredient.addNutritionalInfo(nutrient, value.get<double>());
    }
    
    return ingredient;
}

} // namespace core
} // namespace smart_food
