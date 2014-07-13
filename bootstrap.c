#ifndef BOOTSTRAP_C

#define BOOTSTRAP_C

/**
 * Create startup select menu
 * @param void
 * @return unsigned char (0 - New game choosen, 1 - Load game)
 */
unsigned char bootstrap_menu(void)
{
    // Menu variants
    const char *menu[3] = {"     Start game     ", "     Load game      ", "       Exit         "};
    char current = 0
       , answer;

    do {
        // Show menu with active element
        display_bootstrap_menu(menu, current);

        // Waiting for keyup
        while(!kbhit());

        // Letter W
        if((answer = getchar()) == 119)
            current = (current < 1) ? 2 : current - 1;
        // Letter S
        else if(answer == 115)
            current = (current > 1) ? 0 : current + 1;
        // Enter
        else if(answer == 10) {
            clr_scr();

            if(current > 1 || current < 0)
                exit(-1);
            return current;
        }
    } while(1);
    return 0;
}

/**
 * Create select menu with classes to choose
 * @param void
 * @return User
 */
User bootstrap_new_game(void)
{
    const char *menu[5] = {"    Warrior (STR)   ", "     Rogue (DEX)    ", "   Mage (INT, Dmg)  ", " Priest (INT, Heal) ", "       Exit         "};
    char current = 0
       , answer;
    User character = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, heroes[0], {}, items};

    do {
        // Show menu with active elment
        display_bootstrap_classes(menu, current);

        // Wait for keyup
        while(!kbhit());

        // Key W
        if((answer = getchar()) == 119)
            current = (current < 1) ? 4 : current - 1;
        // Key S
        else if(answer == 115)
            current = (current > 3) ? 0 : current + 1;
        // Enter
        else if(answer == 10) {
            clr_scr();

            // Exit
            if(current == 4)
                exit(-1);

            // If current has wrong class ID
            current = current % 4;

            // Initialize character var and properties
            character.Hero         = heroes[current];
            character.position_x   = __MAP_SIZE_X / 2;
            character.position_y   = __MAP_SIZE_Y / 2;
            character.max_health   = character.health = character.Hero.attributes[0].HP;
            character.max_mana     = character.mana = character.Hero.attributes[0].MP;
            character.regen_HP     = character.Hero.attributes[0].regen_HP;
            character.regen_MP     = character.Hero.attributes[0].regen_MP;
            character.strength     = character.Hero.attributes[0].strength;
            character.intelligence = character.Hero.attributes[0].intelligence;
            character.dexterity    = character.Hero.attributes[0].dexterity;
            character.level        = character.current_exp = character.sum_weight = 0;
            character.bag          = items;
            character.coins        = __START_COINS;
            character.inventory[3] = (Item) character.bag[28];
            break;
        }
    } while(1);

    return character;
}

/**
 * @param void
 * @return User
 */
User bootstrap_load_game(void)
{
    return bootstrap_new_game();
}

/**
 * Ask user to fight with NPC
 * @param const unsigned int current_time
 * @param unsigned int move_history[][]
 * @param User *character
 * @return void
 */
void bootstrap_fight(const unsigned int current_time, unsigned int (*move_history)[__MAP_SIZE_X][__MAP_SIZE_Y], User *character)
{
    display_user_map(*character);

    // Random NPC
    NPC opponent = NPC_list[random(0, __NPC_NUMBER - 1)];
    // Random level with range based on User level
    opponent.level = random(((character->level - __NPC_LEVEL_RANGE + 1) < 1 ? 1 : character->level - __NPC_LEVEL_RANGE + 1), character->level + __NPC_LEVEL_RANGE + 1);

    // Display message
    char msg[100];
    sprintf(msg, "Do you want to fight with %s (%i lvl)? (y/n)", opponent.name, opponent.level);
    display_msg(msg);

    // User want to fight with BOT
    if(wait_answer()) {
        display_user_map(*character);
        display_msg("Fight!");
        // Wait for 2 seconds
        delay(2);
        char *msg_p = NPC_fight(character, opponent);
        display_user_map(*character);
        display_msg(msg_p);
        // Update move history
        (*move_history)[character->position_x][character->position_y] = current_time + random(__MIN_TIME_REMAIN, __MAX_TIME_REMAIN);
        while(!kbhit());
    }
}

/**
 * Run bag module, display bag, waiting commands
 * @param User *character Pointer to User
 * @return void
 */
