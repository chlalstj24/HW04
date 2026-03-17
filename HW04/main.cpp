#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
    void addBasicRecipes() {
        recipes.push_back(Potion("회복 물약", { "약초", "물" }, 50));
        recipes.push_back(Potion("독약", { "독버섯", "물" }, 80));
        recipes.push_back(Potion("에너지 드링크", { "약초", "꿀" }, 120));
    }

    Potion* findPotion(const string& target) {
        for (auto& p : recipes) {
            if (p.name == target) return &p;
            for (const auto& ing : p.ingredients) {
                if (ing == target) return &p;
            }
        }
        return nullptr;
    }

    void showMenuList() {
        for (int i = 0; i < (int)recipes.size(); ++i) {
            cout << i << ". " << recipes[i].name << " (";
            for (auto& s : recipes[i].ingredients) cout << s << " ";
            cout << ") - 가격: " << recipes[i].price << "G" << endl;
        }
    }
};

class StockManager : public RecipeManager {
private:
    int gold = 100;
    int turn = 1;
    vector<pair<string, int>> inventory;

public:
    StockManager() {
        inventory = { {"약초", 6}, {"물", 6}, {"독버섯", 3}, {"꿀", 3} };
    }

    void gather() {
        int r1 = rand() % inventory.size();
        int r2 = rand() % inventory.size();
        inventory[r1].second++;
        inventory[r2].second++;
        cout << "> 채집 완료: " << inventory[r1].first << ", " << inventory[r2].first << " 획득!" << endl;
        turn++;
    }

    void craft(int idx) {
        if (idx < 0 || idx >= (int)recipes.size()) {
            cout << "> 잘못된 레시피 번호입니다." << endl;
            return;
        }

        Potion& p = recipes[idx];
        if (p.stock >= p.MAX_STOCK) {
            cout << "> [실패] 창고가 가득 찼습니다." << endl;
            return;
        }

        for (auto& req : p.ingredients) {
            bool hasIng = false;
            for (auto& inv : inventory) {
                if (inv.first == req && inv.second > 0) { hasIng = true; break; }
            }
            if (!hasIng) { cout << "> [실패] 재료가 부족합니다: " << req << endl; return; }
        }

        for (auto& req : p.ingredients) {
            for (auto& inv : inventory) { if (inv.first == req) inv.second--; }
        }
        p.stock++;
        cout << "> " << p.name << " 제조 성공!" << endl;
        turn++;
    }

    void sellAll() {
        int totalEarned = 0;
        for (auto& p : recipes) {
            totalEarned += (p.stock * p.price);
            p.stock = 0;
        }
        if (totalEarned > 0) {
            gold += totalEarned;
            cout << "> 전량 판매 완료! " << totalEarned << "G를 벌었습니다." << endl;
            turn++;
        }
        else {
            cout << "> 판매할 물약이 없습니다." << endl;
        }
    }

    void processReturn(const string& input) {
        Potion* p = findPotion(input);
        if (!p) {
            cout << "> 검색 결과가 없습니다." << endl;
            return;
        }

        if (p->stock < p->MAX_STOCK) {
            p->stock++;
            cout << "> " << p->name << " 반환 처리되었습니다." << endl;
            turn++;
        }
        else {
            cout << "> 창고가 가득 차서 더 받을 수 없습니다." << endl;
        }
    }

    void printStatus() {
        cout << "\n[" << turn << "턴] 현재 잔고: " << gold << "/1000G" << endl;
        cout << "- 재료: ";
        for (auto& i : inventory) cout << i.first << "(" << i.second << ") ";
        cout << "\n- 창고: ";
        for (auto& p : recipes) cout << p.name << "[" << p.stock << "/3] ";
        cout << "\n------------------------------------------" << endl;
    }

    bool checkWin() { return gold >= 1000; }
    int getTurn() { return turn; }
};

int main() {
    srand((unsigned int)time(NULL));
    StockManager gm;
    gm.addBasicRecipes();

    while (!gm.checkWin()) {
        gm.printStatus();
        cout << "1.채집 2.제조 3.판매 4.반환 5.종료\n선택: ";
        int sel; cin >> sel;

        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "> 숫자로 입력해주세요." << endl;
            continue;
        }

        if (sel == 1) gm.gather();
        else if (sel == 2) {
            gm.showMenuList();
            cout << "번호 선택: "; int idx; cin >> idx;
            gm.craft(idx);
        }
        else if (sel == 3) gm.sellAll();
        else if (sel == 4) {
            cout << "물약 이름 또는 재료 입력: ";
            string target; cin.ignore(); getline(cin, target);
            gm.processReturn(target);
        }
        else if (sel == 5) break;
    }

    if (gm.checkWin()) {
        cout << "\n 목표 달성! 총 소요 턴: " << gm.getTurn() << endl;
    }

    return 0;
}   