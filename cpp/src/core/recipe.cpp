#include "smart_food/core/recipe.hpp" 
#include <stdexcept>
#include <sstream>
#include <random>
#include <nlohmann/json.hpp>

using nlohmann::json;
namespace smart_food {
namespace core {

/**
 * Default constructor. Creates a new recipe with default values.
 */
Recipe::Recipe()
    : name_("New Recipe")
    , description_("")
    , difficulty_(Difficulty::EASY)
    , servings_(1) {
    generateId();
}

/**
 * Constructor with name.
 * @param name The name of the recipe
 */
Recipe::Recipe(const std::string& name)
    : name_(name)
    , description_("")
    , difficulty_(Difficulty::EASY)
    , servings_(1) {
    if (name.empty()) {
        throw std::invalid_argument("Recipe name cannot be empty");
    }
    generateId();
}

/**
 * Constructor with name and description.
 * @param name The name of the recipe
 * @param description The description of the recipe
 */
Recipe::Recipe(const std::string& name, const std::string& description)
    : name_(name)
    , description_(description)
    , difficulty_(Difficulty::EASY)
    , servings_(1) {
    if (name.empty()) {
        throw std::invalid_argument("Recipe name cannot be empty");
    }
    generateId();
}

// Getters

/**
 * @return The unique identifier of the recipe
 */
const std::string& Recipe::getId() const {
    return id_;
}

/**
 * @return The name of the recipe
 */
const std::string& Recipe::getName() const {
    return name_;
}

/**
 * @return The description of the recipe
 */
const std::string& Recipe::getDescription() const {
    return description_;
}


/**
 * @return The difficulty level of the recipe
 */
Recipe::Difficulty Recipe::getDifficulty() const {
    return difficulty_;
}

/**
 * @return The number of servings this recipe makes
 */
int Recipe::getServings() const {
    return servings_;   ///< Number of servings the recipe makes
}

// Remaining getter implementations
std::chrono::minutes Recipe::getTotalTime() const {
    std::chrono::minutes total{0};
    for (const auto& step : steps_) {
        total += step.duration;
    }
    return total;
}

const std::vector<std::shared_ptr<Ingredient>>& Recipe::getIngredients() const {
    return ingredients_;
}

const std::vector<Recipe::Step>& Recipe::getSteps() const {
    return steps_;
}

const std::map<std::string, double>& Recipe::getNutritionalInfo() const {
    return nutritionalInfo_;
}

// Setter implementations
void Recipe::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Recipe name cannot be empty");
    }
    name_ = name;
}

void Recipe::setDescription(const std::string& description) {
    description_ = description;
}

void Recipe::setDifficulty(Difficulty difficulty) {
    difficulty_ = difficulty;
}

void Recipe::setServings(int servings) {
    if (servings <= 0) {
        throw std::invalid_argument("Number of servings must be positive");
    }
    servings_ = servings;
}

// Operation implementations
void Recipe::addIngredient(const std::shared_ptr<Ingredient>& ingredient) {
    if (!ingredient) {
        throw std::invalid_argument("Cannot add null ingredient");
    }
    ingredients_.push_back(ingredient);
    recalculateNutritionalInfo();
}

void Recipe::removeIngredient(const std::string& ingredientId) {
    auto it = std::find_if(ingredients_.begin(), ingredients_.end(),
        [&ingredientId](const auto& ingredient) {
            return ingredient->getId() == ingredientId;
        });
    
    if (it != ingredients_.end()) {
        ingredients_.erase(it);
        recalculateNutritionalInfo();
    }
}

void Recipe::addStep(const Step& step) {
    if (step.order <= 0) {
        throw std::invalid_argument("Step order must be positive");
    }
    
    // Check for duplicate order
    auto it = std::find_if(steps_.begin(), steps_.end(),
        [&step](const auto& existing) {
            return existing.order == step.order;
        });
    
    if (it != steps_.end()) {
        // If duplicate order, shift existing steps
        for (auto& existing : steps_) {
            if (existing.order >= step.order) {
                existing.order++;
            }
        }
    }
    
    steps_.push_back(step);
    
    // Sort steps by order
    std::sort(steps_.begin(), steps_.end(),
        [](const auto& a, const auto& b) {
            return a.order < b.order;
        });
}

