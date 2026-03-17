#pragma once
#ifndef POTION_GAME_H
#define POTION_GAME_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

class Potion {
public:
    string name;
    vector<string> ingredients;
    int price;
    int stock;
    const int MAX_STOCK = 3;

    Potion(string n, vector<string> ings, int p)
        : name(n), ingredients(ings), price(p), stock(0) {
    }
};

class RecipeManager {
protected:
    vector<Potion> recipes;

public:
    void addBasicRecipes();
    Potion* findPotion(const string& target);
    void showMenuList();
};

class StockManager : public RecipeManager {
private:
    int gold = 100;
    int turn = 1;
    vector<pair<string, int>> inventory;

public:
    StockManager();
    void gather();
    void craft(int idx);
    void sellAll();
    void processReturn(const string& input);
    void printStatus();
    bool checkWin();
    int getTurn();
};

#endif