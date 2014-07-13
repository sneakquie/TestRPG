#ifndef USER_H

#define USER_H

/*
 * Hero attributes
 */
typedef struct attribute {
    unsigned short int strength
                     , dexterity
                     , intelligence
                     , HP
                     , MP
                     , regen_HP
                     , regen_MP
                     , exp_next;
} Attribute;

/*
 * Array with different levels
 */
typedef struct hero_class {
    unsigned char id;
    const char *name;

    Attribute attributes[__USR_LEVELS];
} HeroClass;

// Types of item
enum item_type {ARMOR, POTION, BAG, WEAPON};
// Body parts
enum armor_part {HEAD = 0, BREASTPLATE = 1, PAULDRON = 2, BELT = 3, GREAVES = 4, HANDS = 5};

/*
 * Item struct
 */
typedef struct item {
    const char *name;
    unsigned char id
                , level
                , number
                , classes[4];
    enum armor_part part;
    enum item_type type;
    unsigned short int weight
                     , capacity
                     , damage
                     , strength
                     , dexterity
                     , intelligence
                     , regen_HP
                     , regen_MP
                     , HP
                     , MP
                     , costs
                     , req_str
                     , req_dex
                     , req_int;
} Item;

/*
 * User struct
 */
typedef struct user {
    unsigned short int position_x
                     , position_y
                     , max_health
                     , max_mana
                     , health
                     , mana
                     , regen_HP
                     , regen_MP
                     , strength
                     , dexterity
                     , intelligence
                     , level
                     , current_exp
                     , coins
                     , sum_weight;
    HeroClass Hero;
    Item inventory[6];
    Item *bag;
} User;

int move_left(User *);
int move_right(User *);
int move_bottom(User *);
int move_top(User *);
void update_HP(User *, const int);
void update_MP(User *, const int);
void update_EXP(User *, const unsigned int);
int item_pick(User *, unsigned char);
int item_drop(User *, unsigned char);
int item_sell(User *, const unsigned char);
int item_exists(const User, const unsigned char);
int item_check_requirements(const User, unsigned char);
int item_exists(const User, const unsigned char);
int item_names_number(const User);
void item_add_stats(User *, unsigned char);
void item_remove_stats(User *, unsigned char);
int item_valid_weight(const User, short int);
int item_put_off(User *, enum armor_part);
int item_put_on(User *, unsigned char);
int item_update_weight(User *, short int);

#endif