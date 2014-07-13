#ifndef USER_C

#define USER_C

/**
 * Move user left
 * @param User *character
 * @return int
 */
int move_left(User *character)
{
    return (character->position_x) && (--character->position_x + 1);
}

/**
 * Move user right
 * @param User *character
 * @return int
 */
int move_right(User *character)
{
    return (character->position_x < __MAP_SIZE_X - 1) && ++character->position_x;
}

/**
 * Move user bottom
 * @param User *character
 * @return int
 */
int move_bottom(User *character)
{
    return (character->position_y < __MAP_SIZE_Y - 1) && ++character->position_y;
}

/**
 * Move user top
 * @param User *character
 * @return int
 */
int move_top(User *character)
{
    return (character->position_y) && (--character->position_y + 1);
}

/**
 * Update user HP
 * Can be signed values
 * @param User *character
 * @param const int value
 * @return void
 */
void update_HP(User *character, const int value)
{
    // GT max health => EQ max health
    if(character->health + value > character->max_health)
        character->health = character->max_health;
    // == 0 => Die, sorry man :(
    else if(character->health + value < 1)
        die();
    // Standart situation
    else
        character->health += value;
}

/**
 * Update user mana points
 * You can pass signed values
 * @param User *character
 * @param const int value
 * @return void
 */
void update_MP(User *character, const int value)
{
    // GT max mana
    if(character->mana + value > character->max_mana)
        character->mana = character->max_mana;
    // EQ 0
    else if(character->mana + value < 1)
        character->mana = 0;
    else
        character->mana += value;
}

/**
 * Update expirience
 * @param User *character
 * @param const unsigned int value
 * @return void
 */
void update_EXP(User *character, const unsigned int value)
{
    // Next level reached
    if(character->current_exp + value > character->Hero.attributes[character->level].exp_next) {
        // Set max HP and current HP to max from next level
        character->max_health = character->health = character->max_health - character->Hero.attributes[character->level].HP + character->Hero.attributes[++character->level].HP;
        // Set max MP and current MP to max from next level
        character->max_mana   = character->mana = character->max_mana - character->Hero.attributes[character->level - 1].MP + character->Hero.attributes[character->level].MP;
        character->regen_HP   = character->regen_HP - character->Hero.attributes[character->level - 1].regen_HP + character->Hero.attributes[character->level].regen_HP;
        character->regen_MP   = character->regen_MP - character->Hero.attributes[character->level - 1].regen_MP + character->Hero.attributes[character->level].regen_MP;
        character->strength   = character->strength - character->Hero.attributes[character->level - 1].strength + character->Hero.attributes[character->level].strength;
        character->dexterity  = character->dexterity - character->Hero.attributes[character->level - 1].dexterity + character->Hero.attributes[character->level].dexterity;
        character->intelligence = character->intelligence - character->Hero.attributes[character->level - 1].intelligence + character->Hero.attributes[character->level].intelligence;
        short int current = character->current_exp;
        character->current_exp = 0;
        // If there is some rest
        update_EXP(character, current + value - character->Hero.attributes[character->level - 1].exp_next);
    }
    else
        character->current_exp += value;
}

/**
 * Pick item
 * Check can item fit, update weight
 * @param User *character
 * @param unsigned char id
 * @return int
 */
int item_pick(User *character, unsigned char id)
{
    return (item_exists(*character, id--) && item_valid_weight(*character, character->bag[id].weight)) && (item_update_weight(character, character->bag[id].weight) && character->bag[id].number++ + 1);
}

/**
 * Drop item from bag
 * @param User *character
 * @param unsigned char id
 * @return int
 */
int item_drop(User *character, unsigned char id)
{
    return (item_exists(*character, id--) && character->bag[id].number > 0) && (item_update_weight(character, -character->bag[id].weight) && character->bag[id].number-- + 1);
}

/**
 * Sell item from bag
 * Equals to drop item + add coins
 * @param User *character
 * @param unsigned char id
 * @return int
 */
int item_sell(User *character, const unsigned char id)
{
    return item_drop(character, id) && ((character->coins += character->bag[id - 1].costs) + 1);
}

/**
 * Check item requirements
 * Check, can you put item on
 * @param const User character
 * @param unsigned char id
 * @return int
 */
int item_check_requirements(const User character, unsigned char id)
{
    // Item doesn't exists or level is too high
    if(!item_exists(character, id--) || character.level + 1 < character.bag[id].level)
        return 0;
    // Type is armor or weapon
    else if(character.bag[id].type == ARMOR || character.bag[id].type == WEAPON) {
        // Check stats
        if(character.strength >= character.bag[id].req_str && character.dexterity >= character.bag[id].req_dex && character.intelligence >= character.bag[id].req_int)
            // Now, check classes
            for(int i = 0; i < 4; ++i)
                if(character.Hero.id == character.bag[id].classes[i])
                    return 1;
        return 0;
    }
    // Type isn't armor or weapon, level check is done, return true
    return 1;
}

/**
 * Check is item exists (in bag-catalog)
 * @param const User character
 * @param const unsigned char id
 * @return int
 */
int item_exists(const User character, const unsigned char id)
{
    return (id <= __ITEMS_NUMBER && character.bag[id - 1].id);
}

/**
 * Count items with number > 0
 * @param const User character
 * @return int
 */
int item_names_number(const User character)
{
    unsigned char number = 0;
    // Count items
    for(int i = 0; i < __ITEMS_NUMBER; ++i)
        (character.bag[i].number && ++number);
    return number;
}

