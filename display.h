#ifndef DISPLAY_H

#define DISPLAY_H

void display_user_map(const User);
void display_user_bag(const User, unsigned char, unsigned char);
void display_shop(const User, unsigned char);
void display_item_info(const User, unsigned char);
void display_bootstrap_menu(const char *, const unsigned char);
void display_bootstrap_classes(const char, const unsigned char);
void display_top_bar(const User);
void display_msg(const char *);

char *sprint_left_bar(const User);
char *sprint_user_HP(const User);
char *sprint_user_MP(const User);
char *sprint_user_EXP(const User);

char *fill_left_spaces(const int, const unsigned char);
char *fill_left_spaces(const char *, const unsigned char);
char *fill_right_spaces(const int, const unsigned char);
char *fill_right_spaces(const char *, const unsigned char);

void die(void);
void clr_scr(void);
void delay(unsigned int);

#endif