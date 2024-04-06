#include <raylib.h>
#include <stdio.h>
#include "tinyfiledialogs.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdlib.h>

void DrawVerticalLines(float *waveData, int numSamples, int drawFactor);

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {

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

    char const * directoryPath = tinyfd_selectFolderDialog("Select a directory", NULL);

    FilePathList files = LoadDirectoryFiles(directoryPath);
    Wave* waves = malloc(files.count * sizeof(Wave));
    int waveCount = 0; // Keep track of the number of waves loaded

    Sound* sounds = malloc(files.count * sizeof(Sound));
    for (int i = 0; i < files.count; i++) {
        if (IsFileExtension(files.paths[i], ".mp3") || IsFileExtension(files.paths[i], ".wav")) {
            waves[waveCount] = LoadWave(files.paths[i]);
            sounds[waveCount] = LoadSoundFromWave(waves[waveCount]);
            waveCount++;
        }
    }

    int currentTrack = 0;
    Wave wave = waves[currentTrack];
    Sound sound = LoadSoundFromWave(wave); // Convert wave to sound to play it

    // Initialization
    float elapsedTime = 0.0f;
    // Get total duration of the wave
    float totalDuration = (float) wave.frameCount / wave.sampleRate;

    PlaySound(sound); // Play the sound

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    // Initialize camera
    Camera2D camera = (Camera2D){{screenWidth / 2, screenHeight / 2}, {screenWidth / 2, screenHeight / 2}, 0.0f, 8.0f};

    int drawFactor = 1000; // Adjust this value to change the number of lines drawn
    int numSamples = wave.frameCount;
    Vector2 *points = malloc((numSamples / drawFactor) * sizeof(Vector2));

    // Main game loop
    while (!exitWindow && !WindowShouldClose()) { // Update ----------------------------------------------------------------

        elapsedTime += GetFrameTime(); // Update elapsed time
        mousePosition = GetMousePosition();


        if (IsKeyPressed(KEY_P)) {
            StopSound(sound);
            currentTrack = (currentTrack + 1) % waveCount;
            sound = sounds[currentTrack];
            PlaySound(sound);
            wave = waves[currentTrack];
            totalDuration = (float) wave.frameCount / wave.sampleRate;
            elapsedTime = 0.0f;
        }


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
        //----------------------------------------------------------------------------------
        BeginDrawing();
        exitWindow = GuiWindowBox((Rectangle) {0, 0, screenWidth, screenHeight},
                                  "kAudio");

        BeginTextureMode(target); // Begin drawing to texture
        BeginMode2D(camera); // Begin 2D mode with camera
        ClearBackground(BLACK); // Clear the texture background

        DrawVerticalLines(wave.data, wave.frameCount, drawFactor);

        EndMode2D(); // End 2D mode
        EndTextureMode(); // End drawing to texture

        BeginDrawing();
        DrawTexturePro(target.texture, (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height}, (Rectangle){0.0f, 25.0f, (float)screenWidth, (float)screenHeight-25}, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        DrawLine(screenWidth/2, 25, screenWidth/2, screenHeight, WHITE); // Draw the bar from top to bottom of the screen

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(points); // Unload points memory
    free(waves); // Unload waves memory
    free(sounds); // Unload sounds memory
    UnloadDirectoryFiles(files);
    UnloadSound(sound);     // Unload sound data
    UnloadWave(wave);       // Unload wave data
    UnloadRenderTexture(target); // Unload render texture
    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)

    CloseWindow();          // Close window and OpenGL context

    return 0;

}

void DrawVerticalLines(float *waveData, int numSamples, int drawFactor) {

    int newNumSamples = numSamples / drawFactor;
    Vector2 *points = malloc(newNumSamples * sizeof(Vector2));
    for (int i = 0; i < newNumSamples; i++) {
        // Calculate the position of the point
        points[i] = (Vector2) {(float) i * drawFactor / numSamples *  screenWidth/ 2,
                               screenHeight / 2 - waveData[i * drawFactor] * 50};
    }
    DrawLineStrip(points, newNumSamples, DARKGRAY);
    free(points); // Don't forget to free the allocated memory
}