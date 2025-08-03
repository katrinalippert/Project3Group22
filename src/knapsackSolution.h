//
// Created by aviga on 8/2/2025.
//

#ifndef KNAPSACKSOLUTION_H
#define KNAPSACKSOLUTION_H
#pragma once
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

struct FoodItem {
    int id;
    float calories;
    float weight;
};

struct Result {
    std::vector<FoodItem> selectedItems;
    float totalCalories = 0;
    float totalWeight = 0;
};

Result runKnapsackDP(const std::vector<FoodItem>& foodItems, float weightLimit){
    int n = foodItems.size();
    int W = static_cast<int>(weightLimit * 1000.0f);

    std::vector<std::vector<float>> dp(n + 1, std::vector<float>(W + 1, 0));
    std::vector<std::vector<bool>> keep(n + 1, std::vector<bool>(W + 1, false));
    //Create & fill up the table with solutions
    for (int i = 1; i <= n; ++i) {
        int wt = static_cast<int>(foodItems[i - 1].weight * 1000.0f);
        float cal = foodItems[i - 1].calories;
        for (int w = 0; w <= W; ++w) {
            if (wt <= w) {
                float included = dp[i - 1][w - wt] + cal;
                float excluded = dp[i - 1][w];
                if (included > excluded) {
                    dp[i][w] = included;
                    keep[i][w] = true;
                } else {
                    dp[i][w] = excluded;
                }
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    // Backtracking part
    Result result;
    int w = W;
    int totalWeightGrams = 0;
    for (int i = n; i > 0; --i) {
        if (keep[i][w]) {
            int itemWeight = static_cast<int>(foodItems[i - 1].weight * 1000.0f);
            if (totalWeightGrams + itemWeight <= W) {   // hard cap check
                result.selectedItems.push_back(foodItems[i - 1]);
                result.totalCalories += foodItems[i - 1].calories;
                totalWeightGrams += itemWeight;
                w -= itemWeight;
            }
        }
    }
    // Reverse sorting the results
    std::reverse(result.selectedItems.begin(), result.selectedItems.end());
    result.totalWeight = totalWeightGrams / 1000.0f;
    return result;
}
//greedy
void sortByRatio(std::vector<FoodItem>& foodItems) {
    std::sort(foodItems.begin(), foodItems.end(),
        [](const FoodItem& a, const FoodItem& b) {
            float ratioA = (a.weight > 0) ? (a.calories / a.weight) : 0.0f;
            float ratioB = (b.weight > 0) ? (b.calories / b.weight) : 0.0f;
            return ratioA > ratioB;
        });
}

Result runKnapsackGreedy(vector<FoodItem> foodItems, float weightLimit){
    sortByRatio(foodItems);

    Result result;
    for (size_t i = 0; i < foodItems.size(); ++i) {
        if (result.totalWeight + foodItems[i].weight <= weightLimit) {
            result.selectedItems.push_back(foodItems[i]);
            result.totalCalories += foodItems[i].calories;
            result.totalWeight += foodItems[i].weight;
        }
    }
    return result;
}

#endif //KNAPSACKSOLUTION_H