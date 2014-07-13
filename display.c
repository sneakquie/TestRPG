#ifndef DISPLAY_C

#define DISPLAY_C

using namespace std;

/**
 * Display user map
 * @param const User character
 * @return void
 */
void display_user_map(const User character)
{
    clr_scr();

    display_top_bar(character);

    char map[__MAP_SIZE_Y * __MAP_SIZE_X * 6] = ""
       , result[__MAP_SIZE_Y * __MAP_SIZE_X * 6 + 2000] = ""
       , max = 1
       , *left_bar = sprint_left_bar(character)
       , *first_a[__MAP_SIZE_Y + 2] = {NULL}
       , *second_a[__MAP_SIZE_Y + 2] = {NULL};

    /*
     * Generating map
     */
    for(int i = 0; i < __MAP_SIZE_X; ++i)
        strcat(map, "═══");
    strcat(map, "═╗\n");
    for(int i = 0; i < character.position_y; ++i) {
        for(int j = 0; j < __MAP_SIZE_X; j++)
            strcat(map, "│__");
        strcat(map, "│║\n");
    }
    for(int i = 0; i < character.position_x; ++i)
        strcat(map, "│__");
    strcat(map, "│" __USR_BADGE);
    for(int i = character.position_x + 1; i < __MAP_SIZE_X; ++i)
        strcat(map, "│__");
    strcat(map, "│║\n");
    for(int i = character.position_y + 1; i < __MAP_SIZE_Y; ++i) {
        for(int j = 0; j < __MAP_SIZE_X; j++)
            strcat(map, "│__");
        strcat(map, "│║\n");
    }

    /*
     * Divide left bar and map to parts (divided by \n)
     */
    for(first_a[0] = strtok(left_bar, "\n"); first_a[max] = strtok(NULL, "\n"); ++max);
    for(max = 1, second_a[0] = strtok(map, "\n"); second_a[max] = strtok(NULL, "\n"); ++max);

    /*
     * Now, split divided parts
     */
    for(int i = 0; i < max && first_a[i] && second_a[i]; ++i) {
        strcat(result, first_a[i]);
        strcat(result, second_a[i]);
        strcat(result, "\n");
    }
    free(left_bar);

    // Display result
    cout << result
         << "══════════════════"
         << "╩═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝"
         << endl;
}

/**
 * Display user bag with offset
 * @param const User character
 * @param unsigned char offset
 * @param unsigned char items_number
 * @return void
 */
void display_user_bag(const User character, unsigned char offset, unsigned char items_number)
{
    clr_scr();

    display_top_bar(character);

    char inv[__MAP_SIZE_Y * __MAP_SIZE_X * 6] = "══════════════|info: <:info %id%>, sell: <:sell %id%>, drop: <:drop %id%>, use: <:use %id%>, exit: <:return>|══════════════\n"
       , result[__MAP_SIZE_Y * __MAP_SIZE_X * 6 + 2000] = ""
       , *left_bar = sprint_left_bar(character)
       , *first_a[__MAP_SIZE_Y + 2] = {NULL}
       , *second_a[__MAP_SIZE_Y + 2] = {NULL}
       , tmp[100];

    /*
     * Generate user bag, if number of items > 0
     */
    if(items_number > 0)
        for(int i = 0, j = 0; i < __ITEMS_NUMBER && j < __MAP_SIZE_Y + offset; ++i) {
            // Filter items with number = 0 and making offset
            if(character.bag[i].number < 1 || (j < offset && ++j))
                continue;

            // Item view
            sprintf(tmp, "[id:%ix%i], name: %s, level: %i, weight: %i, costs: %i, type: ", character.bag[i].id
                                                                                         , character.bag[i].number
                                                                                         , character.bag[i].name
                                                                                         , character.bag[i].level
                                                                                         , character.bag[i].weight
                                                                                         , character.bag[i].costs);
            strcat(inv, tmp);

            // Different types of items to show
            switch(character.bag[i].type) {
                case WEAPON:
                    strcat(inv, "WEAPON");
                break;
                case BAG:
                    strcat(inv, "BAG");
                break;
                case POTION:
                    strcat(inv, "POTION");
                break;
                default:
                    strcat(inv, "ARMOR");
            }
            strcat(inv, "\n");
            ++j;
        }

    // Number of items = 0
    else
        strcat(inv, "\tNo items to show");

    /*
     * Divide left bar and bag to parts (divided by \n)
     */
    int f = 1
      , s = 1;
    for(first_a[0] = strtok(left_bar, "\n"); first_a[f] = strtok(NULL, "\n"); ++f);
    for(second_a[0] = strtok(inv, "\n"); second_a[s] = strtok(NULL, "\n"); ++s);

    /*
     * Now, split divided parts
     */
    (f > s) && (s = f);
    for(int i = 0; i < s; ++i) {
        if(first_a[i] != NULL)
            strcat(result, first_a[i]);
        if(second_a[i] != NULL)
            strcat(result, second_a[i]);
        strcat(result, "\n");
    }
    free(left_bar);

    // Show generated output
    cout << result
         << "══════════════════"
         << "╩══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════"
         << endl;
}

/**
 * Display shop menu with offset
 * @param const User character
 * @param unsigned char offset
 * @return void
 */
