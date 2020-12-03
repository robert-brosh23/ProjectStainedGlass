#include <stdio.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "basic_2d.h"

/*
struct location{
    int x;
    int y;
    int speed;
    int dir;
}typedef location;
*/

location update_location(location old_loc){
    location new_loc;
    if(check_for_collision(old_loc)){
        //Collision reaction
    }
    else{
        //Just keep going
        new_loc.x = (float)old_loc.x + cos((float)old_loc.dir)*(float)old_loc.speed;
        new_loc.y = (float)old_loc.y + sin((float)old_loc.dir)*(float)old_loc.speed;
        new_loc.speed = old_loc.speed;
        new_loc.dir = old_loc.dir;
    }

    return(new_loc);
}

bool check_for_collision(location loc){

    return 0;
}

float convert_deg_to_rad(float deg){
    return ((deg * PI)/180);
}