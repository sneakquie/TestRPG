#ifndef NPC_H

#define NPC_H

typedef struct npc {
    const char *name;
    const unsigned char drop_items[10];
    const unsigned short int power;
    unsigned char level;
} NPC;

char *NPC_fight(User*, const NPC);

#endif