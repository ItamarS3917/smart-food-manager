#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <map>
#include "ingredient.hpp"

namespace smart_food {
namespace core {

class Recipe {
public:
    enum class Difficulty {
        EASY,
        MEDIUM,
        HARD
    };

    struct Step {
        int order;
        std::string description;
        std::chrono::minutes duration;
    };

    // Constructors
    Recipe();
    explicit Recipe(const std::string& name);
    Recipe(const std::string& name, const std::string& description);

    // Getters
    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    Difficulty getDifficulty() const;
    int getServings() const;
    std::chrono::minutes getTotalTime() const;
    const std::vector<std::shared_ptr<Ingredient>>& getIngredients() const;
    const std::vector<Step>& getSteps() const;
    const std::map<std::string, double>& getNutritionalInfo() const;
    
    // Setters
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setDifficulty(Difficulty difficulty);
    void setServings(int servings);
    
    // Operations
    void addIngredient(const std::shared_ptr<Ingredient>& ingredient);
    void removeIngredient(const std::string& ingredientId);
    void addStep(const Step& step);
    void removeStep(int order);
    void reorderStep(int oldOrder, int newOrder);
    void scaleServings(int newServings);
    
    // Calculations
    double calculateTotalCost() const;
    void updateNutritionalInfo();
    
    // Utility functions
    bool isValid() const;
    std::string serialize() const;
    static Recipe deserialize(const std::string& data);

private:
    std::string id_;
    std::string name_;
    std::string description_;
    Difficulty difficulty_;
    int servings_;
    std::vector<std::shared_ptr<Ingredient>> ingredients_;
    std::vector<Step> steps_;
    std::map<std::string, double> nutritionalInfo_;

    void generateId();
    void recalculateNutritionalInfo();
};

} // namespace core
} // namespace smart_food