void bootstrap_bag(User *character)
{
    unsigned char offset = 0
                , items_number = 0;
    do {
        // Count items number
        items_number = item_names_number(*character);
        display_user_bag(*character, offset, items_number);

        // Wait for typing some key
        while(!kbhit());
        const char answer = getchar();

        // "W" typed, offset to top
        if(answer == 119)
            ((offset > 0) && --offset);
        // "S" typed, offset to bottom
        else if(answer == 115)
            ((offset + __MAP_SIZE_Y < items_number) && ++offset);
        // ":" typed
        else if(answer == 58) {
            display_user_bag(*character, offset, items_number);
            cout << ':';

            char cmd[32]
               , cmd_p[100];
            unsigned int item_id;
            // Waiting for a comand
            gets(cmd);

            // Command is valid
            if(sscanf(cmd, "%s %i", cmd_p, &item_id) > 1) {
                char msg[100];

                // Item doesn't exists
                if(!item_exists(*character, item_id--))
                    continue;
                // Show info about item
                else if(!strcmp(cmd_p, "info")) {
                    display_item_info(*character, item_id + 1);
                    while(!kbhit());
                }
                // Sell item
                else if(!strcmp(cmd_p, "sell")) {
                    // User have the item
                    if(character->bag[item_id].number > 0) {
                        // Generate message
                        sprintf(msg, "Do you really want to sell \"%s\" (%i coins) ?", character->bag[item_id].name, character->bag[item_id].costs);
                        display_user_bag(*character, offset, items_number);
                        display_msg(msg);
                        // Waiting answer about selling. If user pressed Y and item successfully sold
                        if(wait_answer() && item_sell(character, item_id + 1)) {
                            // Generate success message
                            sprintf(msg, "You sold \"%s\" for %i coins", character->bag[item_id].name, character->bag[item_id].costs);
                            display_user_bag(*character, offset, items_number);
                            display_msg(msg);
                            while(!kbhit());
                            continue;
                        }
                    }
                    // For some reasons you haven't sold the item
                    sprintf(msg, "You haven't sold \"%s\"", character->bag[item_id].name);
                    display_user_bag(*character, offset, items_number);
                    display_msg(msg);
                    while(!kbhit());
                }
                // Want to drop item
                else if(!strcmp(cmd_p, "drop")) {
                    // User have the item
                    if(character->bag[item_id].number > 0) {
                        // Generate message
                        sprintf(msg, "Do you really want to drop \"%s\" ?", character->bag[item_id].name);
                        display_user_bag(*character, offset, items_number);
                        display_msg(msg);
                        // Same as sell
                        if(wait_answer() && item_drop(character, item_id + 1)) {
                            // Success message
                            sprintf(msg, "You dropped \"%s\"", character->bag[item_id].name);
                            display_user_bag(*character, offset, items_number);
                            display_msg(msg);
                            while(!kbhit());
                            continue;
                        }
                    }
                    // You haven't dropped the item
                    sprintf(msg, "You haven't dropped \"%s\"", character->bag[item_id].name);
                    display_user_bag(*character, offset, items_number);
                    display_msg(msg);
                    while(!kbhit());
                }
                // Want to use item
                else if(!strcmp(cmd_p, "use")) {
                    if(character->bag[item_id].number > 0) {
                        if(item_check_requirements(*character, item_id + 1)) {
                            if(character->bag[item_id].type == POTION) {
                                update_HP(character, character->bag[item_id].HP);
                                update_MP(character, character->bag[item_id].MP);
                                item_drop(character, item_id + 1);
                            }
                            else {
                                item_put_on(character, item_id + 1);
                            }
                            sprintf(msg, "You used \"%s\"", character->bag[item_id].name);
                        }
                        else {
                            sprintf(msg, "You can't use \"%s\"", character->bag[item_id].name);
                        }
                        display_user_bag(*character, offset, items_number);
                        display_msg(msg);
                        while(!kbhit());
                        continue;
                    }
                    // You haven't used the item
                    sprintf(msg, "You haven't used \"%s\"", character->bag[item_id].name);
                    display_user_bag(*character, offset, items_number);
                    display_msg(msg);
                    while(!kbhit());
                }
            }
            // "return" typed, user want to return from bag-menu
            else if(!strcmp(cmd, "return"))
                break;
        }
    } while(1);
}

/**
 * Run shop module, display shop, waiting commands
 * @param User *character Pointer to User
 * @return void
 */
