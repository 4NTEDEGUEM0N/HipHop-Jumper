#include "../include/NPC.hpp"

#include "../include/Collider.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/DialogState.hpp"

NPC::NPC(GameObject &associated, string file, string name):Component(associated) {
    SpriteRenderer* npc = new SpriteRenderer(associated, file);
    associated.AddComponent(npc);

    hasDialog = false;
    spriteFile = file;
    this->name = name;
}

NPC::~NPC() = default;

void NPC::Update(float dt) {

}

void NPC::Render() {

}

void NPC::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Character") != nullptr) {
        if (hasDialog) return;
        Game& game = Game::GetInstance();
        DialogState* dialogState = new DialogState(dialogs, spriteFile, name);
        game.Push(dialogState);
        hasDialog = true;
    }
}

bool NPC::Is(string type) {
    return type == "NPC";
}

void NPC::RepeatDialog() {
    hasDialog = false;
}





