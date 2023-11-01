// ProjectBatter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

// Structure definition
typedef enum GameState { TITLE, GAME } GameState;

// 
struct Level
{
    Mesh levelMesh;
    Model levelModel;
    Vector2 spawnPointPlayer = { 0,0 };
    Vector2 spawnPointEnemy = { 0,0 };
    Vector3 levelPosition = { -8.0f, 0.0f, -8.0f };
};
struct LevelSpawnColors
{
    /*
    Get colors of spawning points
        Player is pure Green
        Enemy is pure Red

        Color variables are: Red, Green, Blue, Alpha (Transparency | 255 = Non-transparent, 0 = Full transparency)
    */

    Color playerSpawnPointColor = { 0,255,0,255 };
    Color enemySpawnPointColor = { 255,0,0,255 };

    // raylib has set colors for RED and GREEN, but they are not as easily set in third party tool than max green or red values.
};

// Function declaration
static Level GetLevelData(Image heightMap, Color playerSpawnPointColor, Color enemySpawnPointColor);
static void InitPlayerCharacter();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // Game screen size
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib Batter Project");

    GameState currentGameState = TITLE;

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = { 16.0f, 16.0f, 16.0f };     // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

    // Load image and turn it into Cubicmap
    Image heightMap = LoadImage("resources/levels/Level1T.png");      // Load image (RAM)

    LevelSpawnColors spawnColors;

    /*
    Gets level data and includes:
        - Mesh generated from cubicmap (heightMap)
        - Model generated from mesh.
        - Spawn points
            - Player spawn point is generated from green pixel in the level
            - Enemy spawn point is generated from red pixel in the level
        - Level's three dimensional position in the game
    */
    Level levelData = GetLevelData(heightMap, spawnColors.playerSpawnPointColor, spawnColors.enemySpawnPointColor);


    // By default each cube is mapped to one part of texture atlas
    Texture2D texture = LoadTexture("resources/textures/tileTexturePH.png");    // Load map texture
    levelData.levelModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;    // Set map diffuse texture

    // Define player variables here

    //Player location is from spawnPointPlayer
    Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
    Color playerColor = GREEN;

    UnloadImage(heightMap);     // Unload cubesmap image from RAM, already uploaded to VRAM

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        switch (currentGameState)
        {
            case TITLE:
            {
                UpdateCamera(&camera, CAMERA_ORBITAL);

                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentGameState = GAME;
                }
            } break;
            case GAME:
            {
                UpdateCamera(&camera, CAMERA_THIRD_PERSON);
                DisableCursor();

                InitPlayerCharacter();

            } break;
            default: break;
        }


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(camera);

            DrawModel(levelData.levelModel, levelData.levelPosition, 1.0f, WHITE);

        EndMode3D();

        DrawText("PRESS ENTER",  GetScreenWidth() / 3, GetScreenHeight() / 4, 32, MAROON);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // UnloadTexture(cubicmap);    // Unload cubicmap texture
    UnloadTexture(texture);     // Unload map texture
    UnloadModel(levelData.levelModel);         // Unload map model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

static void InitPlayerCharacter()
{
    
}

/*
Gets given height maps spawn points and recolors the map for cubic mesh generation
*/
static Level GetLevelData(Image heightMap, Color playerSpawnPointColor, Color enemySpawnPointColor)
{
    Level tempLevelData;

    // Check spawning positions in the level
    if ((tempLevelData.spawnPointPlayer.x == 0 && tempLevelData.spawnPointPlayer.y == 0) && (tempLevelData.spawnPointEnemy.x == 0 && tempLevelData.spawnPointEnemy.y == 0))
    {
        for (int x = 0; x < heightMap.width; x++)
        {
            for (int y = 0; y < heightMap.height; y++)
            {
                Color pixelColor = GetImageColor(heightMap, x, y);

                if (ColorToInt(pixelColor) == ColorToInt(playerSpawnPointColor))
                {
                    // Found a spawning point, store its position. Casting int into float
                    tempLevelData.spawnPointPlayer = { float(x),float(y) };

                    // When spawn point found recolor image back into BLACK so mesh generation later will find the spawn point as something the player can stand on.
                    ImageDrawPixel(&heightMap, x, y, BLACK);
                }
                else if (ColorToInt(pixelColor) == ColorToInt(enemySpawnPointColor))
                {
                    tempLevelData.spawnPointEnemy = { float(x),float(y) };
                    ImageDrawPixel(&heightMap, x, y, BLACK);
                }
            }
        }
        // TODO: Create error code here when a loaded map doesn't have spawn points for player and/or enemy.
    }

    tempLevelData.levelMesh = GenMeshCubicmap(heightMap, { 1.0f, 1.0f, 1.0f });
    tempLevelData.levelModel = LoadModelFromMesh(tempLevelData.levelMesh);

    return tempLevelData;
}