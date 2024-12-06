#pragma once

#include <string>
#include <map>
#include <chrono>

namespace smart_food {
namespace core {

class Ingredient {
public:
    enum class Unit {
        GRAM,
        KILOGRAM,
        MILLILITER,
        LITER,
        PIECE,
        TEASPOON,
        TABLESPOON,
        CUP,
        OUNCE,
        POUND
    };

    // Constructors
    Ingredient();
    explicit Ingredient(const std::string& name);
    Ingredient(const std::string& name, double quantity, Unit unit);

    // Getters
    const std::string& getId() const;
    const std::string& getName() const;
    double getQuantity() const;
    Unit getUnit() const;
    double getUnitPrice() const;
    const std::chrono::system_clock::time_point& getExpiryDate() const;
    const std::map<std::string, double>& getNutritionalInfo() const;
    
    // Setters
    void setName(const std::string& name);
    void setQuantity(double quantity);
    void setUnit(Unit unit);
    void setUnitPrice(double price);
    void setExpiryDate(const std::chrono::system_clock::time_point& date);
    
    // Operations
    void scale(double factor);
    void addNutritionalInfo(const std::string& nutrient, double value);
    void removeNutritionalInfo(const std::string& nutrient);
    double calculateCost() const;
    
    // Utility functions
    bool isExpired() const;
    bool isLowQuantity() const;
    std::string serialize() const;
    static Ingredient deserialize(const std::string& data);

    // Unit conversion
    static double convertUnit(double value, Unit from, Unit to);
    static std::string unitToString(Unit unit);
    static Unit stringToUnit(const std::string& unitStr);

private:
    std::string id_;
    std::string name_;
    double quantity_;
    Unit unit_;
    double unitPrice_;
    std::chrono::system_clock::time_point expiryDate_;
    std::map<std::string, double> nutritionalInfo_;

    void generateId();
};

} // namespace core
} // namespace smart_food