#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <string>
using namespace std;
using Hp = int;
using Energy = int;

class Character;
class Item;
class Weapon;
class Shield;

void attack(Character& enemy, const shared_ptr<Item>&);
shared_ptr<Shield> convert_to_shield(const shared_ptr<Item>& item);
shared_ptr<Weapon> convert_to_weapon(const shared_ptr<Item>& item);

class Item {
public:
    enum Type { Weapon, Shield };

    const string name;
    const int energy_consumption;
    int mdurability;


    Item(string name, int energy_consumption, int mdurability)
        : name(std::move(name)), energy_consumption(energy_consumption), mdurability(mdurability) {}

    virtual ~Item() = default;  // Define the virtual destructor
    [[nodiscard]] int durability() const { return mdurability; }
    [[nodiscard]] virtual Type type() const = 0;
};

class Character {
    struct Stats {
        Hp health;
        Energy energy;
    } mstats;

    vector<shared_ptr<Item>> items;
    shared_ptr<Shield> active_shield = nullptr;
    string used_gadget = " ";


public:
    Character(string name, const int start_health, const int start_energy, vector<shared_ptr<Item> > items)
        : mstats(start_health, start_energy), items(std::move(items)), name(std::move(name)) {
    }

    const string name;

    void set_shield(const shared_ptr<Item>& shield) {
        active_shield = convert_to_shield(shield);
    }

    void remove_shield() {
        active_shield = nullptr;
    }

    shared_ptr<Shield> get_active_shield() {
        return active_shield;
    }

    void set_used_item(const string& item) {
        used_gadget = item;
    }

    string used_item() {
        return used_gadget;
    }

    void use_gadget(const shared_ptr<Item>& gadget, Character& enemy) {
        mstats.energy -= gadget->energy_consumption;
        gadget->mdurability--;
        switch (gadget->type()) {
            case Item::Weapon:
                attack(enemy, gadget);
                break;
            case Item::Shield:
                set_shield(gadget);
                break;
        }
    }

    vector<shared_ptr<Item>> inventory() {
        return items;
    }

    [[nodiscard]] Stats stats() const { return mstats; }

    void apply_damage(int damage){ mstats.health -= damage;}

    [[nodiscard]] std::optional<shared_ptr<Item> > get_gadget(const string &item_id) const {
        for (auto &item: items) {
            if (item->name == item_id && item->durability() != 0) {
                return std::make_optional(item);
            }
        }
        return nullptr;
    }
};

struct Weapon : Item {
    Weapon(string name, const int energy_consumption, const int durability, const int damage)
        : Item(std::move(name), energy_consumption, durability), damage(damage) {
    }

    const int damage;
    [[nodiscard]] Type type() const override { return Type::Weapon; }

};

struct Shield : Item {
    Shield(string name, int energy_consumption, int durability, int damage_reduction)
        : Item(std::move(name), energy_consumption, durability), damage_reduction(damage_reduction) {
    }

    const int damage_reduction;
    [[nodiscard]] Type type() const override { return Type::Shield; }
};

void read_gadget(string &gadget) {
    getline(cin, gadget);
}

void attack(Character &enemy, const shared_ptr<Item>& gadget) {
    auto weapon = convert_to_weapon(gadget);
    int damage = weapon->damage;
    if(enemy.used_item() == "Explosive Gel") {
        damage = damage * 8 / 10;
    }else if (enemy.get_active_shield() != nullptr && enemy.used_item() != "Batclaw") {
        damage = damage * ((100- enemy.get_active_shield()->damage_reduction) / 100);
    }
    enemy.apply_damage(damage);
    enemy.remove_shield();
}

shared_ptr<Shield> convert_to_shield(const shared_ptr<Item>& item) {
    return dynamic_pointer_cast<Shield>(item);
}

shared_ptr<Weapon> convert_to_weapon(const shared_ptr<Item>& item) {
    return dynamic_pointer_cast<Weapon>(item);
}

