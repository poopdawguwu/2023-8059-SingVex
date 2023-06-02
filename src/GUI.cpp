#include "display/lv_core/lv_obj.h"
#include "main.h"
#include "display/lvgl.h"

const char *btnmap[] = {map, NULL};
autonSelectT autonChoice;

lv_res_t handler( lv_obj_t *btnmatrix, const char *t){
    int autonNum;

	for(int i = 0; i < 5; i++){
		if(strcmp(t, btnmap[i]) == 0){
            autonNum = i;
		}
	}

    switch(autonNum){
        case 0:
            autonChoice = RED1;
            break;
        case 1:
            autonChoice = RED2;
            break;
        case 3:
            autonChoice = BLUE1;
            break;
        case 4:
            autonChoice = BLUE2;
            break;
    }

    return LV_RES_OK;
}

autonSelectT getAutonChoice(){
    return autonChoice;
}

void btnmatrix() {
    lv_theme_t *th = lv_theme_alien_init(210, NULL); //Set a HUE value and keep font default RED
    lv_theme_set_current(th);
    lv_obj_t *btnm = lv_btnm_create(lv_scr_act(), NULL);
    lv_btnm_set_map(btnm, btnmap);
	lv_obj_align(btnm, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_btnm_set_action(btnm, handler);
}