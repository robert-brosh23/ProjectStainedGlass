#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
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

//Prints a string and variable to place on screen and size
void print_text(char* text, float num_to_print, int x, int y, int font_size){
    if(num_to_print==-100){//This is the null character flag
        DrawText(text,x,y,font_size,DARKGRAY);
    }
    else{
        char str[100];
        sprintf(str, "%f", num_to_print);
        char to_print[100];
        strcpy(to_print, text);
        strcat(to_print, str);
        DrawText(to_print, x, y, font_size, DARKGRAY);
    }
}

location update_location(location old_loc, collision_return col_ret, collision_body hitbox){
    location new_loc;

    if(col_ret.edge != -1){ //Add another line
        //Collision reaction
        int i = col_ret.edge;
        //print_text("edge angle collided: ", hitbox.edge_angle[i], 500, 380, 20);
        new_loc.dir = calculate_collision_dir(old_loc, hitbox, i);
    }
    else new_loc.dir = old_loc.dir;
    //Calculate new location using equation x = x_0 + v_x*dt
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
collision_return check_for_collision(location loc, body_coords* bodies, int num_bodies){
    
    int right_edge_cord[num_bodies];
    int left_edge_cord[num_bodies];
    int top_edge_cord[num_bodies];
    int bot_edge_cord[num_bodies];
    int i = 0, j = 0;
    for(i = 0; i < num_bodies; i++){
        right_edge_cord[i] = -1;
        left_edge_cord[i] = __INT_MAX__;
        top_edge_cord[i] = __INT_MAX__;
        bot_edge_cord[i] = -1;
    }
    collision_return col_ret;
    col_ret.body = -1;
    col_ret.edge = -1;
    //Create collision bounds
    for(j = 0; j<num_bodies; j++){ 
        for(i = 0; i<bodies[j].num_points; i++){
            if(bodies[j].points[i].x > right_edge_cord[j]){
                right_edge_cord[j] = bodies[j].points[i].x;
            }
            if(bodies[j].points[i].x < left_edge_cord[j]){
                left_edge_cord[j] = bodies[j].points[i].x;
            }
            if(bodies[j].points[i].y < top_edge_cord[j]){
                top_edge_cord[j] = bodies[j].points[i].y;
            }
            if(bodies[j].points[i].y > bot_edge_cord[j]){
                bot_edge_cord[j] = bodies[j].points[i].y;
            }
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

    //ADD [i] to all right left down and top coords
    //ADD return col_ret.body = i to all IF statements
    for(i = 0; i < num_bodies; i++){
        //This if statement checks if current location lies inside of a collision box
        if(loc.x > left_edge_cord[i] && loc.x < right_edge_cord[i] && loc.y > top_edge_cord[i] && loc.y < bot_edge_cord[i]){
            if(!faces_right(loc) && faces_up(loc)){ //Either right or bottom
                del_x = loc.x - right_edge_cord[i];
                del_y = loc.y - bot_edge_cord[i];
                //closer to right edge if true
                if(abs(del_x)<abs(del_y)){
                    col_ret.edge = 2;
                }
                else col_ret.edge = 1;
                //Assign the body that loc collided with
                col_ret.body = i;
            }
            if(!faces_right(loc) && !faces_up(loc)){ //Either right or top
                del_x = loc.x - right_edge_cord[i];
                del_y = loc.y - top_edge_cord[i];
                if(abs(del_x)<abs(del_y))//closer to right edge if true
                    col_ret.edge = 2;
                else col_ret.edge = 3;
                col_ret.body = i;
            }
            if(faces_right(loc) && faces_up(loc)){ //Either left or bottom edge
                del_x = loc.x - left_edge_cord[i];
                del_y = loc.y - bot_edge_cord[i];
                if(abs(del_x)<abs(del_y))//closer to left edge
                    col_ret.edge = 0;
                else col_ret.edge = 1;
                col_ret.body = i;
            }
            if(faces_right(loc) && !faces_up(loc)){ //Either left or top edge
                del_x = loc.x - left_edge_cord[i];
                del_y = loc.y - top_edge_cord[i];
                if(abs(del_x)<abs(del_y))//closer to left edge
                    col_ret.edge = 0;
                else col_ret.edge = 3;
                col_ret.body = i;
            }
        }
    }
    //print_text("Body colided: ", col_ret.body, 500, 300, 30);
    //print_text("edge colided: ", col_ret.edge, 500, 320, 30);
    return col_ret;
}

float convert_deg_to_rad(float deg){
    //convert from degrees to radians
    return ((deg * PI)/180);
}

//Return 1 for right
//Return 0 for else
bool faces_right(location loc){
    //Checking if direction lies on right side of unit circle
    if(loc.dir>(-PI/2) && loc.dir<(PI/2)) return 1;
    else return 0;
}

//Return 1 for up
//Return 0 for else
bool faces_up(location loc){
    if(loc.dir<0) return 1;
    else return 0;
}

body_coords create_rectangle(int x_upper, int x_lower, int y_upper, int y_lower){
    body_coords rectangle;
    rectangle.num_points = 4;
    rectangle.points[0] = (Vector2){x_lower, y_lower};
    rectangle.points[1] = (Vector2){x_lower, y_upper};
    rectangle.points[2] = (Vector2){x_upper, y_upper};
    rectangle.points[3] = (Vector2){x_upper, y_lower};
    return rectangle;
}

collision_body assign_col_parameters(body_coords coords){
    int i = 0;
    collision_body body;
    //number of edges is number of points
    body.num_edges = coords.num_points;
    //Assign edges
    for(i = 0; i<(body.num_edges-1); i++){
        body.edge_start[i] = coords.points[i];
        body.edge_end[i] = coords.points[i+1];
    }
    body.edge_start[body.num_edges-1] = coords.points[coords.num_points-1];
    body.edge_end[body.num_edges-1] = coords.points[0];
    //Assign edge norms
    //float edge_angle;
    for(i = 0; i<body.num_edges; i++){
        if(abs(body.edge_end[i].x - body.edge_start[i].x) < 1){
            //Accounts for vertical lines
            body.edge_angle[i] = PI/2;
        }
        else{
            body.edge_angle[i] = atan((float)(body.edge_end[i].y-body.edge_start[i].y)/
                                    (float)(body.edge_end[i].x-body.edge_start[i].x));
        }
        //Check for angles out of bounds
        //i.e, and angle of 100 / should look like an angle of -80 / .
        while(body.edge_angle[i] > (PI/2 + .01)) body.edge_angle[i] = body.edge_angle[i] - PI/2;
        while(body.edge_angle[i] < (-PI/2 - .01)) body.edge_angle[i] = body.edge_angle[i] + PI/2;
        //body.edge_norm[i] = (float)90+edge_angle;
    }
    return body;
}

float calculate_collision_dir(location loc, collision_body col, int collision_edge){
    float new_dir;
    new_dir = 2 * (col.edge_angle[collision_edge] - loc.dir) + loc.dir;
    //Correct for angles outside of range
    while(new_dir > PI) new_dir = new_dir - 2*PI;
    while(new_dir < -PI) new_dir = new_dir + 2*PI;
    return new_dir;
}

line rotate_user_line(line user_line, float user_dir, int user_line_length){
    line new_user_line;
    //Always keep line start at the same place
    new_user_line.start = user_line.start;
    //Rotate line ending
    new_user_line.end = (Vector2){user_line.start.x + cos(user_dir) * user_line_length,
                        user_line.start.y + sin(user_dir) * user_line_length};
    //Line color and thickness stay the same
    new_user_line.thickness = user_line.thickness;
    new_user_line.color = user_line.color;
    return new_user_line;

}

//Draw a series of line structs on the play area
//Pass in a pointer to the beginning of lines and number of lines
void draw_line_series(line* lines, int num_lines){
    int i = 0;
    for(i = 0; i<num_lines; i++){
        DrawLineEx(lines[i].start, lines[i].end,
            lines[i].thickness, lines[i].color);
    }
}

//Draw a series of rectangles on the play area
//Pass in a point to the beginning of the rectangles and number of rectangles
void draw_rectangles(body_coords* bodies, int num_bodies, int num_body_goal){
    int i = 0;
    for(i = 0; i < num_bodies; i++){
        if(i == num_body_goal){
            DrawRectangle(bodies[i].points[0].x, bodies[i].points[0].y, 
                bodies[i].points[2].x-bodies[i].points[0].x, 
                bodies[i].points[2].y-bodies[i].points[0].y, BLACK);
        }
        else{
            DrawRectangle(bodies[i].points[0].x, bodies[i].points[0].y, 
                bodies[i].points[2].x-bodies[i].points[0].x, 
                bodies[i].points[2].y-bodies[i].points[0].y, BLUE);
        }
    }
}

void draw_player(Vector2 current_loc){
    DrawCircleV(current_loc, 20, MAROON);
}

//This is what will show while the user is selecting their direction
void draw_game_state(int game_state, line* lines, int num_lines, body_coords* rectangles, int num_bodies, int num_body_goal){
    if(game_state == 0){ //Title screen
        print_text("Welcome to the game!\nPress space to start!\nPress S to shoot your lazer\nPress q to pause\nReset the game with r\nAim your lazer with left and right arrows", -100, 200, 20, 40);
    }
    else{ //Level 1
        draw_rectangles(rectangles, num_bodies, num_body_goal);

        draw_line_series(lines, num_lines);
    }

    //Only happens after a lazer has been sent
    if(game_state%2 == 0 && game_state != 0){
        //Take the last line, and take the end of it, this is the current location
        draw_player(lines[num_lines-1].end);
    }
}