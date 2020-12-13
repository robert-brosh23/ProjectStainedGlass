
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "basic_2d.h"

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");

    // Initialize camera as 2d
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ 0.0f, 0.0f };
    camera.target = (Vector2){ 0.0, 0.0 };
    camera.rotation = 0;
    camera.zoom = .5;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    location starting_loc;
    location current_loc;
    current_loc.speed = 20;
    current_loc.dir = convert_deg_to_rad(30);
    starting_loc = current_loc;
    int frame_count = 0;
    int num_bodies[] = {7, 7};
    int num_levels = 2;
    
    int num_body_goal = 6;
    int level = 0;
    //These are all the hitboxes of each level
    //2D list, the first index corresponds to the level of the hitbox,
    //the second corresponds to the hitbox index of the level

    body_coords level_bodies[num_levels][num_bodies[0]];
    level_bodies[0][0] = create_rectangle(1600, 0, 100, 0);
    level_bodies[0][1] = create_rectangle(100, 0, 800, 0);
    level_bodies[0][2] = create_rectangle(100, 0, 900, 700);
    level_bodies[0][3] = create_rectangle(1600, 1500, 900, 0);
    level_bodies[0][4] = create_rectangle(500,200,300,250);
    level_bodies[0][5] = create_rectangle(600, 500, 300, 0);
    level_bodies[0][6] = create_rectangle(500, 400, 200, 150);
    

    level_bodies[1][0] = create_rectangle(1600, 0, 100, 0);
    level_bodies[1][1] = create_rectangle(100, 0, 800, 0);
    level_bodies[1][2] = create_rectangle(100, 0, 900, 700);
    level_bodies[1][3] = create_rectangle(1600, 1500, 900, 0);
    level_bodies[1][4] = create_rectangle(600, 0, 800, 540);
    level_bodies[1][5] = create_rectangle(1200, 700, 600, 400);
    level_bodies[1][6] = create_rectangle(700, 400, 200, 150);

    int i;
    //Assign collision parameters in all levels at the beginning
    collision_body hitboxes[num_levels][num_bodies[0]];
    for(i = 0; i < num_levels; i++){
        for(int x = 0; x < num_bodies[i]; x++){
            hitboxes[i][x] = assign_col_parameters(level_bodies[i][x]);
        }
    }

    //int col_res = -1;

    
    //Assigning collision body only needs to be done once.
    //collision_body collision_rectangle = assign_col_parameters(rectangle);
    //int overlap = 0;
    // Main game loop
    bool pause = 0;
    line user_line;
    int user_line_length = 400;
    user_line.start = (Vector2){screenWidth/2 * (1/camera.zoom), screenHeight * (1/camera.zoom)};
    user_line.end = (Vector2){user_line.start.x, user_line.start.y - user_line_length};
    user_line.color = RED;
    user_line.thickness = 10;
    float user_dir = -PI/2;

    line *lines;
    int num_lines=1;
    lines = (line *)malloc(sizeof(line));
    //lines[0].start = (Vector2){starting_loc.x, starting_loc.y};
    lines[0] = user_line;
    
    int game_state = 0;
    bool level_done = 0;

    collision_return col_ret;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(level_done){
            if(IsKeyPressed(KEY_C)){
                game_state ++; //Game state will now be odd, waiting for user input again
                level ++;
                level_done = 0;
                free(lines);
                num_lines = 1;
                lines = (line *)malloc(sizeof(line));
                lines[0] = user_line;
                user_dir = -PI/2;
            }
        }
        //This is the title screen
        else if(game_state == 0){
            if(IsKeyPressed(KEY_SPACE)){
                game_state ++;
            }
        }
        //If game state is odd, we are waiting for user input
        else if(game_state%2 == 1){
            //We are waiting for the user input
            current_loc.dir = user_dir;
            if(IsKeyPressed(KEY_S)){
                game_state ++;
                lines[0].start = user_line.start;
                current_loc.x = lines[0].start.x;
                current_loc.y = lines[0].start.y;
            }
            if(IsKeyDown(KEY_LEFT)){
                user_dir = user_dir - .01;
            }
            if(IsKeyDown(KEY_RIGHT)){
                user_dir = user_dir + .01;
            }
            lines[0] = rotate_user_line(lines[0], user_dir, user_line_length);
        }
        //Pause button is Q
        else if(IsKeyPressed(KEY_Q)) pause = !pause;
        //Only update the game if pause is off
        //Game state is even is when the user has shot the lazer and we continuously update the location
        else if(game_state%2 == 0 && frame_count%1==0 && !pause && game_state != 0){
            //Check for collision
            col_ret = check_for_collision(current_loc, level_bodies[level], num_bodies[level]);
            //Check if we have reached goal. If not, make a new line. If so, go to the next level.
            if(col_ret.edge != -1 && col_ret.body != num_body_goal){//This means a collision has occured
                //Make new line
                num_lines = num_lines + 1;
                //Endpoint of the last line is current_loc
                lines[num_lines-2].end = (Vector2){current_loc.x, current_loc.y};
                //Reallocate to add space for another line
                lines = (line *)realloc(lines, sizeof(line)*(num_lines));
                //startpoint of new line is also current_loc
                lines[num_lines-1].start = (Vector2){current_loc.x, current_loc.y};
            }
            if(col_ret.body == num_body_goal){ //Collided into goal hitbox, and the level is done
                level_done = 1;
            }
            else{
                //Update line location
                current_loc = update_location(current_loc, col_ret, hitboxes[level][col_ret.body]);
                lines[num_lines-1].end = (Vector2){current_loc.x, current_loc.y};
            }
            
            for(i = 0; i < num_lines; i++){
                lines[i].color = RED;
                lines[i].thickness = 10;
            }
        }
        if(IsKeyPressed(KEY_R)){ //Reset game
            game_state = 0;
            level = 0;
            free(lines); //Prevent memory leaks
            num_lines = 1; //Reset number of lines to 1
            lines = (line *)malloc(sizeof(line)); //Get this space back
            lines[0] = user_line; //Put back to user_line to get ready for user input
            user_dir = -PI/2; //Line is straight up by default
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            

            BeginMode2D(camera);


               draw_game_state(game_state, lines, num_lines, level_bodies[level], num_bodies[level], num_body_goal);
                

            EndMode2D();

            //("Collision: ", col_res, 10, 400, 20);
            print_text("Dir: ",current_loc.dir, 10, 340,20);
            print_text("X: ",current_loc.x, 10, 360,20);
            //print_text("hitbox[1]angle: ", hitboxes[1].edge_angle[0], 10, 380, 20);
            //print_text("user start x: ",user_line.start.x, 500,380,20);
            //print_text("line0start ",lines[0].start.x, 500,400,20);
            print_text("edge angle: ", hitboxes[level][4].edge_angle[2], 500, 220, 20);
            print_text("Game state: ", game_state, 500,200,20);
            print_text("hitbox3edge3: ", hitboxes[level][3].edge_angle[3], 500, 240, 20);
            print_text("hitbox edgestart3: ", hitboxes[level][3].edge_end[3].y, 500, 260, 20);
            if(level_done==1){
                print_text("You won the level. Press C to continue",0,500,280,20);
            }

            DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
        frame_count++;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}