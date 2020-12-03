/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <stdio.h>
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
    camera.zoom = 1;
    //camera.type = CAMERA_PERSPECTIVE;
    
    //SetCameraMode(camera, CAMERA_ORBITAL);

    //Vector3 cubePosition = { 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    location starting_loc;
    location current_loc;
    current_loc.x = 200;
    current_loc.y = 200;
    current_loc.speed = 20;
    current_loc.dir = convert_deg_to_rad(80);
    starting_loc = current_loc;
    int frame_count = 0;
    //int overlap = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //UpdateCamera(&camera);
        //----------------------------------------------------------------------------------

        if(frame_count%2==0){
            //Update line location
            current_loc = update_location(current_loc);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                DrawRectangle(100, 100, 300, 300, BLUE);
                
                DrawLineEx((Vector2){ starting_loc.x, starting_loc.y }, (Vector2){ current_loc.x, current_loc.y }, 3, RED);
                
                //DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
                //DrawGrid(10, 1.0f);

            EndMode2D();
            
            DrawText("This is a raylib asdfdsf", 10, 40, 20, DARKGRAY);
            char str[10];
            float v_x = cos((float)current_loc.dir)*(float)current_loc.speed;
            sprintf(str, "%f", v_x);
            char to_print[100];
            strcpy(to_print, "Velocity_x: ");
            strcat(to_print, str);
            DrawText(to_print, 10, 400, 20, DARKGRAY);
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