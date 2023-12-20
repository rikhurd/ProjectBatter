#include "character.h"

Character::Character(const Vector3& characterSpawnLocation) {
    characterLocation = characterSpawnLocation;

    characterModel = LoadModelFromMesh(GenMeshCube(1,1,1));
}

void Character::UpdateCharacter() {

}

void Character::SetColor(Color setColor) {
    characterPHColor = setColor;
}

void Character::DrawCharacter() {

    // Draw the character at its position
    //DrawModel(this->model, this->location, 1.0f, BLUE);
    DrawCube(characterLocation, 1, 1, 1, characterPHColor);
    DrawBoundingBox(characterBoundingBox,RED);

}

// PlayerCharacter
//--------------------------------------------------------------------------------------

PlayerCharacter::PlayerCharacter(const Vector3& characterSpawnLocation) : Character(characterSpawnLocation) {
    characterPHColor = GREEN;

}

void PlayerCharacter::UpdateCharacter() {
    float characterSpeedFrame = characterSpeed * GetFrameTime();

    if (IsKeyDown(KEY_W)) characterLocation.x -= characterSpeedFrame;
    if (IsKeyDown(KEY_S))characterLocation.x += characterSpeedFrame;
    if (IsKeyDown(KEY_A)) characterLocation.z += characterSpeedFrame;
    if (IsKeyDown(KEY_D))characterLocation.z -= characterSpeedFrame;

    // Calculate BoundingBox with BoundingBox min Vector3 and max Vector3
    characterBoundingBox = BoundingBox{
        Vector3 {
        characterLocation.x - characterSize.x / 2,
        characterLocation.y - characterSize.y / 2,
        characterLocation.z - characterSize.z / 2
        },
        Vector3 {
        characterLocation.x + characterSize.x / 2,
        characterLocation.y + characterSize.y / 2,
        characterLocation.z + characterSize.z / 2
        } };
}

// EnemyCharacter
//--------------------------------------------------------------------------------------

EnemyCharacter::EnemyCharacter(const Vector3& characterSpawnLocation) : Character(characterSpawnLocation) {
    this->characterPHColor = RED;

}