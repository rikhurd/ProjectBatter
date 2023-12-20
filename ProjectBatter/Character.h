#include "raylib.h"

class Character {
    public:
        Vector3 characterLocation = {0,0,0};
        Vector3 characterSize = { 1,1,1 };
        Mesh characterMesh;
        Model characterModel;
        BoundingBox characterBoundingBox;
        float characterSpeed = 6.0f;

        Color characterPHColor = GRAY;

        Character(const Vector3& characterSpawnLocation = { 0.0f, 0.0f, 0.0f });
        virtual void UpdateCharacter();
        virtual void DrawCharacter();

        // PH Set function for testing BoundingBox overlapping
        void SetColor(Color setColor);
};

// PlayerCharacter
//--------------------------------------------------------------------------------------

class PlayerCharacter : public Character {
    public:
        PlayerCharacter(const Vector3& characterSpawnLocation);
        // Do things like input here
        void UpdateCharacter() override;
};

// EnemyCharacter
//--------------------------------------------------------------------------------------

class EnemyCharacter : public Character {
    public:
        EnemyCharacter(const Vector3& characterSpawnLocation);
};