/**
 * Add stats from item to user object
 * @param User *character
 * @param unsigned char id (Item id)
 * @return void
 */
void item_add_stats(User *character, unsigned char id)
{
    // Item doesn't exists
    if(!item_exists(*character, id--))
        return;
    switch(character->bag[id].type) {
        /*
         * Add stats for armor
         */
        case ARMOR:
            character->strength += character->bag[id].strength;
            character->dexterity += character->bag[id].dexterity;
            character->intelligence += character->bag[id].intelligence;
            character->regen_HP += character->bag[id].regen_HP;
            character->regen_MP += character->bag[id].regen_MP;
            character->max_health += character->bag[id].HP;
            character->max_mana += character->bag[id].MP;
        break;
        /*
         * Add stats for weapon
         */
        case WEAPON:
            character->strength += character->bag[id].strength;
            character->dexterity += character->bag[id].dexterity;
            character->intelligence += character->bag[id].intelligence;
        break;
        /*
         * Potion (isn't using with this function) and bag. Add HP and MP
         */
        default:
            update_HP(character, character->bag[id].HP);
            update_MP(character, character->bag[id].MP);
    }
}

/**
 * Subtract stats from user object
 * @param User *character
 * @param unsigned char id (Item id)
 * @return void
 */
void item_remove_stats(User *character, unsigned char id)
{
    // Item doesn't exists
    if(!item_exists(*character, id--))
        return;
    /*
     * Remove stats
     */
    else if(character->bag[id].type == ARMOR) {
        character->regen_HP = (character->regen_HP - character->bag[id].regen_HP > 0) ? character->regen_HP - character->bag[id].regen_HP : 0;
        character->regen_MP = (character->regen_MP - character->bag[id].regen_MP > 0) ? character->regen_MP - character->bag[id].regen_MP : 0;
        character->max_health = (character->max_health - character->bag[id].HP > 0) ? character->max_health - character->bag[id].HP : 1;
        character->max_mana = (character->max_mana - character->bag[id].MP > 0) ? character->max_mana - character->bag[id].MP : 1;
        if(character->health > character->max_health) update_HP(character, character->health);
        if(character->mana > character->max_mana) update_MP(character, character->max_mana);
    }
    character->strength = ((character->strength - character->bag[id].strength) > 0) ? character->strength - character->bag[id].strength : 0;
    character->dexterity = (character->dexterity - character->bag[id].dexterity > 0) ? character->dexterity - character->bag[id].dexterity : 0;
    character->intelligence = (character->intelligence - character->bag[id].intelligence > 0) ? character->intelligence - character->bag[id].intelligence : 0;
}

/**
 * Put item off from inventory
 * @param User *character
 * @param enum armor_part part
 * @return int
 */
int item_put_off(User *character, enum armor_part part)
{
    // There is no item in this part
    if(!character->inventory[part].id)
        return 0;
    // Remove stats from user object
    item_remove_stats(character, character->inventory[part].id);
    character->inventory[part].id = 0;
    // Set flag to 0
    int is_changed = 0;

    /*
     * While there is changes do
     */
    do {
        is_changed = 0;

        // For each part check requirements, and if not enought stats then put item off and set flag to true
        for(int i = 0; i < 6; ++i)
            if(character->inventory[i].id && !item_check_requirements(*character, character->inventory[i].id)) {
                item_remove_stats(character, character->inventory[i].id);
                item_pick(character, character->inventory[i].id);
                character->inventory[i].id = 0;
                is_changed = 1;
            }
    } while(is_changed);
    return 1;
}

/**
 * Put item on in inventory
 * @param User *character
 * @param unsigned char id
 * @return int
 */
int item_put_on(User *character, unsigned char id)
{
    // Item doesn't exists or number < 1 or not enought stats
    if(!item_exists(*character, id--) || character->bag[id].number < 1 || !item_check_requirements(*character, id + 1))
        return 0;
    unsigned char tmp = character->inventory[character->bag[id].part].id;

    /*
     * Put bag on
     */
    if(character->bag[id].type == BAG) {
        // Can't put bag on. Capacity reasons
        if(character->bag[id].capacity < character->sum_weight - character->bag[id].weight + character->inventory[3].weight)
            return 0;
        character->bag[character->inventory[3].id - 1].number++;
        character->sum_weight += character->bag[character->inventory[3].id - 1].weight;
        item_drop(character, id + 1);
        character->inventory[3] = character->bag[id];
    }

    /*
     * Put (weapon || armor) on. If there is no item on this part
     */
    else if(!tmp || (item_put_off(character, character->bag[id].part) && item_check_requirements(*character, id + 1))) {
        // Drop item (subtract item number)
        item_drop(character, id + 1);
        // If there is item on this part, then pick item (add number)
        (tmp && item_pick(character, tmp));
        character->inventory[character->bag[id].part] = character->bag[id];
        // Add stats to user object
        item_add_stats(character, id + 1);
    }
    return 1;
}

/**
 * Check can you hold weight
 * @param User character
 * @param short int weight
 * @return int
 */
int item_valid_weight(const User character, short int weight)
{
    return (character.sum_weight + weight <= character.inventory[3].capacity);
}

/**
 * Change current items weight sum
 * Signed value
 * @param User character
 * @param short int weight
 * @return int
 */
int item_update_weight(User *character, short int weight)
{
    return item_valid_weight(*character, weight) && ((character->sum_weight = (character->sum_weight + weight > 0) ? character->sum_weight + weight : 0) + 1);
}

#endif