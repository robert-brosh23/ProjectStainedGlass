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

void print_text(char* text, float num_to_print, int x, int y, int font_size){
    char str[100];
    sprintf(str, "%f", num_to_print);
    char to_print[100];
    strcpy(to_print, text);
    strcat(to_print, str);
    DrawText(to_print, x, y, 20, DARKGRAY);
}

location update_location(location old_loc, body_coords body){
    location new_loc;
    collision_body col = assign_col_parameters(body);
    print_text("edge 3: ", col.edge_angle[3],400,300, 20);
    int edge_col = check_for_collision(old_loc, body);
    if(edge_col != -1){
        //Collision reaction
        float new_dir = calculate_collision_dir(old_loc, col, edge_col);
        new_loc.dir = new_dir;
        //Save this line and print it in all future frames
        //Create a new line with same speed and location as old on
        //This one has the new direction
    }
    else new_loc.dir = old_loc.dir;
    //Just keep going
    new_loc.x = round((float)old_loc.x + cos((float)new_loc.dir)*(float)old_loc.speed);
    new_loc.y = round((float)old_loc.y + sin((float)new_loc.dir)*(float)old_loc.speed);
    new_loc.speed = old_loc.speed;
    

    return(new_loc);
}

//Returns -1 for no collision
//Returns 0 if collision occurs on right
//Returns 1 if collision occurs on bottom
//Returns 2 if collision occurs on left
//Returns 3 if collision occurs on top
int check_for_collision(location loc, body_coords body){
    
    int right_edge_cord = -1;
    int left_edge_cord = __INT_MAX__;
    int top_edge_cord = __INT_MAX__;
    int bot_edge_cord = -1;
    int i = 0;
    //Create collision bounds
    for(i = 0; i<body.num_points; i++){
        if(body.points[i].x > right_edge_cord){
            right_edge_cord = body.points[i].x;
        }
        if(body.points[i].x < left_edge_cord){
            left_edge_cord = body.points[i].x;
        }
        if(body.points[i].y < top_edge_cord){
            top_edge_cord = body.points[i].y;
        }
        if(body.points[i].y > bot_edge_cord){
            bot_edge_cord = body.points[i].y;
        }
    }
    /*
    print_text("Right: ", right_edge_cord, 600, 10, 20);
    print_text("Top: ", top_edge_cord, 600, 30, 20);
    print_text("Left: ", left_edge_cord, 600, 50, 20);
    print_text("Bottom: ", bot_edge_cord, 600, 70, 20);
    */
    //Check if endpoint is inside the horizontal collision bounds
    int del_x;
    int del_y;
    if(loc.x > left_edge_cord && loc.x < right_edge_cord && loc.y > top_edge_cord && loc.y < bot_edge_cord){
        if(!faces_right(loc) && faces_up(loc)){ //Either right or bottom
            del_x = loc.x - right_edge_cord;
            del_y = loc.y - bot_edge_cord;
            if(abs(del_x)<abs(del_y))//closer to right edge if true
                return 2;
            else return 1;
        }
        if(!faces_right(loc) && !faces_up(loc)){ //Either right or top
            del_x = loc.x - right_edge_cord;
            del_y = loc.y - top_edge_cord;
            if(abs(del_x)<abs(del_y))//closer to right edge if true
                return 2;
            else return 3;
        }
        if(faces_right(loc) && faces_up(loc)){ //Either left or bottom edge
            del_x = loc.x - left_edge_cord;
            del_y = loc.y - bot_edge_cord;
            if(abs(del_x)<abs(del_y))//closer to left edge
                return 0;
            else return 1;
        }
        if(faces_right(loc) && !faces_up(loc)){ //Either left or top edge
            del_x = loc.x - left_edge_cord;
            del_y = loc.y - top_edge_cord;
            if(abs(del_x)<abs(del_y))//closer to left edge
                return 0;
            else return 3;
        }
    }
    return -1;
}

float convert_deg_to_rad(float deg){
    return ((deg * PI)/180);
}

//Return 1 for right
//Return 0 for else
bool faces_right(location loc){
    if(loc.dir>(-PI/2) && loc.dir<(PI/2)) return 1;
    else return 0;
}

//Return 1 for up
//Return 0 for else
bool faces_up(location loc){
    if(loc.dir<0) return 1;
    else return 0;
}

collision_body assign_col_parameters(body_coords coords){
    int i = 0;
    collision_body body;
    body.num_edges = coords.num_points-1;
    //Assign edges
    for(i = 0; i<coords.num_points; i++){
        body.edge_start[i] = coords.points[i];
        body.edge_end[i] = coords.points[i+1];
    }
    body.edge_start[i+1] = coords.points[i+1];
    body.edge_end[i+1] = coords.points[0];
    //Assign edge norms
    //float edge_angle;
    for(i = 0; i<body.num_edges; i++){
        body.edge_angle[i] = atan((float)(body.edge_end[i].y-body.edge_start[i].y)/
                                (float)(body.edge_end[i].x-body.edge_start[i].x));
        //body.edge_norm[i] = (float)90+edge_angle;
    }
    return body;
}

float calculate_collision_dir(location loc, collision_body col, int collision_edge){
    float new_dir;
    new_dir = 2 * (col.edge_angle[collision_edge] - loc.dir) + loc.dir;
    return new_dir;
}