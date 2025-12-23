#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"
#include "physics.h"
#include <cmath>
#include <iostream>

using std::abs, std::cout, std::fmod;

const float GRAVITY = 9.8f;

Piston::Piston(Vector2 size, float weight, float distance, int RPM) {
    topDeadCenter = { 960, 200 };
    pistonPosition = topDeadCenter;
    velocity = { 0, 0 };
    pistonSize = size;
    crankpinPosition = { topDeadCenter.x + pistonSize.x / 2, topDeadCenter.y + 270 };
    realConrodLength =  274.591;
    pistonWeight = weight;
    maxTravelDistance = distance;
    engineRPM = RPM;
}

void Piston::updatePosition(Vector2 previousPistonPosition, Vector2 previousCrankpinPosition, Vector2 conrodVector, float &acceleration, float conrodLength) {

    // velocity.y += GRAVITY * GetFrameTime();

    pistonPosition.y = pistonPosition.y + ( pistonPosition.y - previousPistonPosition.y ) + (acceleration * ( GetFrameTime() * GetFrameTime()));
    crankpinPosition.x = crankpinPosition.x + ( crankpinPosition.x - previousCrankpinPosition.x ) + (acceleration * ( GetFrameTime() * GetFrameTime())); 
    crankpinPosition.y = crankpinPosition.y + ( crankpinPosition.y - previousCrankpinPosition.y ) + (acceleration * ( GetFrameTime() * GetFrameTime()));

     float dx = crankpinPosition.x - topDeadCenter.x + pistonSize.x / 2;
    // cout << "\nDx: " << dx;
    float dy = crankpinPosition.y - topDeadCenter.y + 300;
    // cout << "\nDy: " << dy;
    float currentRadius = sqrt( dx * dx + dy * dy);

    float radiusError = 90.0f / currentRadius;
    // cout << "\nRadius Error: " << radiusError;
    crankpinPosition = {( topDeadCenter.x + pistonSize.x / 2 ) + dx * radiusError, ( topDeadCenter.y + 300 ) + dy * radiusError };

    float lengthError = realConrodLength - conrodLength;
    Vector2 unitVector = { conrodVector.x / conrodLength, conrodVector.y / conrodLength };
    unitVector = { unitVector.x * lengthError, unitVector.y * lengthError };
    pistonPosition.x += unitVector.x * .97f;
    pistonPosition.y += unitVector.y * .97f;
    crankpinPosition.x -= unitVector.x * .03f;
    crankpinPosition.y -= unitVector.y * .03f;

    pistonPosition.x = topDeadCenter.x + (pistonSize.x / 2);

}

void Piston::combustion(float force, float &acceleration) {
    acceleration = force / pistonWeight;

    velocity.y += acceleration * GetFrameTime();
}

void Piston::draw(float timePassed, float conrodLength, float conrodRotation) {
    DrawRectangleV(pistonPosition, pistonSize, GRAY);
    Rectangle conrod = { pistonPosition.x + pistonSize.x / 2 - 10, pistonPosition.y, 20, realConrodLength };
    DrawRectanglePro(conrod, {0,0} , conrodRotation, GRAY);
    DrawCircle(topDeadCenter.x + pistonSize.x / 2, topDeadCenter.y + 300, 60, GRAY);
    DrawCircle(crankpinPosition.x, crankpinPosition.y, 20, BLACK);
}

Vector2 Piston::getPistonPosition() {
    return pistonPosition;
}

Vector2 Piston::getCrankpinPosition() {
    return crankpinPosition;
}

float Piston::getTopDeadCenter() {
    return topDeadCenter.y;
}