void display_shop(const User character, unsigned char offset)
{
    clr_scr();

    display_top_bar(character);

    char inv[__MAP_SIZE_Y * __MAP_SIZE_X * 6] = "═════════════════════════════════|info: <:info %id%>, buy: <:buy %id%>, exit: <:return>|══════════════════════════════════\n"
       , result[__MAP_SIZE_Y * __MAP_SIZE_X * 6 + 2000] = ""
       , *left_bar = sprint_left_bar(character)
       , *first_a[__MAP_SIZE_Y + 2] = {NULL}
       , *second_a[__MAP_SIZE_Y + 2] = {NULL}
       , tmp[100];

    /*
     * For each item
     */
    for(int i = offset; i < __ITEMS_NUMBER && i < __MAP_SIZE_Y + offset; ++i) {
        // Item doesn't exists for some reasons
        if(!character.bag[i].id)
            continue;

        // Main view of the item
        sprintf(tmp, "[id:%i], name: %s, level: %i, weight: %i, costs: %i, type: ", character.bag[i].id
                                                                                  , character.bag[i].name
                                                                                  , character.bag[i].level
                                                                                  , character.bag[i].weight
                                                                                  , character.bag[i].costs * 2);
        strcat(inv, tmp);

        // Different types of items
        switch(character.bag[i].type) {
            case WEAPON:
                strcat(inv, "WEAPON");
            break;
            case BAG:
                strcat(inv, "BAG");
            break;
            case POTION:
                strcat(inv, "POTION");
            break;
            default:
                strcat(inv, "ARMOR");
        }
        strcat(inv, "\n");
    }

    /*
     * Divide left bar and shop-menu to parts (divided by \n)
     */
    int f = 1
      , s = 1;
    for(first_a[0] = strtok(left_bar, "\n"); first_a[f] = strtok(NULL, "\n"); ++f);
    for(second_a[0] = strtok(inv, "\n"); second_a[s] = strtok(NULL, "\n"); ++s);

    /*
     * Now, split divided parts
     */
    (f > s) && (s = f);
    for(int i = 0; i < s; ++i) {
        if(first_a[i] != NULL)
            strcat(result, first_a[i]);
        if(second_a[i] != NULL)
            strcat(result, second_a[i]);
        strcat(result, "\n");
    }
    free(left_bar);

    // Display output
    cout << result
         << "══════════════════"
         << "╩══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════"
         << endl;
}

/**
 * Display info about the item
 * @param const User character
 * @param unsigned char id Item id
 * @return void
 */