void print_active_shield(Character& character) {
    cout << "Active Shield: ";
    auto shield = character.get_active_shield();
    shield == nullptr ? cout << "None" : cout << shield->name;
    cout << endl;
}

void print_inventory(Character& character) {
    const vector<shared_ptr<Item>> inventory = character.inventory();
    for(auto& item : inventory) {
        if(item->mdurability != 0) {
            cout << item->name << ": \nEnergy consumption: " << item->energy_consumption;
            switch(item->type()) {
                case Item::Weapon:
                    cout << ", Damage: " << convert_to_weapon(item)->damage;
                break;
                case Item::Shield:
                    cout << ", Damage_reduction: " << convert_to_shield(item)->damage_reduction;
                break;
            }
            cout << ", durability: ";
            item->mdurability > 0 ? cout << item->mdurability : cout << "Infinity";
            cout << endl;
        }
    }
}

void print_used_gadget(const string& gadget,const Character& character) {
    cout << character.name << " used " << gadget << endl << endl;
}

void print_stats(const Character& character){
    cout << "Health: " << character.stats().health;
    cout << "\tEnergy: " << character.stats().energy << endl;
}

int main() {
    vector<shared_ptr<Item> > batman_gadgets;
    batman_gadgets.push_back(make_shared<Weapon>("Batarang", 50, -1, 11));
    batman_gadgets.push_back(make_shared<Weapon>("Grapple Gun", 88, 5, 18));
    batman_gadgets.push_back(make_shared<Weapon>("Explosive Gel", 92, 3, 10));
    batman_gadgets.push_back(make_shared<Weapon>("Batclaw", 120, 1, 20));
    batman_gadgets.push_back(make_shared<Shield>("Cape Glide", 20, -1, 40));
    batman_gadgets.push_back(make_shared<Shield>("Smoke Pellet", 50, 2, 90));

    vector<shared_ptr<Item> > joker_gadgets;
    joker_gadgets.push_back(make_shared<Weapon>("Joy Buzzer", 40, -1, 8));
    joker_gadgets.push_back(make_shared<Weapon>("Laughing Gas", 56, 8, 13));
    joker_gadgets.push_back(make_shared<Weapon>("Acid Flower", 100, 3, 22));
    joker_gadgets.push_back(make_shared<Shield>("Trick Shield", 15, -1, 32));
    joker_gadgets.push_back(make_shared<Shield>("Rubber Chicken", 40, 3, 80));

    vector chars = {
        Character("Batman", 100, 500, std::move(batman_gadgets)),
        Character("Joker", 100, 500, std::move(joker_gadgets))
    };

    optional<shared_ptr<Item> > opt;
    string gadget;
    size_t i = 0;

    while (true) {
        cout << endl;
        cout << "Turn: " << chars[i].name << endl;
        print_stats(chars[i]);
        print_active_shield(chars[i]);
        cout << endl << "Opponet stats: " << endl;
        print_stats(chars[(1 + i) % 2]);
        cout << endl;
        print_inventory(chars[i]);
        do {
            read_gadget(gadget);
            opt = chars[i].get_gadget(gadget);
        } while (opt == nullptr);
        const shared_ptr<Item> ptr = opt.value(); //convert form optional<shared_ptr<Item> to shared_ptr<Item>
        chars[i].use_gadget(ptr, chars[(1 + i) % 2]);
        print_used_gadget(gadget, chars[i]);
        chars[i].set_used_item(gadget);
        if(chars[i].stats().health < 0 || chars[i].stats().energy < 0) break;
        i = (1 + i) % 2; // next character
    }
    cout << chars[i].name << ":" << endl;
    print_stats(chars[i]);
    cout << chars[(1 + i) % 2].name << ":" << endl;
    print_stats(chars[(1 + i) % 2]);
    cout << "Winner: " << chars[(1 + i) % 2].name;


    return 0;
}