void Recipe::removeStep(int order) {
    auto it = std::find_if(steps_.begin(), steps_.end(),
        [order](const auto& step) {
            return step.order == order;
        });
    
    if (it != steps_.end()) {
        steps_.erase(it);
        
        // Reorder remaining steps
        int newOrder = 1;
        for (auto& step : steps_) {
            step.order = newOrder++;
        }
    }
}

void Recipe::reorderStep(int oldOrder, int newOrder) {
    if (oldOrder <= 0 || newOrder <= 0) {
        throw std::invalid_argument("Step orders must be positive");
    }
    
    auto it = std::find_if(steps_.begin(), steps_.end(),
        [oldOrder](const auto& step) {
            return step.order == oldOrder;
        });
    
    if (it == steps_.end()) {
        throw std::invalid_argument("Step with old order not found");
    }
    
    Step step = *it;
    steps_.erase(it);
    step.order = newOrder;
    
    // Adjust orders of other steps
    for (auto& existing : steps_) {
        if (oldOrder < newOrder) {
            if (existing.order > oldOrder && existing.order <= newOrder) {
                existing.order--;
            }
        } else {
            if (existing.order >= newOrder && existing.order < oldOrder) {
                existing.order++;
            }
        }
    }
    
    steps_.push_back(step);
    
    // Sort steps by order
    std::sort(steps_.begin(), steps_.end(),
        [](const auto& a, const auto& b) {
            return a.order < b.order;
        });
}

double Recipe::calculateTotalCost() const {
    double totalCost = 0.0;
    for (const auto& ingredient : ingredients_) {
        totalCost += ingredient->calculateCost();
    }
    return totalCost;
}

void Recipe::updateNutritionalInfo() {
    recalculateNutritionalInfo();
}

bool Recipe::isValid() const {
    return !name_.empty() && 
           servings_ > 0 && 
           !ingredients_.empty() && 
           !steps_.empty();
}

std::string Recipe::serialize() const {
    json j;
    j["id"] = id_;
    j["name"] = name_;
    j["description"] = description_;
    j["difficulty"] = static_cast<int>(difficulty_);
    j["servings"] = servings_;
    
    j["ingredients"] = json::array();
    for (const auto& ingredient : ingredients_) {
        j["ingredients"].push_back(ingredient->serialize());
    }
    
    j["steps"] = json::array();
    for (const auto& step : steps_) {
        json stepJson;
        stepJson["order"] = step.order;
        stepJson["description"] = step.description;
        stepJson["duration"] = step.duration.count();
        j["steps"].push_back(stepJson);
    }
    
    j["nutritionalInfo"] = nutritionalInfo_;
    
    return j.dump();
}

Recipe Recipe::deserialize(const std::string& data) {
    json j = json::parse(data);
    
    Recipe recipe(j["name"].get<std::string>(), j["description"].get<std::string>());
    recipe.id_ = j["id"].get<std::string>();
    recipe.setDifficulty(static_cast<Difficulty>(j["difficulty"].get<int>()));
    recipe.setServings(j["servings"].get<int>());
    
    for (const auto& ingredientJson : j["ingredients"]) {
        recipe.addIngredient(std::make_shared<Ingredient>(
            Ingredient::deserialize(ingredientJson.dump())));
    }
    
    for (const auto& stepJson : j["steps"]) {
        Step step;
        step.order = stepJson["order"].get<int>();
        step.description = stepJson["description"].get<std::string>();
        step.duration = std::chrono::minutes(stepJson["duration"].get<int>());
        recipe.addStep(step);
    }
    
    recipe.nutritionalInfo_ = j["nutritionalInfo"].get<std::map<std::string, double>>();
    
    return recipe;
}

void Recipe::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    ss << "rec_";
    for (int i = 0; i < 8; i++) {
        ss << std::hex << dis(gen);
    }
    id_ = ss.str();
}

void Recipe::recalculateNutritionalInfo() {
    nutritionalInfo_.clear();
    
    for (const auto& ingredient : ingredients_) {
        const auto& ingredientInfo = ingredient->getNutritionalInfo();
        for (const auto& [nutrient, value] : ingredientInfo) {
            nutritionalInfo_[nutrient] += value;
        }
    }
}
} // namespace core
} // namespace smart_food