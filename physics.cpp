#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"
#include "physics.h"
#include <cmath>
#include <iostream>

using std::abs, std::cout, std::fmod;

const float GRAVITY = 9.8f;

Piston::Piston(Vector2 size, float weight, float distance) {
    pistonSize = size;
    crankshaftPosition = { 960, 720 };
    crankpinPosition = { crankshaftPosition.x, crankshaftPosition.y - crankpinRadius };
    pistonPosition = { crankpinPosition.x - pistonSize.x / 2, crankpinPosition.y - actualConrodLength};
    pistonWeight = weight;
    maxTravelDistance = distance;
    acceleration = 0;
}

void Piston::updatePosition(Vector2 previousPistonPosition, Vector2 previousCrankpinPosition, float &acceleration) {

    pistonPosition.y = pistonPosition.y + ( pistonPosition.y - previousPistonPosition.y ) + (acceleration * (( 1.0f / 1000.0f ) * ( 1.0f / 1000.0f )));
    crankpinPosition.x = crankpinPosition.x + ( crankpinPosition.x - previousCrankpinPosition.x ); 
    crankpinPosition.y = crankpinPosition.y + ( crankpinPosition.y - previousCrankpinPosition.y );

    Vector2 conrodVector = { pistonPosition.x - crankpinPosition.x, pistonPosition.y - crankpinPosition.y };
    float conrodLength = sqrt(conrodVector.x * conrodVector.x + conrodVector.y * conrodVector.y);

    for (int i = 0; i < 8; i++) {
        conrodVector.x = pistonPosition.x - crankpinPosition.x + pistonSize.x / 2;
        conrodVector.y = pistonPosition.y - crankpinPosition.y;

        conrodLength = sqrt(conrodVector.x * conrodVector.x + conrodVector.y * conrodVector.y);
        conrodRotation = 90 + atan2(conrodVector.y, conrodVector.x) * 180 / PI;
        float dx = crankpinPosition.x - crankshaftPosition.x;
        float dy = crankpinPosition.y - crankshaftPosition.y;
        float currentRadius = sqrt( dx * dx + dy * dy);
        float radiusError = crankpinRadius / currentRadius;
        crankpinPosition.x = crankshaftPosition.x + dx * radiusError;
        crankpinPosition.y = crankshaftPosition.y + dy * radiusError;  

        float lengthError = actualConrodLength - conrodLength;
        Vector2 unitVector = { conrodVector.x / conrodLength, conrodVector.y / conrodLength };
        unitVector.x = unitVector.x * lengthError;
        unitVector.y = unitVector.y * lengthError;
        pistonPosition.y += unitVector.y * .95f;
        crankpinPosition.x -= unitVector.x * .05f;
        crankpinPosition.y -= unitVector.y * .05f;

        dx = crankpinPosition.x - crankshaftPosition.x;
        dy = crankpinPosition.y - crankshaftPosition.y;
        currentRadius = sqrt( dx * dx + dy * dy);
        radiusError = crankpinRadius / currentRadius;
        crankpinPosition.x = crankshaftPosition.x + dx * radiusError;
        crankpinPosition.y = crankshaftPosition.y + dy * radiusError; 
    }

}

void Piston::combustion(float force, float &acceleration) {
    acceleration = force / pistonWeight;
}

void Piston::draw(float timePassed, bool combusting) {
    if (combusting) {
        DrawCircle(pistonPosition.x + pistonSize.x / 2, crankshaftPosition.y - actualConrodLength - pistonSize.y - 20, 50, RED);
    }
    DrawRectangleV({ pistonPosition.x, pistonPosition.y - pistonSize.y + 20}, pistonSize, GRAY);
    Rectangle conrod = { pistonPosition.x + pistonSize.x / 2, pistonPosition.y, 20, actualConrodLength };
    DrawRectanglePro(conrod, {10,0}, conrodRotation, GRAY);
    DrawCircle(crankshaftPosition.x, crankshaftPosition.y, crankshaftRadius, GRAY);
    DrawCircle(crankpinPosition.x, crankpinPosition.y, 20, BLACK);
}

Vector2 Piston::getPistonPosition() {
    return pistonPosition;
}

Vector2 Piston::getCrankpinPosition() {
    return crankpinPosition;
}

Vector2 Piston::getCrankshaftPosition() {
    return crankshaftPosition;
}