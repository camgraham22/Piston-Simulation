#include "physics.cpp"
#include "throttle.cpp"
#include <string.h>
#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"


int main() {

    Vector2 size = { 100, 100 };
    float weight = 1;
    float distance = 300;
    bool combustionInProgress = false;
    Piston piston{ size, weight, distance };
    float acceleration = 0.0f;
    Vector2 previousPistonPosition = piston.getPistonPosition();
    Vector2 previousCrankpinPosition = piston.getCrankpinPosition();
    previousCrankpinPosition.x -= 1;
    Vector2 crankshaftPosition = piston.getCrankshaftPosition();
    bool combusting = false;

    Vector2 throttleSliderPositon = { 300, 150 };
    Vector2 throttleSliderSize = { 400, 5 };
    Vector2 currentThrottlePosition = { throttleSliderPositon.x, throttleSliderPositon.y + throttleSliderSize.y / 2 };
    float throttleSliderXBoundary = throttleSliderPositon.x + throttleSliderSize.x; 
    float throttleSliderYBoundary = throttleSliderPositon.y + throttleSliderSize.y; 


    int targetFPS = 60;
    InitWindow(1920, 1080, "Piston Simulation");
    SetTargetFPS(targetFPS);

    float timePassed = 0;

    while (!WindowShouldClose()) {

        timePassed += GetFrameTime();

        Vector2 pistonPosition = piston.getPistonPosition();
        Vector2 crankpinPosition = piston.getCrankpinPosition();

        Vector2 originalCrankpinPos = crankpinPosition;
        float travelAngle = 0.0f;

        for (int i = 0; i < 16; i++) {
 
            pistonPosition = piston.getPistonPosition();
            crankpinPosition = piston.getCrankpinPosition();

            combusting = crankpinPosition.y < crankshaftPosition.y && crankpinPosition.x > crankshaftPosition.x + 1.5f;

            if (combusting) {
                float force = 1000000.0f;
                force *= ( 1 / (3 *  ((crankpinPosition.x - crankshaftPosition.x) / piston.crankpinRadius)));
                piston.combustion(force, acceleration);
            }
            piston.updatePosition(previousPistonPosition, previousCrankpinPosition, acceleration);
            acceleration = 0;

            Vector2 relativeOld = { previousCrankpinPosition.x - crankshaftPosition.x, previousCrankpinPosition.y - crankshaftPosition.y };
            Vector2 relativeNew = { crankpinPosition.x - crankshaftPosition.x, crankpinPosition.y - crankshaftPosition.y };
            float lengthOld = sqrt(relativeOld.x * relativeOld.x + relativeOld.y * relativeOld.y);
            float lengthNew = sqrt(relativeNew.x * relativeNew.x + relativeNew.y * relativeNew.y);
            travelAngle += acosf((relativeOld.x * relativeNew.x + relativeOld.y * relativeNew.y) / ( 50 * 50 ));

            previousPistonPosition = pistonPosition;
            previousCrankpinPosition = crankpinPosition;
        }

        pistonPosition = piston.getPistonPosition();
        crankpinPosition = piston.getCrankpinPosition();

        int RPM = (travelAngle / ( 2.0f * PI)) / (1.0f / 60.0f) * 60;
        char rpmBuffer[7];
        std::snprintf(rpmBuffer, sizeof(rpmBuffer), "%d", RPM);

        BeginDrawing();

        ClearBackground(WHITE);
        if (crankpinPosition.y < crankshaftPosition.y - 48 && crankpinPosition.x > crankshaftPosition.x + 2) {
            combusting = true;
            float force = 200000.0f * (1.0f - (( throttleSliderXBoundary - currentThrottlePosition.x ) / ( throttleSliderXBoundary - throttleSliderPositon.x )));
            piston.combustion(force, acceleration);
            timePassed = 0;
        }
        else {
            combusting = false;
        }
        throttlePosition(currentThrottlePosition, throttleSliderPositon, throttleSliderSize);
        float explosionRadius = 50.0f * (1.5f - (( throttleSliderXBoundary - currentThrottlePosition.x ) / ( throttleSliderXBoundary - throttleSliderPositon.x )));

        DrawText(rpmBuffer, 0, 0, 20, BLACK);
        DrawText(TextFormat("%d", RPM), 0, 0, 20, BLACK);   
    
        piston.draw(timePassed, combusting);

        EndDrawing();

    }

    CloseWindow();

}