void bootstrap_shop(User *character)
{
    unsigned char offset = 0;
    do {
        display_shop(*character, offset);

        // Wait for typing some key
        while(!kbhit());
        const char answer = getchar();

        // "W" typed, offset to top
        if(answer == 119)
            ((offset > 0) && --offset);
        // "S" typed, offset to bottom
        else if(answer == 115)
            ((offset + __MAP_SIZE_Y < __ITEMS_NUMBER) && ++offset);
        // ":" typed
        else if(answer == 58) {
            display_shop(*character, offset);
            cout << ':';

            char cmd[32]
               , cmd_p[100];
            unsigned int item_id;
            // Waiting for a comand
            gets(cmd);

            // Command is valid
            if(sscanf(cmd, "%s %i", cmd_p, &item_id) > 1) {
                // Item doesn't exists
                if(!item_exists(*character, item_id--))
                    continue;
                // Show info about item
                else if(!strcmp(cmd_p, "info")) {
                    display_item_info(*character, item_id + 1);
                    while(!kbhit());
                }
                // Buy item
                else if(!strcmp(cmd_p, "buy")) {
                    char msg[100];
                    // User have the item
                    if(item_valid_weight(*character, character->bag[item_id].weight) && character->coins >= character->bag[item_id].costs << 1) {
                        // Generate message
                        sprintf(msg, "Do you really want to buy \"%s\" (%i coins) ?", character->bag[item_id].name, character->bag[item_id].costs << 1);
                        display_shop(*character, offset);
                        display_msg(msg);
                        // Waiting answer about selling. If user pressed Y and successfully bought item
                        if(wait_answer() && item_pick(character, item_id + 1)) {
                            // Generate success message
                            character->coins -= character->bag[item_id].costs << 1;
                            sprintf(msg, "You bought \"%s\" for %i coins", character->bag[item_id].name, character->bag[item_id].costs << 1);
                            display_shop(*character, offset);
                            display_msg(msg);
                            while(!kbhit());
                            continue;
                        }
                    }
                    // For some reasons you haven't bought the item
                    sprintf(msg, "You didn't bought \"%s\"", character->bag[item_id].name);
                    display_shop(*character, offset);
                    display_msg(msg);
                    while(!kbhit());
                }
            }
            // "return" typed, user want to return from menu
            else if(!strcmp(cmd, "return"))
                break;
        }
    } while(1);
}

/**
 * Run inventory module, display inventory
 * @param User *character
 * @return void
 */
void bootstrap_inventory(User *character)
{
    do {
        display_inventory(*character);

        // Wait for typing some key
        while(!kbhit());
        const char answer = getchar();

        // ":" typed
        if(answer == 58) {
            display_inventory(*character);
            cout << ':';

            char cmd[32]
               , cmd_p[100];
            unsigned int item_id;
            // Waiting for a comand
            gets(cmd);

            // Command is valid
            if(sscanf(cmd, "%s %i", cmd_p, &item_id) > 1) {
                if(!item_exists(*character, item_id--))
                    continue;
                // Show info about item
                else if(!strcmp(cmd_p, "info")) {
                    display_item_info(*character, item_id + 1);
                    while(!kbhit());
                }
                // Put item off
                else if(!strcmp(cmd_p, "poff")) {
                    char msg[100];
                    // User have the item
                    if(character->inventory[character->bag[item_id].part].id) {
                        // Generate message
                        sprintf(msg, "Do you really want to put \"%s\" off ?", character->bag[item_id].name);
                        display_inventory(*character);
                        display_msg(msg);
                        // Waiting answer about put off. If user pressed Y and item successfully put off
                        if(wait_answer() && item_put_off(character, character->bag[item_id].part)) {
                            item_pick(character, item_id + 1);
                            // Generate success message
                            sprintf(msg, "You successfully put \"%s\" off", character->bag[item_id].name);
                            display_inventory(*character);
                            display_msg(msg);
                            while(!kbhit());
                            continue;
                        }
                    }
                    sprintf(msg, "You can't put \"%s\" off", character->bag[item_id].name);
                    display_inventory(*character);
                    display_msg(msg);
                    while(!kbhit());
                }
            }
            // "return" typed, user want to return from menu
            else if(!strcmp(cmd, "return"))
                break;
        }
    } while(1);
}

/**
 * Return random number from MIN to MAX
 * @param const unsigned int min
 * @param const unsigned int max
 * @return unsigned int
 */
unsigned int random(const unsigned int min, const unsigned int max)
{
    return rand() % (max - min + 1) + min;
}

/**
 * Waiting answer from user (Yes or No)
 * You can also type "t" (tak)
 * @param void
 * @return unsigned char
 */
unsigned char wait_answer(void)
{
    char c;
    do {
        while(!kbhit());
        c = getchar();
    } while(!strchr("yYtTnN", c));
    return !(c == 'N' || c == 'n');
}

#endif