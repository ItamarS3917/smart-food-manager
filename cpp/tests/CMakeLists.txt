find_package(GTest REQUIRED)

set(TEST_SOURCES
    core/test_meal.cpp
    core/test_recipe.cpp
    core/test_ingredient.cpp
    core/test_storage.cpp
    test_main.cpp
)

add_executable(smart_food_tests ${TEST_SOURCES})
target_link_libraries(smart_food_tests
    PRIVATE
        smart_food
        GTest::GTest
        GTest::Main
)

include(GoogleTest)
gtest_discover_tests(smart_food_tests)
