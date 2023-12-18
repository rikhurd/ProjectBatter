#include "character.h"

Character::Character(const Vector3& characterSpawnLocation) {
    characterLocation = characterSpawnLocation;
    characterSpeed = 5.0f;
}

void Character::UpdateCharacter() {

}

void Character::DrawCharacter() {

    // Draw the character at its position
    //DrawModel(this->model, this->location, 1.0f, BLUE);
    DrawCube(characterLocation, 1, 1, 1, GREEN);

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

}

// EnemyCharacter
//--------------------------------------------------------------------------------------

EnemyCharacter::EnemyCharacter(const Vector3& characterSpawnLocation) : Character(characterSpawnLocation) {
    this->characterPHColor = RED;

}