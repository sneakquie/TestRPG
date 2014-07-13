#ifndef NPC_C

#define NPC_C

NPC NPC_list[__NPC_NUMBER] = {
    {"Skeleton", {1, 2}, 10},
    {"Bear", {1, 3}, 13},
    {"Snake", {}, 5},
    {"Dark Mage", {1, 2, 6, 4, 7}, 17}
};

char *NPC_fight(User *character, const NPC opponent)
{
    unsigned short int power  = character->strength + character->dexterity + character->intelligence - opponent.power * opponent.level
                     , experience = random((character->level + 1) * 10, (character->level + 1) * 40) + random(1, 6) * (opponent.level - character->level)
                     , coins = random((character->level + 1) * 3, (character->level + 1) << 6);

    update_HP(character, -(character->max_health * random(1, 50) / 100));
    update_MP(character, -(character->max_mana * random(1, 50) / 100));
    update_EXP(character, experience);
    character->coins += coins;

    char msg[100] = ""
       , *result;
    sprintf(msg, "You've fought %s and got %i EXP and %i coins", opponent.name, experience, coins);

    if(NULL == (result = (char *) malloc(strlen(msg) * sizeof(char)) + 1))
        exit(-1);
    strcpy(result, msg);
    return result;
}

#endif