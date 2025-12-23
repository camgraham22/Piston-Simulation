#ifndef PHYSICS_H
#define PHYSICS_H

#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"

class Piston {
    private:
        Vector2 topDeadCenter;
        Vector2 pistonPosition;
        Vector2 velocity;
        Vector2 pistonSize;
        Vector2 crankpinPosition;
        float realConrodLength;
        float pistonWeight;
        float maxTravelDistance;
        int engineRPM;
    
    public:
        Piston(Vector2 size, float weight, float distance, int RPM);
        void updatePosition(Vector2 pistonPosition, Vector2 crankpinPostion, Vector2 conrodVector, float &acceleration, float length);
        void combustion(float force, float &acceleration);
        void draw(float time, float length, float rotation);
        Vector2 getPistonPosition();
        Vector2 getCrankpinPosition();
        float getTopDeadCenter();

};

#endif