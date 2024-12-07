#pragma once
#include <string>
#include <map>
#include <chrono>

namespace smart_food {
namespace core {

/**
 * @brief Represents an ingredient in a recipe or meal.
 *
 * The Ingredient class manages all aspects of a food ingredient including
 * its quantity, unit of measurement, price, expiration date, and nutritional
 * information. It provides functionality for scaling quantities, unit conversion,
 * and cost calculation.
 */
class Ingredient {
public:
    /**
     * @brief Units of measurement for ingredients
     */
    enum class Unit {
        GRAM,      ///< Weight in grams
        KILOGRAM,  ///< Weight in kilograms
        MILLILITER,///< Volume in milliliters
        LITER,     ///< Volume in liters
        PIECE,     ///< Count of individual items
        TEASPOON,  ///< Volume in teaspoons
        TABLESPOON,///< Volume in tablespoons
        CUP,       ///< Volume in cups
        OUNCE,     ///< Weight in ounces
        POUND      ///< Weight in pounds
    };

    // Constructors
    /**
     * @brief Default constructor.
     * Creates a new ingredient with default values and generates a unique ID.
     */
    Ingredient();

    /**
     * @brief Constructor with name.
     * @param name The name of the ingredient
     * @throws std::invalid_argument if name is empty
     */
    explicit Ingredient(const std::string& name);

    /**
     * @brief Constructor with name, quantity, and unit.
     * @param name The name of the ingredient
     * @param quantity The amount of the ingredient
     * @param unit The unit of measurement
     * @throws std::invalid_argument if name is empty or quantity is negative
     */
    Ingredient(const std::string& name, double quantity, Unit unit);

    // Getters
    /**
     * @brief Get the unique identifier of the ingredient
     * @return The ingredient's ID
     */
    const std::string& getId() const;

    /**
     * @brief Get the name of the ingredient
     * @return The ingredient's name
     */
    const std::string& getName() const;

    /**
     * @brief Get the quantity of the ingredient
     * @return The amount in the specified unit
     */
    double getQuantity() const;

    /**
     * @brief Get the unit of measurement
     * @return The unit type
     */
    Unit getUnit() const;

    /**
     * @brief Get the price per unit
     * @return The unit price
     */
    double getUnitPrice() const;

    /**
     * @brief Get the expiration date
     * @return The expiry timestamp
     */
    const std::chrono::system_clock::time_point& getExpiryDate() const;

    /**
     * @brief Get the nutritional information
     * @return Map of nutrient names to values
     */
    const std::map<std::string, double>& getNutritionalInfo() const;

    // Setters
    /**
     * @brief Set the ingredient name
     * @param name The new name
     * @throws std::invalid_argument if name is empty
     */
    void setName(const std::string& name);

    /**
     * @brief Set the quantity
     * @param quantity The new amount
     * @throws std::invalid_argument if quantity is negative
     */
    void setQuantity(double quantity);

    /**
     * @brief Set the unit of measurement
     * @param unit The new unit
     */
    void setUnit(Unit unit);

    /**
     * @brief Set the price per unit
     * @param price The new unit price
     * @throws std::invalid_argument if price is negative
     */
    void setUnitPrice(double price);

    /**
     * @brief Set the expiration date
     * @param date The new expiry timestamp
     */
    void setExpiryDate(const std::chrono::system_clock::time_point& date);

    // Operations
    /**
     * @brief Scale the ingredient quantity by a factor
     * @param factor The scaling factor
     * @throws std::invalid_argument if factor is not positive
     */
    void scale(double factor);

    /**
     * @brief Add or update nutritional information
     * @param nutrient The name of the nutrient
     * @param value The amount of the nutrient
     * @throws std::invalid_argument if value is negative
     */
    void addNutritionalInfo(const std::string& nutrient, double value);

    /**
     * @brief Remove a nutrient from nutritional information
     * @param nutrient The name of the nutrient to remove
     */
    void removeNutritionalInfo(const std::string& nutrient);

    /**
     * @brief Calculate the total cost of the ingredient
     * @return The cost (quantity * unit price)
     */
    double calculateCost() const;

    // Utility functions
    /**
     * @brief Check if the ingredient is expired
     * @return true if current time is past expiry date
     */
    bool isExpired() const;

    /**
     * @brief Check if the quantity is below recommended threshold
     * @return true if quantity is below threshold for the unit type
     */
    bool isLowQuantity() const;

    /**
     * @brief Serialize the ingredient to JSON format
     * @return JSON string representation
     */
    std::string serialize() const;

    /**
     * @brief Create an ingredient from JSON format
     * @param data JSON string containing ingredient data
     * @return New Ingredient instance
     */
    static Ingredient deserialize(const std::string& data);

    // Unit conversion
    /**
     * @brief Convert a value between units
     * @param value The amount to convert
     * @param from Source unit
     * @param to Target unit
     * @return Converted value
     * @throws std::invalid_argument if units are incompatible
     */
    static double convertUnit(double value, Unit from, Unit to);

    /**
     * @brief Convert unit enum to string representation
     * @param unit The unit to convert
     * @return String representation of the unit
     */
    static std::string unitToString(Unit unit);

    /**
     * @brief Convert string to unit enum
     * @param unitStr String representation of the unit
     * @return Unit enum value
     * @throws std::invalid_argument if string is not a valid unit
     */
    static Unit stringToUnit(const std::string& unitStr);

private:
    std::string id_;         ///< Unique identifier
    std::string name_;       ///< Ingredient name
    double quantity_;        ///< Amount of ingredient
    Unit unit_;             ///< Unit of measurement
    double unitPrice_;       ///< Price per unit
    std::chrono::system_clock::time_point expiryDate_; ///< Expiration date
    std::map<std::string, double> nutritionalInfo_;    ///< Nutritional information

    /**
     * @brief Generate a unique ID for the ingredient
     * Format: "ing_" followed by 8 random hexadecimal digits
     */
    void generateId();
};

} // namespace core
} // namespace smart_food