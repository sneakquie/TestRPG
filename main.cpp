#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "defines.h"
#include "user.h"
#include "display.h"
#include "npc.h"
#include "bootstrap.h"
#include "kbhit.c"
#include "user.c"
#include "npc.c"
#include "classes.c"
#include "display.c"
#include "bootstrap.c"

using namespace std;

int main(void)
{
    // Initialize user var
    User character = bootstrap_menu() ? bootstrap_load_game() : bootstrap_new_game();

    display_user_map(character);

    unsigned int last_move = time(NULL)
               , move_history[__MAP_SIZE_X][__MAP_SIZE_Y] = {0};
    srand(last_move);

    do {
        // Waiting for keydown
        while(!kbhit());
        const char answer = getchar();
        unsigned int current_time = time(NULL);

        if( (answer == 100
            && move_right(&character))
         || (answer == 119
            && move_top(&character))
         || (answer == 115
            && move_bottom(&character))
         || (answer == 97
            && move_left(&character))
        ) {
            /*
             * Regeneration
             */
            if(last_move + __USR_REGEN_TIME <= current_time) {
                last_move = current_time;
                update_HP(&character, character.regen_HP);
                update_MP(&character, character.regen_MP);
            }

            /*
             * Fight with a NPC
             */
            if(move_history[character.position_x][character.position_y] <= current_time && !(rand() % __NPC_CHANCE))
                bootstrap_fight(current_time, &move_history, &character);
        }

        // ":" typed
        else if(answer == 58) {
            display_user_map(character);
            cout << ':';

            char cmd[32];
            gets(cmd);

            // Want to exit
            if(!strcmp(cmd, "exit")) {
                clr_scr();
                break;
            }
            // Show bag
            else if(!strcmp(cmd, "bag"))
                bootstrap_bag(&character);
            // Show inventory
            else if(!strcmp(cmd, "inventory"))
                bootstrap_inventory(&character);
            // Show shop
            else if(!strcmp(cmd, "shop"))
                bootstrap_shop(&character);
        }
        display_user_map(character);
    } while(1);
    return 0;
}