
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

    Camera2D camera = { 0 };
    camera.offset = (Vector2){ 0.0f, 0.0f };
    camera.target = (Vector2){ 0.0, 0.0 };
    camera.rotation = 0;
    camera.zoom = .5;
    //camera.type = CAMERA_PERSPECTIVE;
    
    //SetCameraMode(camera, CAMERA_ORBITAL);

    //Vector3 cubePosition = { 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    location starting_loc;
    location current_loc;
    current_loc.speed = 10;
    current_loc.dir = convert_deg_to_rad(30);
    starting_loc = current_loc;
    int frame_count = 0;
    int num_bodies = 6;
    body_coords all_bodies[num_bodies];
    body_coords rectangle;
    rectangle = create_rectangle(500,200,300,250);
    all_bodies[0] = rectangle;
    
    body_coords rectangle2;
    rectangle2 = create_rectangle(600, 500, 300, 0);
    all_bodies[1] = rectangle2;

    all_bodies[2] = create_rectangle(100, 0, 900, 700);
    all_bodies[3] = create_rectangle(1600, 0, 100, 0);
    all_bodies[4] = create_rectangle(100, 0, 800, 10);
    all_bodies[5] = create_rectangle(700, 600, 700, 600);
    int num_body_goal = 5;

    

    collision_body hitboxes[num_bodies];
    for(int x = 0; x < num_bodies; x++){
        hitboxes[x] = assign_col_parameters(all_bodies[x]);
    }

    //int col_res = -1;

    
    //Assigning collision body only needs to be done once.
    //collision_body collision_rectangle = assign_col_parameters(rectangle);
    //int overlap = 0;
    // Main game loop
    bool pause = 0;
    int i;
    int user_input = 1;
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

    collision_return col_ret;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if(game_state == 0){
            if(IsKeyPressed(KEY_SPACE)){
                game_state = 1;
            }
        }
        if(game_state == 1){
            //We are waiting for the user input
            current_loc.dir = user_dir;
            if(IsKeyPressed(KEY_S)){
                game_state = 2;
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
        else if(IsKeyPressed(KEY_Q)) pause = !pause;
        else if(game_state == 2 && frame_count%1==0 && !pause){
            //Check for collision
            col_ret = check_for_collision(current_loc, all_bodies, num_bodies);
            if(col_ret.edge != -1){//This means a collision has occured
                //Make new line
                num_lines = num_lines + 1;
                //Endpoint of the last line is current_loc
                lines[num_lines-2].end = (Vector2){current_loc.x, current_loc.y};
                //Reallocate to add space for another line
                lines = (line *)realloc(lines, sizeof(line)*(num_lines));
                //startpoint of new line is also current_loc
                lines[num_lines-1].start = (Vector2){current_loc.x, current_loc.y};
            }
            //Update line location
            current_loc = update_location(current_loc, col_ret, hitboxes[col_ret.body]);
            lines[num_lines-1].end = (Vector2){current_loc.x, current_loc.y};
            
            for(i = 0; i < num_lines; i++){
                lines[i].color = RED;
                lines[i].thickness = 10;
            }
        }
        if(IsKeyPressed(KEY_R)){ //Reset game
            game_state = 0;
            free(lines);
            num_lines = 1;
            lines = (line *)malloc(sizeof(line));
            lines[0] = user_line;
            user_dir = -PI/2;
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            

            BeginMode2D(camera);

                //DrawRectangle(rectangle.points[0].x, rectangle.points[0].y, 
                //    rectangle.points[2].x-rectangle.points[0].x, 
                //    rectangle.points[2].y-rectangle.points[0].y, BLUE);
                /*
                draw_rectangles(all_bodies, num_bodies);
                
                draw_line_series(lines, num_lines);

                if(user_input==1){
                    draw_user_features();
                    draw_line_series(&user_line, 1);
                }
                */
               draw_game_state(game_state, lines, num_lines, all_bodies, num_bodies, num_body_goal);
                

            EndMode2D();

            //("Collision: ", col_res, 10, 400, 20);
            print_text("Dir: ",current_loc.dir, 10, 340,20);
            print_text("X: ",current_loc.x, 10, 360,20);
            //print_text("hitbox[1]angle: ", hitboxes[1].edge_angle[0], 10, 380, 20);
            //print_text("user start x: ",user_line.start.x, 500,380,20);
            //print_text("line0start ",lines[0].start.x, 500,400,20);
            print_text("edge angle: ", hitboxes[4].edge_angle[2], 500, 220, 20);
            print_text("Game state: ", game_state, 500,200,20);

            /*
            char str[100];
            float v_x = col_res;
            sprintf(str, "%f", v_x);
            char to_print[100];
            strcpy(to_print, "collision: ");
            strcat(to_print, str);
            DrawText(to_print, 10, 400, 20, DARKGRAY);
            */
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