#include <stdio.h>
#include "function.h"


int main(){
    ch *enemy1 = enemy_1();
    setup_console();
    Start_setup();
    Start_basic();
    ch *user = character_making();
    getchar();
    figth_event(user,enemy1);
    






    free(user);
    free(enemy1);
}