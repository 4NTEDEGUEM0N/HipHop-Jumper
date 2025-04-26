#ifndef TIMER_HPP
#define TIMER_HPP

using namespace std;

class Timer {
private:
    float time;

public:
    Timer();
    void Update(float dt);
    void Restart();
    float Get();
};

#endif //TIMER_HPP
