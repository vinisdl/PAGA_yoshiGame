#pragma once
#include "Object.h"
class Layer :
    public Object
{
public:
    Layer() {}
    ~Layer() {}
    
    inline void setOffsetX(float offsetX) { this->offsetX = offsetX; }
    inline void setOffsetY(float offsetY) { this->offsetY = offsetY; }

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();


protected:
    float offsetX, offsetY;

};

