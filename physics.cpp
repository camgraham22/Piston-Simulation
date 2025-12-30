#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"
#include "physics.h"
#include <cmath>
#include <iostream>

using std::abs, std::cout, std::fmod;

const float GRAVITY = 9.8f;

Piston::Piston(Vector2 size, float weight, float distance, int RPM) {
    velocity = { 0, 0 };
    pistonSize = size;
    crankshaftPosition = { 960, 720 };
    crankpinPosition = { crankshaftPosition.x, crankshaftPosition.y - 50 };
    realConrodLength =  200;
    pistonPosition = { crankpinPosition.x - pistonSize.x / 2, crankpinPosition.y - realConrodLength};
    pistonWeight = weight;
    maxTravelDistance = distance;
    engineRPM = RPM;
}

void Piston::updatePosition(Vector2 previousPistonPosition, Vector2 previousCrankpinPosition, Vector2 conrodVector, float &acceleration, float &conrodLength, float &conrodRotation) {

    // velocity.y += GRAVITY * GetFrameTime();

    pistonPosition.y = pistonPosition.y + ( pistonPosition.y - previousPistonPosition.y ) + (acceleration * ( GetFrameTime() * GetFrameTime()));
    // cout << "\nCrank before: " << crankpinPosition.x << ", " << crankpinPosition.y;
    crankpinPosition.x = crankpinPosition.x + ( crankpinPosition.x - previousCrankpinPosition.x ) /*+ (acceleration * ( GetFrameTime() * GetFrameTime()))*/; 
    crankpinPosition.y = crankpinPosition.y + ( crankpinPosition.y - previousCrankpinPosition.y )/* + (acceleration * ( GetFrameTime() * GetFrameTime()))*/;

    for (int i = 0; i < 5; i++) {
        conrodVector.x = pistonPosition.x - crankpinPosition.x + 50;
        conrodVector.y = pistonPosition.y - crankpinPosition.y;
        conrodLength = sqrt(conrodVector.x * conrodVector.x + conrodVector.y * conrodVector.y);
        conrodRotation = 90 + atan2(conrodVector.y, conrodVector.x) * 180 / PI;
        float dx = crankpinPosition.x - crankshaftPosition.x;
        float dy = crankpinPosition.y - crankshaftPosition.y;
        float currentRadius = sqrt( dx * dx + dy * dy);
        float radiusError = 50.0f / currentRadius;
        // cout << ", Radius Error: " << radiusError;
        crankpinPosition.x = crankshaftPosition.x + dx * radiusError;
        crankpinPosition.y = crankshaftPosition.y + dy * radiusError;   
        float lengthError = realConrodLength - conrodLength;
        // cout << "\nError: " << lengthError;
        Vector2 unitVector = { conrodVector.x / conrodLength, conrodVector.y / conrodLength };
        // cout << "\nVector before: { " << unitVector.x << ", " << unitVector.y << " }";
        unitVector.x = unitVector.x * lengthError;
        unitVector.y = unitVector.y * lengthError;
        // cout << "\nVector after: { " << unitVector.x << ", " << unitVector.y << " }";
        pistonPosition.y += unitVector.y * .95f;
        // cout << "\nOld crank: " << crankpinPosition.x << ", " << crankpinPosition.y;
        crankpinPosition.x -= unitVector.x * .05f;
        crankpinPosition.y -= unitVector.y * .05f;
        // cout << "\nNew crank: " << crankpinPosition.x << ", " << crankpinPosition.y;
    }

}

void Piston::combustion(float force, float &acceleration) {
    acceleration += force / pistonWeight;

}

void Piston::draw(float timePassed, float conrodLength, float conrodRotation, bool combusting) {
    if (combusting) {
        DrawCircle(pistonPosition.x + pistonSize.x / 2, crankshaftPosition.y - realConrodLength - pistonSize.y - 20, 50, RED);
    }
    DrawRectangleV({ pistonPosition.x, pistonPosition.y - pistonSize.y + 20}, pistonSize, GRAY);
    Rectangle conrod = { pistonPosition.x + pistonSize.x / 2, pistonPosition.y, 20, realConrodLength };
    DrawRectanglePro(conrod, {10,0}, conrodRotation, GRAY);
    DrawCircle(crankshaftPosition.x, crankshaftPosition.y, 70, GRAY);
    // cout << "\nPos: " << crankpinPosition.x << ", " << crankpinPosition.y;
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