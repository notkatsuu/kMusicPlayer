#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"


#include <stdlib.h>


#define MAX_SAMPLES_PER_UPDATE 1024

void DrawWaveData(float *waveData, int numSamples, int drawFactor, Rectangle bounds);



int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;


    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Practica 4 - Raylib Maze 3D");
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(GRAY));
    SetTargetFPS(144); // Set our game to run at 60 frames-per-second

    Vector2 mousePosition = {0};
    Vector2 panOffset = mousePosition;

    bool dragWindow = false;
    bool exitWindow = false;

    Vector2 windowPosition = {500, 200};
    SetWindowPosition(windowPosition.x, windowPosition.y);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);


    InitAudioDevice();      // Initialize audio device

    Wave wave = LoadWave(RESOURCES_PATH"audio.mp3"); // Load music file
    Sound sound = LoadSoundFromWave(wave); // Convert wave to sound to play it




    // Initialization
    float elapsedTime = 0.0f;
    // Get total duration of the wave
    float totalDuration = (float) wave.frameCount / wave.sampleRate;

    PlaySound(sound); // Play the sound

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    // Get the wave data
    float *waveData = wave.data;

    // Initialize camera
    Camera2D camera = {0};
    camera.target = (Vector2) {screenWidth / 2, screenHeight / 2};
    camera.offset = (Vector2) {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 8.0f;

    int drawFactor = 1000; // Adjust this value to change the number of lines drawn
    int numSamples = wave.frameCount;
    Vector2 *points = malloc((numSamples / drawFactor) * sizeof(Vector2));

    // Main game loop
    // Main game loop
    while (!exitWindow && !WindowShouldClose()) {

        // Update
        //----------------------------------------------------------------------------------
        elapsedTime += GetFrameTime(); // Update elapsed time
        mousePosition = GetMousePosition();

        // Update camera
        camera.target.x = elapsedTime / totalDuration * screenWidth;
        camera.offset = (Vector2) {screenWidth / 2, screenHeight / 2}; // Center the camera


        // Update zoom level based on mouse wheel
        float mouseWheelMove = GetMouseWheelMove();
        camera.zoom += mouseWheelMove * 0.25f;
        if (camera.zoom > 30.0f) camera.zoom = 30.0f;
        if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) {
            if (CheckCollisionPointRec(mousePosition,
                                       (Rectangle){0, 0, screenWidth, 20})) {
                dragWindow = true;
                panOffset = mousePosition;
            }
        }

        if (dragWindow) {
            windowPosition.x += mousePosition.x - panOffset.x;
            windowPosition.y += mousePosition.y - panOffset.y;

            SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                dragWindow = false;
        }

        // Draw
        BeginDrawing();

        exitWindow = GuiWindowBox((Rectangle) {0, 0, screenWidth, screenHeight},
                                  "kAudio");


        BeginTextureMode(target); // Begin drawing to texture

        ClearBackground(BLACK); // Clear the texture background

        BeginMode2D(camera); // Begin 2D mode with camera

        // Draw wave data
        Rectangle bounds = {0, 0, screenWidth, screenHeight};
        DrawWaveData(waveData, numSamples, drawFactor, bounds);




        EndMode2D(); // End 2D mode

        EndTextureMode(); // End drawing to texture

        BeginDrawing();



        // Draw the texture
        DrawTexturePro(target.texture, (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height}, (Rectangle){0.0f, 25.0f, (float)screenWidth, (float)screenHeight-25}, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

        DrawLine(screenWidth/2, 25, screenWidth/2, screenHeight, WHITE); // Draw the bar from top to bottom of the screen

        EndDrawing();
    }

    // De-Initialization
    UnloadSound(sound);     // Unload sound data
    UnloadWave(wave);       // Unload wave data
    UnloadRenderTexture(target); // Unload render texture
    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)

    CloseWindow();          // Close window and OpenGL context

    return 0;

}

void DrawWaveData(float *waveData, int numSamples, int drawFactor, Rectangle bounds) {
    float sampleWidth = bounds.width / numSamples;
    float sampleHeight = bounds.height / 2;
    int newNumSamples = numSamples / drawFactor;
    Vector2 *points = malloc(newNumSamples * sizeof(Vector2));

// Draw wave data
    for (int i = 0; i < newNumSamples; i++) {
        // Calculate the position of the point
        points[i] = (Vector2) {(float) i * drawFactor / numSamples * bounds.width / 2,
                               bounds.height / 2 - waveData[i * drawFactor] * 50};
    }

// Draw the polyline
    DrawLineStrip(points, newNumSamples, DARKGRAY);

    free(points); // Don't forget to free the allocated memory

}