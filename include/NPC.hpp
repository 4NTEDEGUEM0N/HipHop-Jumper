#ifndef NPC_HPP
#define NPC_HPP
#include "Component.hpp"
#include "GameObject.hpp"

using namespace std;

class NPC : public Component {
private:
    string name;
    string spriteFile;

public:
    NPC(GameObject &associated, string file, string name, int frameCountW, int frameCountH);
    ~NPC();
    void Update(float dt);
    void Render();
    bool Is(string type);
    void NotifyCollision(GameObject &other);

    void RepeatDialog();
    vector<string> dialogs;
    bool hasDialog;
};

#endif //NPC_HPP
