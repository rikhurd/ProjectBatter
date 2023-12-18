#include "raylib.h"

class Character {
    public:
        Vector3 characterLocation = {0,0,0};
        Mesh mesh;
        Model model;
        float characterSpeed = 5.0f;

        Color characterPHColor = GRAY;

        Character(const Vector3& characterSpawnLocation = { 0.0f, 0.0f, 0.0f });
        virtual void UpdateCharacter();
        virtual void DrawCharacter();
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