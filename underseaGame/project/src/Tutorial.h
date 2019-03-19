#pragma once


class Game;

class Tutorial {

public:
    void render(const Game& game) const;
    const float tutorialEndDate = 10.f; // in seconds
    const float animationInterval = 2.f; 

private:
    const float tutorialBeginDate = 1.f; 
};

