#ifndef PHYSICS_H
#define PHYSICS_H

#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"

class Piston {
    private:
        Vector2 pistonPosition;
        Vector2 pistonSize;
        Vector2 crankshaftPosition;
        Vector2 crankpinPosition;
        float conrodLength;
        float conrodRotation;
        float pistonWeight;
        float maxTravelDistance;
        int engineRPM;
    
    public:
        const float actualConrodLength = 200;
        const int crankshaftRadius = 70;
        const int crankpinRadius = 50;

        Piston(Vector2 size, float weight, float distance);
        void updatePosition(Vector2 pistonPosition, Vector2 crankpinPostion, float &acceleration);
        void combustion(float force, float &acceleration);
        void draw(float time, bool combusting);
        Vector2 getPistonPosition();
        Vector2 getCrankpinPosition();
        Vector2 getCrankshaftPosition();

};

#endif