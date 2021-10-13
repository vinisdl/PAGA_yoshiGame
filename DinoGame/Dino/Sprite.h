#pragma once
#include "Object.h"
class Sprite :
    public Object
{
public:
    Sprite() {}
    ~Sprite() {}

    void setSpritesheet(GLuint texID, int nAnimations, int nFrames);
    inline void setAnimation(int iAnimation) { this->iAnimation = iAnimation; }
    inline void setFrame(int iFrame) { this->iFrame = iFrame; }
    void update();

    bool onScreen();

    void Draw();

    void removeScreen();

    inline bool getActionActive() { return this->actionActive; }
    inline void setActionActive(bool actionActive) { this->actionActive = actionActive; }


protected:

    bool _onScreen;
    bool actionActive;
    int nAnimations, nFrames;
    int iAnimation, iFrame;
    float dx, dy;
};

