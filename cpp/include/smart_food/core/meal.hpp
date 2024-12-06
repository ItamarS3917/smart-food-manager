#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include "recipe.hpp"
#include "ingredient.hpp"

namespace smart_food {
namespace core {

class Meal {
public:
    enum class Type {
        BREAKFAST,
        LUNCH,
        DINNER,
        SNACK
    };

    enum class Status {
        PLANNED,
        SHOPPING,
        PREPARING,
        READY,
        CONSUMED
    };

    // Constructors
    Meal();
    explicit Meal(const std::string& name);
    Meal(const std::string& name, Type type);

    // Getters
    const std::string& getName() const;
    Type getType() const;
    Status getStatus() const;
    const std::chrono::system_clock::time_point& getPlannedTime() const;
    const std::shared_ptr<Recipe>& getRecipe() const;
    const std::vector<std::shared_ptr<Ingredient>>& getIngredients() const;
    double getEstimatedCost() const;
    int getServings() const;
    
    // Setters
    void setName(const std::string& name);
    void setType(Type type);
    void setStatus(Status status);
    void setPlannedTime(const std::chrono::system_clock::time_point& time);
    void setRecipe(const std::shared_ptr<Recipe>& recipe);
    void setServings(int servings);

    // Operations
    void addIngredient(const std::shared_ptr<Ingredient>& ingredient);
    void removeIngredient(const std::string& ingredientId);
    void scaleServings(int newServings);
    void updateCost();
    
    // Utility functions
    bool isComplete() const;
    double calculateNutritionalValue() const;
    std::string serialize() const;
    static Meal deserialize(const std::string& data);

private:
    std::string name_;
    Type type_;
    Status status_;
    std::chrono::system_clock::time_point plannedTime_;
    std::shared_ptr<Recipe> recipe_;
    std::vector<std::shared_ptr<Ingredient>> ingredients_;
    double estimatedCost_;
    int servings_;

    void recalculateEstimatedCost();
};

} // namespace core
} // namespace smart_food