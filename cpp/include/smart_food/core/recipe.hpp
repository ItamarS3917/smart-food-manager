#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <map>
#include "ingredient.hpp"

namespace smart_food {
namespace core {

/**
 * @brief Recipe class represents a cooking recipe with ingredients, steps, and nutritional information.
 * 
 * This class manages all aspects of a cooking recipe including:
 * - Basic information (name, description, difficulty)
 * - Ingredients list with quantities
 * - Step-by-step preparation instructions
 * - Nutritional information
 * - Serving size management
 */
class Recipe {
public:
    /**
     * @brief Represents the difficulty level of preparing the recipe
     */
    enum class Difficulty {
        EASY,    ///< Simple recipes suitable for beginners
        MEDIUM,  ///< Intermediate recipes requiring some cooking experience
        HARD     ///< Complex recipes for experienced cooks
    };

    /**
     * @brief Represents a single step in the recipe preparation process
     */
    struct Step {
        int order;                     ///< The order/sequence number of this step
        std::string description;       ///< Detailed description of what to do in this step
        std::chrono::minutes duration; ///< Estimated time to complete this step
    };

    // ------------------------
    // Constructors
    // ------------------------

    /**
     * @brief Default constructor
     * Creates a new recipe with default values
     */
    Recipe();

    /**
     * @brief Constructor with name
     * @param name The name of the recipe
     */
    explicit Recipe(const std::string& name);

    /**
     * @brief Constructor with name and description
     * @param name The name of the recipe
     * @param description A detailed description of the recipe
     */
    Recipe(const std::string& name, const std::string& description);

    // ------------------------
    // Getters
    // ------------------------

    /**
     * @brief Get the unique identifier of the recipe
     * @return The recipe's ID
     */
    const std::string& getId() const;

    /**
     * @brief Get the name of the recipe
     * @return The recipe's name
     */
    const std::string& getName() const;

    /**
     * @brief Get the description of the recipe
     * @return The recipe's description
     */
    const std::string& getDescription() const;

    /**
     * @brief Get the difficulty level of the recipe
     * @return The recipe's difficulty
     */
    Difficulty getDifficulty() const;

    /**
     * @brief Get the number of servings this recipe makes
     * @return Number of servings
     */
    int getServings() const;

    /**
     * @brief Calculate the total preparation time
     * @return Total time needed to prepare the recipe
     */
    std::chrono::minutes getTotalTime() const;

    /**
     * @brief Get the list of ingredients
     * @return Vector of ingredients with their quantities
     */
    const std::vector<std::shared_ptr<Ingredient>>& getIngredients() const;

    /**
     * @brief Get the preparation steps
     * @return Vector of preparation steps in order
     */
    const std::vector<Step>& getSteps() const;

    /**
     * @brief Get the nutritional information
     * @return Map of nutritional values (e.g., "calories" -> 500)
     */
    const std::map<std::string, double>& getNutritionalInfo() const;

    // ------------------------
    // Setters
    // ------------------------

    /**
     * @brief Set the recipe name
     * @param name New name for the recipe
     * @throws std::invalid_argument if name is empty
     */
    void setName(const std::string& name);

    /**
     * @brief Set the recipe description
     * @param description New description for the recipe
     */
    void setDescription(const std::string& description);

    /**
     * @brief Set the difficulty level
     * @param difficulty New difficulty level
     */
    void setDifficulty(Difficulty difficulty);

    /**
     * @brief Set the number of servings
     * @param servings New number of servings
     * @throws std::invalid_argument if servings <= 0
     */
    void setServings(int servings);

    // ------------------------
    // Operations
    // ------------------------

    /**
     * @brief Add a new ingredient to the recipe
     * @param ingredient Ingredient to add with its quantity
     * @throws std::invalid_argument if ingredient is null
     */
    void addIngredient(const std::shared_ptr<Ingredient>& ingredient);

    /**
     * @brief Remove an ingredient from the recipe
     * @param ingredientId ID of the ingredient to remove
     */
    void removeIngredient(const std::string& ingredientId);

    /**
     * @brief Add a new preparation step
     * @param step Step to add to the recipe
     * @throws std::invalid_argument if step order <= 0
     */
    void addStep(const Step& step);

    /**
     * @brief Remove a preparation step
     * @param order Order number of the step to remove
     */
    void removeStep(int order);

    /**
     * @brief Change the order of a preparation step
     * @param oldOrder Current order of the step
     * @param newOrder New order for the step
     * @throws std::invalid_argument if orders <= 0 or step not found
     */
    void reorderStep(int oldOrder, int newOrder);

    /**
     * @brief Scale the recipe for a different number of servings
     * @param newServings Target number of servings
     * @throws std::invalid_argument if newServings <= 0
     */
    void scaleServings(int newServings);

    // ------------------------
    // Calculations
    // ------------------------

    /**
     * @brief Calculate the total cost of ingredients
     * @return Total cost of the recipe
     */
    double calculateTotalCost() const;

    /**
     * @brief Update the nutritional information based on ingredients
     */
    void updateNutritionalInfo();

    // ------------------------
    // Utility Functions
    // ------------------------

    /**
     * @brief Check if the recipe is complete and valid
     * @return true if recipe has name, servings > 0, ingredients, and steps
     */
    bool isValid() const;

    /**
     * @brief Convert the recipe to a string format for storage
     * @return JSON string representation of the recipe
     */
    std::string serialize() const;

    /**
     * @brief Create a recipe from a stored string format
     * @param data JSON string containing recipe data
     * @return New Recipe object
     */
    static Recipe deserialize(const std::string& data);

private:
    std::string id_;              ///< Unique identifier for the recipe
    std::string name_;            ///< Name of the recipe
    std::string description_;     ///< Detailed description of the recipe
    Difficulty difficulty_;       ///< Difficulty level of preparation
    int servings_;               ///< Number of servings the recipe makes
    std::vector<std::shared_ptr<Ingredient>> ingredients_;  ///< List of ingredients with quantities
    std::vector<Step> steps_;    ///< Ordered list of preparation steps
    std::map<std::string, double> nutritionalInfo_;  ///< Nutritional values per serving

    /**
     * @brief Generate a unique identifier for the recipe
     */
    void generateId();

    /**
     * @brief Recalculate nutritional information based on ingredients
     */
    void recalculateNutritionalInfo();
};

} // namespace core
} // namespace smart_food