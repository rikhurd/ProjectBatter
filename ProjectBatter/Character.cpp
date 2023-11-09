#include "character.h"

Character::Character(const Vector3& characterSpawnLocation) {
    characterLocation = characterSpawnLocation;
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
 

}

void PlayerCharacter::UpdateCharacter() {
    /*
    if (IsKeyDown(KEY_W));

        if (IsKeyDown(KEY_S));
            if (IsKeyDown(KEY_A));
                if (IsKeyDown(KEY_D));
                */
}

// EnemyCharacter
//--------------------------------------------------------------------------------------