void display_item_info(const User character, unsigned char id)
{
    clr_scr();

    // Item doesn't exists
    if(!item_exists(character, id--))
        return;

    display_top_bar(character);

    char inv[__MAP_SIZE_Y * __MAP_SIZE_X * 6] = "════════════════════════════════════════════════════|exit: <:return>|═════════════════════════════════════════════════════\n"
       , result[__MAP_SIZE_Y * __MAP_SIZE_X * 6 + 2000] = ""
       , tmp_c[100] = ""
       , *left_bar = sprint_left_bar(character)
       , *first_a[__MAP_SIZE_Y + 2] = {NULL}
       , *second_a[__MAP_SIZE_Y + 2] = {NULL}
       , *tmp;

    /*
     * Display information, common for every type of items
     */
    strcat(inv, "\n\n\t\t\t\t\t╒═════════════════════════════════════════════════╕\n\t\t\t\t\t│                                                 │\n\t\t\t\t\t│ ID: ");
    tmp = fill_right_spaces(character.bag[id].id, 44);
    strcat(inv, tmp);
    free(tmp);
    strcat(inv, "│\n\t\t\t\t\t│ Name: ");
    tmp = fill_right_spaces(character.bag[id].name, 42);
    strcat(inv, tmp);
    free(tmp);
    strcat(inv, "│\n\t\t\t\t\t│ Level: ");
    tmp = fill_right_spaces(character.bag[id].level, 41);
    strcat(inv, tmp);
    free(tmp);
    strcat(inv, "│\n\t\t\t\t\t│ Weight: ");
    tmp = fill_right_spaces(character.bag[id].weight, 40);
    strcat(inv, tmp);
    free(tmp);
    strcat(inv, "│\n\t\t\t\t\t│ Costs: ");
    tmp = fill_right_spaces(character.bag[id].costs, 41);
    strcat(inv, tmp);
    free(tmp);

    /*
     * Now, display info for every type of the item
     */
    switch(character.bag[id].type) {
        /*
         * Armor type
         */
        case ARMOR:
            strcat(inv, "│\n\t\t\t\t\t│ Type: ARMOR                                     │\n\t\t\t\t\t│ Part: ");
            tmp = fill_right_spaces(parts[character.bag[id].part], 42);
            strcat(inv, tmp);
            free(tmp);

            // Classes
            strcat(inv, "│\n\t\t\t\t\t│ Classes: {");
            strcpy(tmp_c, heroes[character.bag[id].classes[0] - 1].name);
            for(int i = 1; i < 4; ++i) {
                if(character.bag[id].classes[i] < 1 || character.bag[id].classes[i] > 4)
                    break;
                strcat(tmp_c, ", ");
                strcat(tmp_c, heroes[character.bag[id].classes[i] - 1].name);
            }
            strcat(tmp_c, "}");
            tmp = fill_right_spaces(tmp_c, 38);
            strcat(inv, tmp);
            free(tmp);
            strcat(inv, "│\n");

            // Armor adds strength
            if(character.bag[id].strength > 0) {
                strcat(inv, "\t\t\t\t\t│ STR += ");
                sprintf(tmp_c, "%i", character.bag[id].strength);
                tmp = fill_right_spaces(tmp_c, 41);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Armor adds dexterity
            if(character.bag[id].dexterity > 0) {
                strcat(inv, "\t\t\t\t\t│ DEX += ");
                sprintf(tmp_c, "%i", character.bag[id].dexterity);
                tmp = fill_right_spaces(tmp_c, 41);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Armor adds intelligence
            if(character.bag[id].intelligence > 0) {
                strcat(inv, "\t\t\t\t\t│ INT += ");
                sprintf(tmp_c, "%i", character.bag[id].intelligence);
                tmp = fill_right_spaces(tmp_c, 41);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Armor adds HP regeneration
            if(character.bag[id].regen_HP > 0) {
                strcat(inv, "\t\t\t\t\t│ HP_R += ");
                sprintf(tmp_c, "%i", character.bag[id].regen_HP);
                tmp = fill_right_spaces(tmp_c, 40);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Armor adds MP regeneration
            if(character.bag[id].regen_MP > 0) {
                strcat(inv, "\t\t\t\t\t│ MP_R += ");
                sprintf(tmp_c, "%i", character.bag[id].regen_MP);
                tmp = fill_right_spaces(tmp_c, 40);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Armor adds max HP
            if(character.bag[id].HP > 0) {
                strcat(inv, "\t\t\t\t\t│ HP += ");
                sprintf(tmp_c, "%i", character.bag[id].HP);
                tmp = fill_right_spaces(tmp_c, 42);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Armor adds max MP
            if(character.bag[id].MP > 0) {
                strcat(inv, "\t\t\t\t\t│ MP += ");
                sprintf(tmp_c, "%i", character.bag[id].MP);
                tmp = fill_right_spaces(tmp_c, 42);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Item requires some strength
            if(character.bag[id].req_str > 0) {
                strcat(inv, "\t\t\t\t\t│ REQ_STR = ");
                sprintf(tmp_c, "%i", character.bag[id].req_str);
                tmp = fill_right_spaces(tmp_c, 38);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Item requires some dexterity
            if(character.bag[id].req_dex > 0) {
                strcat(inv, "\t\t\t\t\t│ REQ_DEX = ");
                sprintf(tmp_c, "%i", character.bag[id].req_dex);
                tmp = fill_right_spaces(tmp_c, 38);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Item requires some intelligence
            if(character.bag[id].req_int > 0) {
                strcat(inv, "\t\t\t\t\t│ REQ_INT = ");
                sprintf(tmp_c, "%i", character.bag[id].req_int);
                tmp = fill_right_spaces(tmp_c, 38);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
        break;
        /*
         * Type - weapon
         */
        case WEAPON:
            strcat(inv, "│\n\t\t\t\t\t│ Type: WEAPON                                    │\n\t\t\t\t\t│ Damage: ");
            // Damage range
            sprintf(tmp_c, "%i - %i", (character.bag[id].damage / 2) ? character.bag[id].damage / 2 : 1, character.bag[id].damage);
            tmp = fill_right_spaces(tmp_c, 40);
            strcat(inv, tmp);
            free(tmp);

            // Classes
            strcat(inv, "│\n\t\t\t\t\t│ Classes: {");
            strcpy(tmp_c, heroes[character.bag[id].classes[0] - 1].name);
            for(int i = 1; i < 4; ++i) {
                if(character.bag[id].classes[i] < 1 || character.bag[id].classes[i] > 4)
                    break;
                strcat(tmp_c, ", ");
                strcat(tmp_c, heroes[character.bag[id].classes[i] - 1].name);
            }
            strcat(tmp_c, "}");
            tmp = fill_right_spaces(tmp_c, 38);
            strcat(inv, tmp);
            free(tmp);
            strcat(inv, "│\n");

            // Weapon adds strangth
            if(character.bag[id].strength > 0) {
                strcat(inv, "\t\t\t\t\t│ STR += ");
                sprintf(tmp_c, "%i", character.bag[id].strength);
                tmp = fill_right_spaces(tmp_c, 41);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Weapon adds dexterity
            if(character.bag[id].dexterity > 0) {
                strcat(inv, "\t\t\t\t\t│ DEX += ");
                sprintf(tmp_c, "%i", character.bag[id].dexterity);
                tmp = fill_right_spaces(tmp_c, 41);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Weapon adds intelligence
            if(character.bag[id].intelligence > 0) {
                strcat(inv, "\t\t\t\t\t│ INT += ");
                sprintf(tmp_c, "%i", character.bag[id].intelligence);
                tmp = fill_right_spaces(tmp_c, 41);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Item requires some strength
            if(character.bag[id].req_str > 0) {
                strcat(inv, "\t\t\t\t\t│ REQ_STR = ");
                sprintf(tmp_c, "%i", character.bag[id].req_str);
                tmp = fill_right_spaces(tmp_c, 38);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Item requires some dexterity
            if(character.bag[id].req_dex > 0) {
                strcat(inv, "\t\t\t\t\t│ REQ_DEX = ");
                sprintf(tmp_c, "%i", character.bag[id].req_dex);
                tmp = fill_right_spaces(tmp_c, 38);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Item requires some intelligence
            if(character.bag[id].req_int > 0) {
                strcat(inv, "\t\t\t\t\t│ REQ_INT = ");
                sprintf(tmp_c, "%i", character.bag[id].req_int);
                tmp = fill_right_spaces(tmp_c, 38);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
        break;
        /*
         * Potion
         */
        case POTION:
            strcat(inv, "│\n\t\t\t\t\t│ Type: POTION                                    │\n");
            // Potion adds HP
            if(character.bag[id].HP > 0) {
                strcat(inv, "\t\t\t\t\t│ HP += ");
                sprintf(tmp_c, "%i", character.bag[id].HP);
                tmp = fill_right_spaces(tmp_c, 42);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
            // Potion adds MP
            if(character.bag[id].MP > 0) {
                strcat(inv, "\t\t\t\t\t│ MP += ");
                sprintf(tmp_c, "%i", character.bag[id].MP);
                tmp = fill_right_spaces(tmp_c, 42);
                strcat(inv, tmp);
                strcat(inv, "│\n");
                free(tmp);
            }
        break;
        /*
         * Default value is BAG
         */
        default:
            strcat(inv, "│\n\t\t\t\t\t│ Type: BAG                                       │\n\t\t\t\t\t│ Capacity: ");
            // Display weight in KG
            if(character.bag[id].capacity >= 1000)
                sprintf(tmp_c, "%.1fkg", (float) character.bag[id].capacity / 1000);
            // Display weight in g
            else
                sprintf(tmp_c, "%ig", character.bag[id].capacity);
            tmp = fill_right_spaces(tmp_c, 38);
            strcat(inv, tmp);
            strcat(inv, "│\n");
            free(tmp);
    }
    strcat(inv, "\t\t\t\t\t│                                                 │\n\t\t\t\t\t╘═════════════════════════════════════════════════╛");

    /*
     * Divide left bar and item info to parts (divided by \n)
     */
    int f = 1
      , s = 1;
    for(first_a[0] = strtok(left_bar, "\n"); first_a[f] = strtok(NULL, "\n"); ++f);
    for(second_a[0] = strtok(inv, "\n"); second_a[s] = strtok(NULL, "\n"); ++s);

    /*
     * Split divided parts
     */
    (f > s) && (s = f);
    for(int i = 0; i < s; ++i) {
        if(first_a[i] != NULL)
            strcat(result, first_a[i]);
        if(second_a[i] != NULL)
            strcat(result, second_a[i]);
        strcat(result, "\n");
    }
    free(left_bar);

    // Display item info
    cout << result
         << "══════════════════"
         << "╩══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════"
         << endl;
}

/**
 * Display bootstrap menu
 * @param const char *menu[3]
 * @param const unsigned char active
 * @return void
 */
void display_bootstrap_menu(const char *menu[3], const unsigned char active)
{
    clr_scr();

    cout << __BOOT_MENU_N
         << __BOOT_MENU_T << "╔════════════════════════════════════════════════╗" << endl
         << __BOOT_MENU_T << "║                                                ║" << endl
         << __BOOT_MENU_T << "║          ____  ____   ____    ___   _          ║" << endl
         << __BOOT_MENU_T << "║         |  _ \\|  _ \\ / ___|  / _ \\ / |         ║" << endl
         << __BOOT_MENU_T << "║         | |_) | |_) | |  _  | | | || |         ║" << endl
         << __BOOT_MENU_T << "║         |  _ <|  __/| |_| | | |_| || |         ║" << endl
         << __BOOT_MENU_T << "║         |_| \\_\\_|    \\____|  \\___(_)_|         ║" << endl
         << __BOOT_MENU_T << "║                                                ║" << endl
         << __BOOT_MENU_T << "║                                                ║" << endl;

    /*
     * Display menu variants
     */
    for (int i = 0; i < 3; ++i) {
        // Menu option is active
        if(active == i) {
            cout << __BOOT_MENU_T << "║             ┏━━━━━━━━━━━━━━━━━━━━┓             ║" << endl
                 << __BOOT_MENU_T << "║             ┃" << menu[i]       << "┃             ║" << endl
                 << __BOOT_MENU_T << "║             ┗━━━━━━━━━━━━━━━━━━━━┛             ║" << endl;
        }
        else {
            cout << __BOOT_MENU_T << "║             ┌────────────────────┐             ║" << endl
                 << __BOOT_MENU_T << "║             │" << menu[i]       << "│             ║" << endl
                 << __BOOT_MENU_T << "║             └────────────────────┘             ║" << endl;
        }
        cout << __BOOT_MENU_T << "║                                                ║" << endl;
    }

    cout << __BOOT_MENU_T << "║                                                ║" << endl
         << __BOOT_MENU_T << "╟────────────────────────────────────────────────╢" << endl
         << __BOOT_MENU_T << "║                                 sneakquie (c)  ║" << endl
         << __BOOT_MENU_T << "╙────────────────────────────────────────────────╜" << endl;
}

/**
 * Display classes menu
 * @param const char *menu[5]
 * @param const unsigned char active
 * @return void
 */
void display_bootstrap_classes(const char *menu[5], const unsigned char active)
{
    clr_scr();

    cout << __CLASS_MENU_N
         << __CLASS_MENU_T << "╔════════════════════════════════════════════════╗" << endl
         << __CLASS_MENU_T << "║                                                ║" << endl
         << __CLASS_MENU_T << "║          ____  ____   ____    ___   _          ║" << endl
         << __CLASS_MENU_T << "║         |  _ \\|  _ \\ / ___|  / _ \\ / |         ║" << endl
         << __CLASS_MENU_T << "║         | |_) | |_) | |  _  | | | || |         ║" << endl
         << __CLASS_MENU_T << "║         |  _ <|  __/| |_| | | |_| || |         ║" << endl
         << __CLASS_MENU_T << "║         |_| \\_\\_|    \\____|  \\___(_)_|         ║" << endl
         << __CLASS_MENU_T << "║                                                ║" << endl
         << __CLASS_MENU_T << "║                                                ║" << endl
         << __CLASS_MENU_T << "║                Select Hero Class               ║" << endl
         << __CLASS_MENU_T << "║                                                ║" << endl;

    /*
     * Display menu options
     */
    for (int i = 0; i < 5; ++i) {
        // Menu option is active
        if(active == i) {
            cout << __CLASS_MENU_T << "║             ┏━━━━━━━━━━━━━━━━━━━━┓             ║" << endl
                 << __CLASS_MENU_T << "║             ┃" << menu[i]       << "┃             ║" << endl
                 << __CLASS_MENU_T << "║             ┗━━━━━━━━━━━━━━━━━━━━┛             ║" << endl;
        }
        else {
            cout << __CLASS_MENU_T << "║             ┌────────────────────┐             ║" << endl
                 << __CLASS_MENU_T << "║             │" << menu[i]       << "│             ║" << endl
                 << __CLASS_MENU_T << "║             └────────────────────┘             ║" << endl;
        }
        cout << __CLASS_MENU_T << "║                                                ║" << endl;
    }

    cout << __CLASS_MENU_T << "║                                                ║" << endl
         << __CLASS_MENU_T << "╟────────────────────────────────────────────────╢" << endl
         << __CLASS_MENU_T << "║                                 sneakquie (c)  ║" << endl
         << __CLASS_MENU_T << "╙────────────────────────────────────────────────╜" << endl;
}

/**
 * Display inventory - body parts with current armor
 * @param const User character
 * @return void
 */
void display_inventory(const User character)
{
    clr_scr();

    display_top_bar(character);

    char inv[__MAP_SIZE_Y * __MAP_SIZE_X * 6] = "═══════════════════════════════|info: <:info %id%>, put off: <:poff %id%>, exit: <:return>|═══════════════════════════════\n"
       , result[__MAP_SIZE_Y * __MAP_SIZE_X * 6 + 2000] = ""
       , *left_bar = sprint_left_bar(character)
       , *first_a[__MAP_SIZE_Y + 2] = {NULL}
       , *second_a[__MAP_SIZE_Y + 2] = {NULL}
       , tmp[100];

    /*
     * For every body part
     */
    for(int i = 0; i < 6; ++i) {
        strcat(inv, " -");
        strcat(inv, parts[i]);
        strcat(inv, ": ");
        // There is armor on this part
        if(character.inventory[i].id) {
            sprintf(tmp, "[id:%i], name: %s, level: %i, costs: %i\n", character.inventory[i].id
                                                                  , character.inventory[i].name
                                                                  , character.inventory[i].level
                                                                  , character.inventory[i].costs);
            strcat(inv, tmp);
        }
        // Part is empty
        else
            strcat(inv, "(Empty)\n");
    }

    /*
     * Divide inventory and left bar
     */
    int f = 1
      , s = 1;
    for(first_a[0] = strtok(left_bar, "\n"); first_a[f] = strtok(NULL, "\n"); ++f);
    for(second_a[0] = strtok(inv, "\n"); second_a[s] = strtok(NULL, "\n"); ++s);

    /*
     * Split divided parts
     */
    (f > s) && (s = f);
    for(int i = 0; i < s; ++i) {
        if(first_a[i] != NULL)
            strcat(result, first_a[i]);
        if(second_a[i] != NULL)
            strcat(result, second_a[i]);
        strcat(result, "\n");
    }
    free(left_bar);

    // Display result
    cout << result
         << "══════════════════"
         << "╩══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════"
         << endl;
}

/**
 * Display top bar
 * @param const User character
 * @return void
 */
void display_top_bar(const User character)
{
    char *tmp        = sprint_user_HP(character)
       , *experience = sprint_user_EXP(character)
       , *mana       = sprint_user_MP(character)
       , *first_a[7] = {NULL}
       , *second_a[7] = {NULL}
       , *third_a[7]  = {NULL}
       , max = 1
       , result[3000] = ""
       , tmp_c[100] = "";

    /*
     * Divide health, experience and mana bars (\n)
     */
    for(first_a[0] = strtok(tmp, "\n"); first_a[max] = strtok(NULL, "\n"); ++max);
    for(max = 1, second_a[0] = strtok(experience, "\n"); second_a[max] = strtok(NULL, "\n"); ++max);
    for(max = 1, third_a[0] = strtok(mana, "\n"); third_a[max] = strtok(NULL, "\n"); ++max);

    /*
     * Concat divided parts
     */
    for(int i = 0; i < max && first_a[i] && second_a[i] && third_a[i]; ++i) {
        strcat(result, first_a[i]);
        strcat(result, second_a[i]);
        strcat(result, third_a[i]);
        strcat(result, "\n");
    }

    // Free memory
    free(tmp);
    free(mana);
    free(experience);

    // Display bars
    cout << result
         << "\t\t  ";

    // Display HP stats
    sprintf(tmp_c, "[HP: %i/%i]", character.health, character.max_health);
    tmp  = fill_right_spaces(tmp_c, 20);
    cout << tmp
         << "                                  ";
    free(tmp);

    // Display EXP stats
    sprintf(tmp_c, "[EXP: %i/%i]", character.current_exp, character.Hero.attributes[character.level].exp_next);
    tmp  = fill_right_spaces(tmp_c, 20);
    cout << tmp
         << "                             ";
    free(tmp);

    // Display MP stats
    sprintf(tmp_c, "[MP: %i/%i]", character.mana, character.max_mana);
    tmp  = fill_left_spaces(tmp_c, 20);
    cout << tmp
         << endl;
    free(tmp);
}

/**
 * Display message
 * @param const char *msg
 * @return void
 */
void display_msg(const char *msg)
{
    char *msg_n = fill_right_spaces(msg, 117);
    cout << "                  ╓─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╖" << endl
         << "                  ║  " << msg_n << "  ║" << endl
         << "                  ╙─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╜" << endl;
    free(msg_n);
}

/**
 * Generate output for left bar and return it
 * @param const User character
 * @return char*
 */
char *sprint_left_bar(const User character)
{
    char bar[1000] = ""
       , tmp_c[100] = ""
       , *tmp;

    // Info about class
    tmp = fill_right_spaces(character.Hero.name, 8);
    strcat(bar, "\n══════════════════╦\n  Class: ");
    strcat(bar, tmp);
    free(tmp);

    // Info about level
    tmp = fill_right_spaces(character.level + 1, 8);
    strcat(bar, " ║\n  Level: ");
    strcat(bar, tmp);
    free(tmp);

    // Info about coins
    tmp = fill_right_spaces(character.coins, 8);
    strcat(bar, " ║\n  Coins: ");
    strcat(bar, tmp);
    free(tmp);

    // Info about strength
    tmp = fill_right_spaces(character.strength, 8);
    strcat(bar, " ║\n  STR:   ");
    strcat(bar, tmp);
    free(tmp);

    // Info about dexterity
    tmp = fill_right_spaces(character.dexterity, 8);
    strcat(bar, " ║\n  DEX:   ");
    strcat(bar, tmp);
    free(tmp);

    // Info about intelligence
    tmp = fill_right_spaces(character.intelligence, 8);
    strcat(bar, " ║\n  INT:   ");
    strcat(bar, tmp);
    free(tmp);

    // Info about health regeneration
    tmp = fill_right_spaces(character.regen_HP, 8);
    strcat(bar, " ║\n  H_Reg: ");
    strcat(bar, tmp);
    free(tmp);

    // Info about mana regeneration
    tmp = fill_right_spaces(character.regen_MP, 8);
    strcat(bar, " ║\n  M_Reg: ");
    strcat(bar, tmp);
    free(tmp);

    strcat(bar, " ║\n");

    // Bag info (capacity and current sum weight)
    // Display bag capacity in KG
    if(character.inventory[3].capacity >= 1000) {
        // Display sum weight in KG
        if(character.sum_weight >= 1000)
            sprintf(tmp_c, "%.1f/%.1fkg", character.sum_weight / 1000., character.inventory[3].capacity / 1000.);
        // Display sum weight in g
        else
            sprintf(tmp_c, "%ig/%.1fkg", character.sum_weight, character.inventory[3].capacity / 1000.);
    }
    // Display sum weight and capacity in g
    else
        sprintf(tmp_c, "%i/%ig", character.sum_weight, character.inventory[3].capacity);

    // Fill right spaces and copy to buffer
    tmp = fill_right_spaces(tmp_c, 15);
    strcat(bar, "  ");
    strcat(bar, tmp);
    strcat(bar, " ║\n");
    free(tmp);

    // Fill rest part of bar
    for(int i = 0; i < __MAP_SIZE_Y - 9; ++i)
        strcat(bar, "                  ║\n");

    char *result = (char*) malloc(sizeof(char) * strlen(bar));
    if(NULL == result)
        exit(-1);
    strcpy(result, bar);
    return result;
}

/**
 * Generate output for HP top bar
 * @param const User character
 * @return char*
 */
char *sprint_user_HP(const User character)
{
    // Percents of HP
    unsigned char health = (int) ((float) character.health / character.max_health * 100);
    // If percents = 0, then increment
    !health && ++health;

    char buffer[1000] = "\t\t   _______\n"
       , health_s[4];

    // String view of percents
    sprintf(health_s, "%i", health);

    // >= 80%
    if(health > 79) {
        strcat(buffer, "\t\t  ⎛▓▓▓▓▓▓▓⎞\n\t\t  ⎜▓▓▓▓▓▓▓⎟\n");

        // 100% => show "MAX", because "100%" is too long
        if(health > 99)
            strcat(buffer, "\t\t  ⎜▓(MAX)▓⎟\n");
        else {
            strcat(buffer, "\t\t  ⎜▓(");
            strcat(buffer, health_s);
            strcat(buffer, "%)▓⎟\n");
        }
        strcat(buffer, "\t\t  ⎜▓▓▓▓▓▓▓⎟\n\t\t  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // >= 60%
    else if(health > 59) {
        strcat(buffer, "\t\t  ⎛       ⎞\n\t\t  ⎜▓▓▓▓▓▓▓⎟\n\t\t  ⎜▓(");
        strcat(buffer, health_s);
        strcat(buffer, "%)▓⎟\n\t\t  ⎜▓▓▓▓▓▓▓⎟\n\t\t  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // >= 40%
    else if(health > 39) {
        strcat(buffer, "\t\t  ⎛       ⎞\n\t\t  ⎜       ⎟\n\t\t  ⎜▓(");
        strcat(buffer, health_s);
        strcat(buffer, "%)▓⎟\n\t\t  ⎜▓▓▓▓▓▓▓⎟\n\t\t  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // >= 20%
    else if(health > 19) {
        strcat(buffer, "\t\t  ⎛       ⎞\n\t\t  ⎜       ⎟\n\t\t  ⎜ (");
        strcat(buffer, health_s);
        strcat(buffer, "%) ⎟\n\t\t  ⎜▓▓▓▓▓▓▓⎟\n\t\t  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // < 20%
    else {
        strcat(buffer, "\t\t  ⎛       ⎞\n\t\t  ⎜       ⎟\n\t\t  ⎜ (");
        strcat(buffer, health_s);

        if(health < 10)
            strcat(buffer, "%)  ⎟\n\t\t  ⎜       ⎟\n\t\t  ⎝▓▓▓▓▓▓▓⎠\n");
        else
            strcat(buffer, "%) ⎟\n\t\t  ⎜       ⎟\n\t\t  ⎝▓▓▓▓▓▓▓⎠\n");
    }
    strcat(buffer, "\t\t   ▔▔▔▔▔▔▔\n");

    // Alloc HP top bar in memory
    char *result = (char*) malloc(strlen(buffer) * sizeof(char));
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}

/**
 * Generate output for MP top bar
 * @param const User character
 * @return char*
 */
char *sprint_user_MP(const User character)
{
    // Mana percents
    int mana = (int) ((float) character.mana / character.max_mana * 100);
    char buffer[1000] = "   _______\n"
       , mana_s[4];

    // String view of percents
    sprintf(mana_s, "%i", mana);

    // Mana >= 80%
    if(mana > 79) {
        strcat(buffer, "  ⎛▓▓▓▓▓▓▓⎞\n  ⎜▓▓▓▓▓▓▓⎟\n");

        if(mana > 99)
            strcat(buffer, "  ⎜▓(MAX)▓⎟\n");
        else {
            strcat(buffer, "  ⎜▓(");
            strcat(buffer, mana_s);
            strcat(buffer, "%)▓⎟\n");
        }
        strcat(buffer, "  ⎜▓▓▓▓▓▓▓⎟\n  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // >= 60%
    else if(mana > 59) {
        strcat(buffer, "  ⎛       ⎞\n  ⎜▓▓▓▓▓▓▓⎟\n  ⎜▓(");
        strcat(buffer, mana_s);
        strcat(buffer, "%)▓⎟\n  ⎜▓▓▓▓▓▓▓⎟\n  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // >= 40%
    else if(mana > 39) {
        strcat(buffer, "  ⎛       ⎞\n  ⎜       ⎟\n  ⎜▓(");
        strcat(buffer, mana_s);
        strcat(buffer, "%)▓⎟\n  ⎜▓▓▓▓▓▓▓⎟\n  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // >= 20%
    else if(mana > 19) {
        strcat(buffer, "  ⎛       ⎞\n  ⎜       ⎟\n  ⎜ (");
        strcat(buffer, mana_s);
        strcat(buffer, "%) ⎟\n  ⎜▓▓▓▓▓▓▓⎟\n  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // > 0
    else if(mana > 0) {
        strcat(buffer, "  ⎛       ⎞\n  ⎜       ⎟\n  ⎜ (");
        strcat(buffer, mana_s);

        if(mana < 10)
            strcat(buffer, "%)  ⎟\n  ⎜       ⎟\n  ⎝▓▓▓▓▓▓▓⎠\n");
        else
            strcat(buffer, "%) ⎟\n  ⎜       ⎟\n  ⎝▓▓▓▓▓▓▓⎠\n");
    }

    // No mana
    else
        strcat(buffer, "  ⎛       ⎞\n  ⎜       ⎟\n  ⎜ (0%)  ⎟\n  ⎜       ⎟\n  ⎝       ⎠\n");

    strcat(buffer, "   ▔▔▔▔▔▔▔\n");

    // Allocate memory
    char *result = (char*) malloc(strlen(buffer) * sizeof(char));
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}

/**
 * Generate output for EXP top bar
 * @param const User character
 * @return char*
 */
char *sprint_user_EXP(const User character)
{
    int experience = (int) ((float) character.current_exp / character.Hero.attributes[character.level].exp_next * 100);
    char line[500] = ""
       , experience_s[7] = ""
       , buffer[1000] = "                                                                                                        "
                        "\n                                                                                                       \n ⎧";

    if(experience > 99)
        strcpy(experience_s, "[MAX]");
    else
        sprintf(experience_s, "[%i%%]", experience);

    /*
     * First line
     */
    for(int i = 0; i < experience; ++i)
        strcat(line, "░");
    for(int i = experience; i < 100; ++i)
        strcat(line, "∙");
    strcat(buffer, line);
    strcat(buffer, "⎫\n ⎨");

    /*
     * Second line
     */
    if(experience > 47) {
        for(int i = 0; i < 47; ++i)
            strcat(buffer, "░");

        strcat(buffer, experience_s);

        unsigned char diff = 52;
        for(int i = 52; i < experience; ++i) {
            strcat(buffer, "░");
            ++diff;
        }
        for(int i = diff; i < 100; ++i)
            strcat(buffer, "∙");
    }
    else {
        for(int i = 0; i < experience; ++i)
            strcat(buffer, "░");
        for(int i = experience; i < 47; ++i)
            strcat(buffer, "∙");

        strcat(buffer, experience_s);

        for(int i = 47 + strlen(experience_s); i < 100; ++i)
            strcat(buffer, "∙");
    }
    strcat(buffer, "⎬\n ⎩");

    /*
     * Third line
     */
    strcat(buffer, line);
    strcat(buffer, "⎭\n                                                                                                       "
                   "\n                                                                                                        ");

    char *result = (char*) malloc(strlen(buffer) * sizeof(char));
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}

/**
 * Fill left spaces to int value
 * @param const int value
 * @param const unsigned char number
 * @return char*
 */
char *fill_left_spaces(const int value, const unsigned char number)
{
    char value_s[11]
       , buffer[200] = ""
       , *result;
    sprintf(value_s, "%i", value);
    // Fill spaces
    for(int i = 0, diff = number - strlen(value_s); i < diff && strcat(buffer, " "); ++i);
    // Copy value
    strcat(buffer, value_s);

    // Alloc memory
    result = (char *) malloc(strlen(buffer) * sizeof(char) + 1);
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}

/**
 * Fill left spaces to string value
 * @param const char *value
 * @param const unsigned char number
 * @return char*
 */
char *fill_left_spaces(const char *value, const unsigned char number)
{
    char buffer[200] = ""
       , *result;
    // Fill spaces
    for(int i = 0, diff = number - strlen(value); i < diff && strcat(buffer, " "); ++i);
    // Copy string value
    strcat(buffer, value);

    // Alloc memory
    result = (char *) malloc(strlen(buffer) * sizeof(char) + 1);
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}

/**
 * Fill right spaces to string value
 * @param const int value
 * @param const unsigned char number
 * @return char*
 */
char *fill_right_spaces(const int value, const unsigned char number)
{
    char buffer[200] = ""
       , *result;
    // Copy value to result
    sprintf(buffer, "%i", value);
    // Fill spaces
    for(int i = 0, diff = number - strlen(buffer); i < diff && strcat(buffer, " "); ++i);

    result = (char *) malloc(strlen(buffer) * sizeof(char) + 1);
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}
/**
 * Fill right spaces to string value
 * @param const char *value
 * @param const unsigned char number
 * @return char*
 */
char *fill_right_spaces(const char *value, const unsigned char number)
{
    char buffer[200] = ""
       , *result;
    // Copy value
    strcpy(buffer, value);
    // Fill spaces
    for(int i = 0, diff = number - strlen(buffer); i < diff && strcat(buffer, " "); ++i);

    // Alloc memory
    result = (char *) malloc(strlen(buffer) * sizeof(char) + 1);
    if(NULL == result)
        exit(-1);
    strcpy(result, buffer);
    return result;
}

/**
 * Die message, exit
 * @return void
 */
void die(void)
{
    clr_scr();
    cout << "    ___________________________________________________________________________________________________" << endl
         << "    |_________________________________________________________________________________________________|" << endl
         << "    |               ______                                                                            |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |       _______|      |______                                                                     |" << endl
         << "    |      ||         RIP        |                                                                    |" << endl
         << "    |      ||                    |                                                                    |" << endl
         << "    |      |_______        ______|                You died like a hero...                             |" << endl
         << "    |              ||     |                       Don't worry, God takes the good ones                |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |              ||     |                                                                           |" << endl
         << "    |              |______|                                                                           |" << endl
         << "    |                                                                                                 |" << endl
         << "    +-------------------------------------------------------------------------------------------------+" << endl << endl;
        exit(-1);
}

/**
 * Clear screen
 * @return void
 */
void clr_scr(void)
{
    system("clear");
}

/**
 * Delay time
 * @param unsigned int delay
 * @return void
 */
void delay(unsigned int delay)
{
    delay += time(NULL);
    while(time(NULL) < delay);
}

#endif