#include <gtest/gtest.h>
#include <smart_food/core/meal.hpp>
#include <chrono>

using namespace smart_food::core;

class MealTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
        testMeal = std::make_shared<Meal>("Test Meal", Meal::Type::LUNCH);
    }

    void TearDown() override {
        // Clean up test fixtures
    }

    std::shared_ptr<Meal> testMeal;
};

TEST_F(MealTest, DefaultConstructor) {
    Meal meal;
    EXPECT_FALSE(meal.getName().empty());
    EXPECT_EQ(meal.getType(), Meal::Type::BREAKFAST);
    EXPECT_EQ(meal.getStatus(), Meal::Status::PLANNED);
    EXPECT_EQ(meal.getServings(), 1);
    EXPECT_DOUBLE_EQ(meal.getEstimatedCost(), 0.0);
}

TEST_F(MealTest, ParameterizedConstructor) {
    EXPECT_EQ(testMeal->getName(), "Test Meal");
    EXPECT_EQ(testMeal->getType(), Meal::Type::LUNCH);
    EXPECT_EQ(testMeal->getStatus(), Meal::Status::PLANNED);
}

TEST_F(MealTest, SetAndGetName) {
    testMeal->setName("New Test Meal");
    EXPECT_EQ(testMeal->getName(), "New Test Meal");
}

TEST_F(MealTest, SetAndGetType) {
    testMeal->setType(Meal::Type::DINNER);
    EXPECT_EQ(testMeal->getType(), Meal::Type::DINNER);
}

TEST_F(MealTest, SetAndGetStatus) {
    testMeal->setStatus(Meal::Status::PREPARING);
    EXPECT_EQ(testMeal->getStatus(), Meal::Status::PREPARING);
}

TEST_F(MealTest, SetAndGetPlannedTime) {
    auto now = std::chrono::system_clock::now();
    testMeal->setPlannedTime(now);
    EXPECT_EQ(testMeal->getPlannedTime(), now);
}

TEST_F(MealTest, AddAndRemoveIngredients) {
    auto ingredient = std::make_shared<Ingredient>("Test Ingredient", 100.0, Ingredient::Unit::GRAM);
    testMeal->addIngredient(ingredient);
    EXPECT_EQ(testMeal->getIngredients().size(), 1);
    
    testMeal->removeIngredient(ingredient->getId());
    EXPECT_TRUE(testMeal->getIngredients().empty());
}

TEST_F(MealTest, ScaleServings) {
    testMeal->setServings(2);
    EXPECT_EQ(testMeal->getServings(), 2);
    
    testMeal->scaleServings(4);
    EXPECT_EQ(testMeal->getServings(), 4);
}

TEST_F(MealTest, EstimatedCost) {
    auto ingredient1 = std::make_shared<Ingredient>("Ingredient 1", 100.0, Ingredient::Unit::GRAM);
    ingredient1->setUnitPrice(0.1); // 0.1 per gram
    
    auto ingredient2 = std::make_shared<Ingredient>("Ingredient 2", 200.0, Ingredient::Unit::GRAM);
    ingredient2->setUnitPrice(0.2); // 0.2 per gram
    
    testMeal->addIngredient(ingredient1);
    testMeal->addIngredient(ingredient2);
    testMeal->updateCost();
    
    // Expected cost: (100g * 0.1) + (200g * 0.2) = 10 + 40 = 50
    EXPECT_DOUBLE_EQ(testMeal->getEstimatedCost(), 50.0);
}

TEST_F(MealTest, Serialization) {
    testMeal->setName("Serialization Test");
    testMeal->setType(Meal::Type::DINNER);
    testMeal->setStatus(Meal::Status::PREPARING);
    
    auto ingredient = std::make_shared<Ingredient>("Test Ingredient", 100.0, Ingredient::Unit::GRAM);
    ingredient->setUnitPrice(0.1);
    testMeal->addIngredient(ingredient);
    
    std::string serialized = testMeal->serialize();
    Meal deserialized = Meal::deserialize(serialized);
    
    EXPECT_EQ(deserialized.getName(), "Serialization Test");
    EXPECT_EQ(deserialized.getType(), Meal::Type::DINNER);
    EXPECT_EQ(deserialized.getStatus(), Meal::Status::PREPARING);
    EXPECT_EQ(deserialized.getIngredients().size(), 1);
    EXPECT_EQ(deserialized.getIngredients()[0]->getName(), "Test Ingredient");
    EXPECT_DOUBLE_EQ(deserialized.getIngredients()[0]->getQuantity(), 100.0);
    EXPECT_EQ(deserialized.getIngredients()[0]->getUnit(), Ingredient::Unit::GRAM);
}

TEST_F(MealTest, InvalidOperations) {
    // Test setting invalid name
    EXPECT_THROW(testMeal->setName(""), std::invalid_argument);
    
    // Test invalid servings
    EXPECT_THROW(testMeal->setServings(0), std::invalid_argument);
    EXPECT_THROW(testMeal->setServings(-1), std::invalid_argument);
    
    // Test invalid scaling
    EXPECT_THROW(testMeal->scaleServings(0), std::invalid_argument);
    EXPECT_THROW(testMeal->scaleServings(-2), std::invalid_argument);
    
    // Test null ingredient
    EXPECT_THROW(testMeal->addIngredient(nullptr), std::invalid_argument);
}

TEST_F(MealTest, RecipeIntegration) {
    auto recipe = std::make_shared<Recipe>("Test Recipe");
    auto ingredient1 = std::make_shared<Ingredient>("Ingredient 1", 100.0, Ingredient::Unit::GRAM);
    auto ingredient2 = std::make_shared<Ingredient>("Ingredient 2", 200.0, Ingredient::Unit::GRAM);
    
    recipe->addIngredient(ingredient1);
    recipe->addIngredient(ingredient2);
    
    testMeal->setRecipe(recipe);
    EXPECT_EQ(testMeal->getRecipe(), recipe);
    EXPECT_EQ(testMeal->getIngredients().size(), 2);
}

TEST_F(MealTest, NutritionalCalculation) {
    auto ingredient1 = std::make_shared<Ingredient>("Ingredient 1", 100.0, Ingredient::Unit::GRAM);
    ingredient1->addNutritionalInfo("calories", 200.0);
    
    auto ingredient2 = std::make_shared<Ingredient>("Ingredient 2", 200.0, Ingredient::Unit::GRAM);
    ingredient2->addNutritionalInfo("calories", 300.0);
    
    testMeal->addIngredient(ingredient1);
    testMeal->addIngredient(ingredient2);
    
    EXPECT_DOUBLE_EQ(testMeal->calculateNutritionalValue(), 500.0);
}

TEST_F(MealTest, CompletionStatus) {
    EXPECT_FALSE(testMeal->isComplete());
    
    auto ingredient = std::make_shared<Ingredient>("Test Ingredient", 100.0, Ingredient::Unit::GRAM);
    testMeal->addIngredient(ingredient);
    testMeal->setStatus(Meal::Status::PREPARING);
    
    EXPECT_TRUE(testMeal->isComplete());
}