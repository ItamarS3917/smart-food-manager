#include <smart_food/core/meal.hpp>
#include <stdexcept>
#include <json/json.hpp>

using json = nlohmann::json;

namespace smart_food {
namespace core {

Meal::Meal()
    : name_("New Meal")
    , type_(Type::BREAKFAST)
    , status_(Status::PLANNED)
    , estimatedCost_(0.0)
    , servings_(1) {
    plannedTime_ = std::chrono::system_clock::now();
}

Meal::Meal(const std::string& name)
    : name_(name)
    , type_(Type::BREAKFAST)
    , status_(Status::PLANNED)
    , estimatedCost_(0.0)
    , servings_(1) {
    plannedTime_ = std::chrono::system_clock::now();
}

Meal::Meal(const std::string& name, Type type)
    : name_(name)
    , type_(type)
    , status_(Status::PLANNED)
    , estimatedCost_(0.0)
    , servings_(1) {
    plannedTime_ = std::chrono::system_clock::now();
}

// Getters
const std::string& Meal::getName() const { return name_; }
Meal::Type Meal::getType() const { return type_; }
Meal::Status Meal::getStatus() const { return status_; }
const std::chrono::system_clock::time_point& Meal::getPlannedTime() const { return plannedTime_; }
const std::shared_ptr<Recipe>& Meal::getRecipe() const { return recipe_; }
const std::vector<std::shared_ptr<Ingredient>>& Meal::getIngredients() const { return ingredients_; }
double Meal::getEstimatedCost() const { return estimatedCost_; }
int Meal::getServings() const { return servings_; }

// Setters
void Meal::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Meal name cannot be empty");
    }
    name_ = name;
}

void Meal::setType(Type type) {
    type_ = type;
}

void Meal::setStatus(Status status) {
    status_ = status;
}

void Meal::setPlannedTime(const std::chrono::system_clock::time_point& time) {
    plannedTime_ = time;
}

void Meal::setRecipe(const std::shared_ptr<Recipe>& recipe) {
    recipe_ = recipe;
    if (recipe_) {
        // Clear existing ingredients and copy from recipe
        ingredients_.clear();
        for (const auto& ingredient : recipe_->getIngredients()) {
            ingredients_.push_back(std::make_shared<Ingredient>(*ingredient));
        }
        scaleServings(servings_);
        updateCost();
    }
}

void Meal::setServings(int servings) {
    if (servings <= 0) {
        throw std::invalid_argument("Number of servings must be positive");
    }
    servings_ = servings;
    scaleServings(servings);
}

// Operations
void Meal::addIngredient(const std::shared_ptr<Ingredient>& ingredient) {
    if (!ingredient) {
        throw std::invalid_argument("Cannot add null ingredient");
    }
    ingredients_.push_back(ingredient);
    updateCost();
}

void Meal::removeIngredient(const std::string& ingredientId) {
    auto it = std::find_if(ingredients_.begin(), ingredients_.end(),
        [&ingredientId](const auto& ingredient) {
            return ingredient->getId() == ingredientId;
        });
    
    if (it != ingredients_.end()) {
        ingredients_.erase(it);
        updateCost();
    }
}

void Meal::scaleServings(int newServings) {
    if (newServings <= 0) {
        throw std::invalid_argument("Number of servings must be positive");
    }

    if (servings_ == newServings) {
        return;
    }

    double scaleFactor = static_cast<double>(newServings) / servings_;
    for (auto& ingredient : ingredients_) {
        ingredient->scale(scaleFactor);
    }
    
    servings_ = newServings;
    updateCost();
}

void Meal::updateCost() {
    estimatedCost_ = 0.0;
    for (const auto& ingredient : ingredients_) {
        estimatedCost_ += ingredient->calculateCost();
    }
}

bool Meal::isComplete() const {
    return !ingredients_.empty() && status_ != Status::PLANNED;
}

double Meal::calculateNutritionalValue() const {
    double totalCalories = 0.0;
    for (const auto& ingredient : ingredients_) {
        const auto& nutritionInfo = ingredient->getNutritionalInfo();
        auto caloriesIt = nutritionInfo.find("calories");
        if (caloriesIt != nutritionInfo.end()) {
            totalCalories += caloriesIt->second;
        }
    }
    return totalCalories;
}

std::string Meal::serialize() const {
    json j;
    j["name"] = name_;
    j["type"] = static_cast<int>(type_);
    j["status"] = static_cast<int>(status_);
    j["plannedTime"] = std::chrono::system_clock::to_time_t(plannedTime_);
    j["estimatedCost"] = estimatedCost_;
    j["servings"] = servings_;

    j["ingredients"] = json::array();
    for (const auto& ingredient : ingredients_) {
        j["ingredients"].push_back(ingredient->serialize());
    }

    if (recipe_) {
        j["recipe"] = recipe_->serialize();
    }

    return j.dump();
}

Meal Meal::deserialize(const std::string& data) {
    json j = json::parse(data);
    Meal meal(j["name"].get<std::string>());
    
    meal.setType(static_cast<Type>(j["type"].get<int>()));
    meal.setStatus(static_cast<Status>(j["status"].get<int>()));
    meal.setPlannedTime(std::chrono::system_clock::from_time_t(j["plannedTime"].get<time_t>()));
    meal.estimatedCost_ = j["estimatedCost"].get<double>();
    meal.servings_ = j["servings"].get<int>();

    for (const auto& ingredientJson : j["ingredients"]) {
        meal.addIngredient(std::make_shared<Ingredient>(
            Ingredient::deserialize(ingredientJson.dump())));
    }

    if (j.contains("recipe") && !j["recipe"].is_null()) {
        meal.setRecipe(std::make_shared<Recipe>(
            Recipe::deserialize(j["recipe"].dump())));
    }

    return meal;
}

} // namespace core
} // namespace smart_food
