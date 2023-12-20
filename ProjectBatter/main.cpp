// ProjectBatter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include "Character.h"

// Structure definition
typedef enum GameState { TITLE, GAME } GameState;

// 
typedef struct Level {
    Mesh levelMesh;
    Model levelModel;
    BoundingBox levelBoundingBox;
    Vector3 spawnPointPlayer = { 0,0,0 };
    Vector3 spawnPointEnemy = { 0,0,0 };
    Vector3 levelPosition = { -8.0f, 0.0f, -8.0f };

} Level;

struct LevelSpawnColors {
    /*
    Get colors of spawning points
        Player is pure Green
        Enemy is pure Red

        Color variables are: Red, Green, Blue, Alpha (Transparency | 255 = Non-transparent, 0 = Full transparency)
    */

    const Color playerSpawnPointColor = { 0,255,0,255 };
    const Color enemySpawnPointColor = { 255,0,0,255 };

    // raylib has set colors for RED and GREEN, but they are not as easily set in third party tool than max green or red values.
};

// Function declaration
static Level GetLevelData(Image heightMap, Color playerSpawnPointColor, Color enemySpawnPointColor);

static Camera SwitchCameraType(GameState currentGameState, Vector3 cameraTargetPos = { 0.0f, 0.0f, 0.0f });

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    // Game screen size
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib Batter Project");

    GameState currentGameState = TITLE;

    /* 
    *   Define the camera to look into our 3d world.
    *   Camera is defined by GameState.
            Title   - is rotating camera to show the level
            Game    - follows player character
    */
    Camera camera = SwitchCameraType(currentGameState);

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

    UnloadImage(heightMap);     // Unload cubesmap image from RAM, already uploaded to VRAM

    std::unique_ptr<PlayerCharacter> player = std::make_unique<PlayerCharacter>(levelData.spawnPointPlayer);
    std::unique_ptr<EnemyCharacter> enemy = std::make_unique<EnemyCharacter>(levelData.spawnPointEnemy);

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {         // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------

        switch (currentGameState) {
            case TITLE: {
                UpdateCamera(&camera, CAMERA_ORBITAL);

                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentGameState = GAME;

                    // Set new camera variables here so it would be called only once and not updated each frame
                    camera = SwitchCameraType(currentGameState, player->characterLocation);
                    DisableCursor();
                }
            } break;
            case GAME: {
                UpdateCamera(&camera, CAMERA_THIRD_PERSON);
                player->UpdateCharacter();

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
            DrawBoundingBox(levelData.levelBoundingBox, RED);

            player->DrawCharacter();
            enemy->DrawCharacter();


        EndMode3D();

        switch (currentGameState) {
            case TITLE: {

                DrawText("PRESS ENTER", GetScreenWidth() / 3, GetScreenHeight() / 4, 32, MAROON);
            } break;
            case GAME:
            {
                if (CheckCollisionBoxes(levelData.levelBoundingBox, player->characterBoundingBox)) player->SetColor(RED);
                else  player->SetColor(GREEN);
            } break;
            default: break;
        }



        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);     // Unload map texture
    UnloadModel(levelData.levelModel);         // Unload map model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

Camera SwitchCameraType(GameState currentGameState, Vector3 cameraTargetPos) {
    Camera tempCamera = { 0 };

    switch (currentGameState) {
        case TITLE: {
            tempCamera.position = { 16.0f, 16.0f, 16.0f };     // Camera position
            tempCamera.target = { 0.0f, 0.0f, 0.0f };          // Camera looking at point
            tempCamera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
            tempCamera.fovy = 45.0f;                                    // Camera field-of-view Y
            //tempCamera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

        } break;
        case GAME: {
            tempCamera.position = { 8.0f, 16.0f, 0 };     // Camera position
            tempCamera.target = cameraTargetPos;          // Camera looking at point
            tempCamera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
            tempCamera.fovy = 45.0f;                                    // Camera field-of-view Y
            //tempCamera.projection = CAMERA_THIRD_PERSON;                 // Camera projection type

        } break;
        default: break;
    }
    return tempCamera;
}



/*
Gets given height maps spawn points and recolors the map for cubic mesh generation
*/
static Level GetLevelData(Image heightMap, Color playerSpawnPointColor, Color enemySpawnPointColor) {
    Level tempLevelData;
    Vector3 levelCubeSize = {1.0f,1.0f,1.0f};

    // Check spawning positions in the level
    if ((tempLevelData.spawnPointPlayer.x == 0 && tempLevelData.spawnPointPlayer.y == 0) && (tempLevelData.spawnPointEnemy.x == 0 && tempLevelData.spawnPointEnemy.y == 0)) {
        for (int x = 0; x < heightMap.width; x++)
            for (int y = 0; y < heightMap.height; y++) {
                Color pixelColor = GetImageColor(heightMap, x, y);

                if (ColorToInt(pixelColor) == ColorToInt(playerSpawnPointColor)) {
                    // Found a spawning point, calculate its 3D position with level position. Store its position. Casting int into float

                    tempLevelData.spawnPointPlayer = {
                        (float(x) + tempLevelData.levelPosition.x),
                        (tempLevelData.levelPosition.y),
                        (float(y) + tempLevelData.levelPosition.z)
                    };

                    // When spawn point found recolor image back into BLACK so mesh generation later will find the spawn point as something the player can stand on. If the pixel is not Black or White the ground the mesh will have hole in the position where the spawn point is.
                    ImageDrawPixel(&heightMap, x, y, BLACK);
                }
                else if (ColorToInt(pixelColor) == ColorToInt(enemySpawnPointColor)) {
                    tempLevelData.spawnPointEnemy = {
                        (float(x) + tempLevelData.levelPosition.x),
                        (tempLevelData.levelPosition.y),
                        (float(y) + tempLevelData.levelPosition.z)
                    };

                    ImageDrawPixel(&heightMap, x, y, BLACK);
                }
            }
        // TODO: Create error code here when a loaded map doesn't have spawn points for player or enemy.
    }

    tempLevelData.levelMesh = GenMeshCubicmap(heightMap, { 1.0f, 1.0f, 1.0f });
    tempLevelData.levelModel = LoadModelFromMesh(tempLevelData.levelMesh);
    tempLevelData.levelBoundingBox = BoundingBox{
        Vector3 {
        tempLevelData.levelPosition.x - (levelCubeSize.x / 2),
        0,
        tempLevelData.levelPosition.z - (levelCubeSize.z / 2)
        },
        Vector3 {
        tempLevelData.levelPosition.x + (heightMap.height) - (levelCubeSize.x / 2),
        levelCubeSize.y,
        tempLevelData.levelPosition.z + (heightMap.width) - (levelCubeSize.z / 2)
        } };

    return tempLevelData;
}