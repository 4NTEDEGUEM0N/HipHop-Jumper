#ifndef NPC_HPP
#define NPC_HPP
#include "Component.hpp"
#include "GameObject.hpp"

using namespace std;

class NPC : public Component {
private:
    string name;
    bool hasDialog;
    string spriteFile;

public:
    NPC(GameObject& associated, string file, string name);
    ~NPC();
    void Update(float dt);
    void Render();
    bool Is(string type);
    void NotifyCollision(GameObject &other);

    void RepeatDialog();
    vector<string> dialogs;
};

#endif //NPC_HPP
