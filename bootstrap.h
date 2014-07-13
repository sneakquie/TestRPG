#ifndef BOOTSTRAP_H

#define BOOTSTRAP_H

unsigned char bootstrap_menu(void);
User bootstrap_new_game(void);
User bootstrap_load_game(void);
void bootstrap_fight(const unsigned int, unsigned int (*move_history)[__MAP_SIZE_X][__MAP_SIZE_Y], User *);
void bootstrap_bag(User *);
void bootstrap_shop(User *);
void bootstrap_inventory(User *);
unsigned int random(const unsigned int, const unsigned int);
unsigned char wait_answer(void);

#endif