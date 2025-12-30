#include "physics.cpp"
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

    int targetFPS = 144;
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
            piston.combustion(200000.0f, acceleration);
            timePassed = 0;
        }
        else {
            combusting = false;
        }
        piston.draw(timePassed, conrodLength, conrodRotation, combusting);

        EndDrawing();

    }

    CloseWindow();

}