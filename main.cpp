#include "physics.cpp"
#include "throttle.cpp"
#include <string.h>
#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"


int main() {

    Vector2 size = { 100, 100 };
    float weight = 1.32;
    float distance = 300;
    int RPM = 100;
    bool combustionInProgress = false;
    Piston piston{ size, weight, distance, RPM };
    float acceleration = 0.0f;
    Vector2 previousPistonPosition = piston.getPistonPosition();
    Vector2 previousCrankpinPosition = piston.getCrankpinPosition();
    previousCrankpinPosition.x -= 5;
    Vector2 crankshaftPosition = piston.getCrankshaftPosition();
    bool combusting = false;

    Vector2 throttleSliderPositon = { 300, 150 };
    Vector2 throttleSliderSize = { 400, 5 };
    Vector2 currentThrottlePosition = { throttleSliderPositon.x, throttleSliderPositon.y + throttleSliderSize.y / 2 };
    float throttleSliderXBoundary = throttleSliderPositon.x + throttleSliderSize.x; 
    float throttleSliderYBoundary = throttleSliderPositon.y + throttleSliderSize.y; 


    int targetFPS = 1000;
    InitWindow(1920, 1080, "Piston Simulation");
    SetTargetFPS(targetFPS);

    float timePassed = 0;

    while (!WindowShouldClose()) {
    
        if (timePassed != 0) {
            acceleration = 0;
        }
        timePassed += GetFrameTime();

        Vector2 pistonPosition = piston.getPistonPosition();
        Vector2 crankpinPostion = piston.getCrankpinPosition();
        Vector2 conrodVector = { pistonPosition.x - crankpinPostion.x, pistonPosition.y - crankpinPostion.y };
        float conrodLength = sqrt(conrodVector.x * conrodVector.x + conrodVector.y * conrodVector.y);
        Vector2 crankTravel = { crankpinPostion.x - previousCrankpinPosition.x, crankpinPostion.y - previousCrankpinPosition.y };
        float crankTravelLength = sqrt(crankTravel.x * crankTravel.x + crankTravel.y * crankTravel.y);
        int RPM = 60 * (crankTravelLength / (2 * PI));
        std::string rpmstr = std::to_string(RPM);
        const char* rpmBuffer[10];
        memcpy(rpmBuffer, &rpmstr, sizeof(rpmstr));
        float conrodRotation;
        piston.updatePosition(previousPistonPosition, previousCrankpinPosition, conrodVector, acceleration, conrodLength, conrodRotation);

        
        previousPistonPosition = pistonPosition;
        previousCrankpinPosition = crankpinPostion;

        BeginDrawing();

        DrawText(*rpmBuffer, 0, 0, 20, BLACK);

        ClearBackground(WHITE);
        if (crankpinPostion.y < crankshaftPosition.y - 48 && crankpinPostion.x > crankshaftPosition.x + 2) {
            combusting = true;
            float force = 200000.0f * (1.0f - (( throttleSliderXBoundary - currentThrottlePosition.x ) / ( throttleSliderXBoundary - throttleSliderPositon.x )));
            cout << "\n" << (1.0f - (( throttleSliderXBoundary - currentThrottlePosition.x ) / ( throttleSliderXBoundary - throttleSliderPositon.x )));
            piston.combustion(force, acceleration);
            timePassed = 0;
        }
        else {
            combusting = false;
        }
        throttlePosition(currentThrottlePosition, throttleSliderPositon, throttleSliderSize);
        float explosionRadius = 50.0f * (1.5f - (( throttleSliderXBoundary - currentThrottlePosition.x ) / ( throttleSliderXBoundary - throttleSliderPositon.x )));
        piston.draw(timePassed, conrodLength, conrodRotation, combusting, explosionRadius);

        EndDrawing();

    }

    CloseWindow();

}