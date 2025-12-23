#include "physics.cpp"
#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"


int main() {

    Vector2 size = { 100, 100 };
    float weight = 1.32;
    float distance = 300;
    int RPM = 100;
    bool combustionInProgress = false;
    Piston piston{ size, weight, distance, RPM };
    float pistonTopDeadCenter = piston.getTopDeadCenter();
    float acceleration = 0.0f;
    Vector2 previousPistonPosition = piston.getPistonPosition();
    Vector2 previousCrankpinPosition = piston.getCrankpinPosition();

    int targetFPS = 30;
    InitWindow(1920, 1080, "Piston Simulation");
    SetTargetFPS(targetFPS);

    float timePassed = 0;

    while (!WindowShouldClose()) {
    
        timePassed += GetFrameTime();

        Vector2 pistonPostion = piston.getPistonPosition();
        Vector2 crankpinPostion = piston.getCrankpinPosition();
        Vector2 conrodVector = {pistonPostion.x - crankpinPostion.x, pistonPostion.y - crankpinPostion.y};
        float conrodLength = sqrt(conrodVector.x * conrodVector.x + conrodVector.y * conrodVector.y);
        float conrodRotation = atan2(conrodVector.y, conrodVector.x);
        piston.updatePosition(previousPistonPosition, previousCrankpinPosition, conrodVector, acceleration, conrodLength);
        
        previousPistonPosition = pistonPostion;
        previousCrankpinPosition = crankpinPostion;

        BeginDrawing();

        ClearBackground(WHITE);
        if (pistonPostion.y == pistonTopDeadCenter || combustionInProgress) {
            // cout << "\nKaboom!!";
            combustionInProgress = true;
            if (pistonPostion.y == pistonTopDeadCenter || (pistonPostion.y < pistonTopDeadCenter && pistonPostion.y > pistonTopDeadCenter - 100)) {
                cout << "\nFIRE!!!!";
                piston.combustion(10.800, acceleration);
            }
            else {
                combustionInProgress = false;
            }
            timePassed = 0;
        }
        piston.draw(timePassed, conrodLength, conrodRotation);

        EndDrawing();

    }

    CloseWindow();

}