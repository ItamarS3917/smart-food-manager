#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <mutex>
#include "meal.hpp"
#include "recipe.hpp"
#include "ingredient.hpp"

namespace smart_food {
namespace core {

class Storage {
public:
    // Singleton instance
    static Storage& getInstance();

    // Prevent copying and assignment
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

    // Meal management
    std::shared_ptr<Meal> getMeal(const std::string& id) const;
    std::vector<std::shared_ptr<Meal>> getMeals() const;
    std::vector<std::shared_ptr<Meal>> getMealsByDate(const std::chrono::system_clock::time_point& date) const;
    void addMeal(const std::shared_ptr<Meal>& meal);
    void updateMeal(const std::shared_ptr<Meal>& meal);
    void removeMeal(const std::string& id);

    // Recipe management
    std::shared_ptr<Recipe> getRecipe(const std::string& id) const;
    std::vector<std::shared_ptr<Recipe>> getRecipes() const;
    std::vector<std::shared_ptr<Recipe>> searchRecipes(const std::string& query) const;
    void addRecipe(const std::shared_ptr<Recipe>& recipe);
    void updateRecipe(const std::shared_ptr<Recipe>& recipe);
    void removeRecipe(const std::string& id);

    // Ingredient management
    std::shared_ptr<Ingredient> getIngredient(const std::string& id) const;
    std::vector<std::shared_ptr<Ingredient>> getIngredients() const;
    std::vector<std::shared_ptr<Ingredient>> getLowStockIngredients() const;
    std::vector<std::shared_ptr<Ingredient>> getExpiringIngredients() const;
    void addIngredient(const std::shared_ptr<Ingredient>& ingredient);
    void updateIngredient(const std::shared_ptr<Ingredient>& ingredient);
    void removeIngredient(const std::string& id);

    // Persistence operations
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    void clear();

    // Statistics and analytics
    double calculateTotalInventoryValue() const;
    std::map<std::string, double> getInventoryStatistics() const;
    std::map<std::string, double> getWasteStatistics() const;

private:
    Storage() = default;  // Private constructor for singleton

    mutable std::mutex mutex_;
    std::map<std::string, std::shared_ptr<Meal>> meals_;
    std::map<std::string, std::shared_ptr<Recipe>> recipes_;
    std::map<std::string, std::shared_ptr<Ingredient>> ingredients_;

    // Helper functions
    void validateMeal(const std::shared_ptr<Meal>& meal) const;
    void validateRecipe(const std::shared_ptr<Recipe>& recipe) const;
    void validateIngredient(const std::shared_ptr<Ingredient>& ingredient) const;
};

} // namespace core
} // namespace smart_food