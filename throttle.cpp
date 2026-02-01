#include "./Includes/raylib-5.5_win64_mingw-w64/include/raylib.h"
#include <iostream>

using std::cout;

void throttlePosition(Vector2 &currentThrottlePosition, Vector2 throttleSliderPositon, Vector2 throttleSliderSize) {

    float throttleSliderXBoundary = throttleSliderPositon.x + throttleSliderSize.x; 
    float throttleSliderYBoundary = throttleSliderPositon.y + throttleSliderSize.y; 


    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();
        const float ERROR = 30.0f;

        if (( mousePosition.x > throttleSliderPositon.x - ERROR && mousePosition.x < throttleSliderXBoundary + ERROR ) &&
            ( mousePosition.y > throttleSliderPositon.y - ERROR && mousePosition.y < throttleSliderYBoundary + ERROR)) {
                currentThrottlePosition.x = mousePosition.x;
            }
    }

    if (currentThrottlePosition.x < throttleSliderPositon.x) {
        currentThrottlePosition.x = throttleSliderPositon.x;
    }

    if (currentThrottlePosition.x > throttleSliderXBoundary) {
        currentThrottlePosition.x = throttleSliderXBoundary;
    }

    DrawRectangle(throttleSliderPositon.x, throttleSliderPositon.y, throttleSliderSize.x, throttleSliderSize.y, GRAY);
    DrawCircle(currentThrottlePosition.x, currentThrottlePosition.y, 20, RED);
}