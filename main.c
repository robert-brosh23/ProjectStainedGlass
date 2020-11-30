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
#include "raylib.h"

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");

    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 8.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    
    SetCameraMode(camera, CAMERA_ORBITAL);

    Vector3 cubePosition = { 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    bool pause = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);
        //----------------------------------------------------------------------------------

        if (IsKeyPressed(KEY_SPACE)){
            pause = !pause;
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                if(pause==0){
                    DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                }
                if(pause==1){
                    DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, BLUE);
                }
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawText("This is a raylib sd", 10, 40, 20, DARKGRAY);

            if(pause==0){
                DrawText("Pause: 0", 10, 60, 20, DARKGRAY);
            }
            else{
                DrawText("Pause: 1", 10, 60, 20, DARKGRAY);
            }

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}