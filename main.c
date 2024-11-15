#include <stdlib.h>

#include "tinyfiledialogs.h"
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION

#include "pthread.h"
#include "raygui.h"
#include "raymath.h"
#include "semaphore.h"
#include <direct.h>

// Include styles
#include "styles/style_candy.h"
#include "styles/style_cyber.h"
#include "styles/style_dark.h"
#include "styles/style_jungle.h"
#include "styles/style_lavanda.h"
#include "styles/style_terminal.h"

#define MAX_LOADING_THREADS 4
#define KATSUGRAY                                                              \
  (Color) { 10, 10, 10, 255 }
#define PLAYBUTTON "#131#"
#define PAUSEBUTTON "#132#"
#define PATH_FILE "path.txt"

#include <complex.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function Declarations --------------------------------------

// UI Drawing Functions ------------------------------------------

void DrawUI();

void DrawAllWaveforms();

void DrawWaveform(const float *waveData, int numSamples, int drawFactor,
                  int sampleRate);

void DrawVibratingString();

void DrawSpectrogram();

// UI Logic Functions -------------------------------------------
void GUINextTheme();

Color IntToColor(int value);

void LoadFiles();

void UpdateTitles();

// File Management Functions -------------------------------------------
void CountAudioFiles();

void LoadAllMusic();

void refreshDirectory();

void createDirectories();

void RefreshDataAllocation();

//Threading Functions -------------------------------------------
void *LoadFilesThread();

void *LoadMusic(void *arg);

// Music Functions -------------------------------------------
void PlayNextTrack(int *index);

void SkipSecondsInMusicStream(const int *index, float seconds);

// Input Handling Functions -------------------------------------------
void HandleKeyboardInputs();


// Global Variables -------------------------------------------

Texture2D logoTexture;
float logoRotation = 0.0f;

typedef enum {
    LOADING_FILES, LOADING_WAVEFORMS, PLAYING
} PlayerState;

// Theme Functions -------------------------------------------
typedef void (*Theme)();

//UTILITIES
void fft(complex double* a, int n);

Theme themeFunctions[] = {
        GuiLoadStyleDark, GuiLoadStyleCyber, GuiLoadStyleCandy,
        GuiLoadStyleJungle, GuiLoadStyleLavanda, GuiLoadStyleTerminal,
        // GuiLoadStyleKatsu
};

Color logoColors[] = {
        WHITE, BLUE, ORANGE, GREEN, PURPLE, GREEN,
};

typedef enum {
    WAVEFORM, STRING, BAR, ORBIT, NONE
} ViewportType;

// Global Variables -------------------------------------------
const int screenWidth = 800;
const int screenHeight = 450;

bool firstStart = true;
int filesLoaded = -1;
bool waveformLoaded = false;
bool playing = false;
int currentTrack = 0; // To track the song that's playing

bool dragWindow = false;
bool exitWindow = false;

char *directoryPath;

Wave *waves;                // Array for storing wave data
Music *tracks;              // Array for storing music tracks
FilePathList files;         // Raw list of files in the directory
RenderTexture2D **waveformChunks; // Array of arrays of RenderTexture2D
int *numChunks; // Array to store the number of chunks for each song
float *totalDurations;      // Array for storing the total duration of each song

sem_t sem_fileLoader;
pthread_t loadingThread;

complex double *input;
int fftSize = 4096; // This value may need to be adjusted depending on your needs
float *waveData;
int waveCount = 0;
float elapsedTime = 0.0f;
float musicVolume = 0.5f;

// Struct for passing data to the thread
typedef struct {
    char *path;
    int index;
} pathIndex;

PlayerState currentState = LOADING_FILES;
ViewportType currentViewport = STRING;
bool infoWindow = false;

int currentTheme = 0;
int currentTextSize;
char *playButtonText = PLAYBUTTON;

Vector2 mousePosition = {0};
Vector2 panOffset;

// UI
// ELEMENTS-------------------------------------------------------------------------
Rectangle volumeBar = {((float) screenWidth / 2) + 80,
                       ((float) screenHeight / 2) + 180, 80, 10};
Rectangle progressBar = {0, (float) screenHeight - 20, (float) screenWidth, 20};
int progressBarState = 0;

int textHeaderPosition = screenWidth; // Starting position for the title text
// (comes from the right to left)
int nextSongTextPosition =
        screenWidth; // Starting position for the next song text

char **filteredFiles;

void DrawWaveformChunks(int currentTrack, Camera2D camera_2d);


int main(void) {
    LoadFiles();

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "kMusicPlayer");

    GUINextTheme();

    SetTargetFPS(144);
    sem_init(&sem_fileLoader, 0, MAX_LOADING_THREADS);

    Vector2 windowPosition = {500, 200};
    SetWindowPosition((int) windowPosition.x, (int) windowPosition.y);

    InitAudioDevice();
    SetMasterVolume(musicVolume);

    Camera2D waveformCam = {
        .offset = {(float)screenWidth / 2, (float)screenHeight / 2},
        .target = {(float)screenWidth / 2, (float)screenHeight / 2},
        .rotation = 0.0f,
        .zoom = 3.0f
    };

    Camera3D orbitalCam = {
        .position = {0.0f, 2.0f, 10.0f},
        .target = {0.0f, 0.0f, 0.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };

    // Load all waveforms as chunks
    DrawAllWaveforms();

    // Main game loop
    while (!exitWindow && !WindowShouldClose()) {
        switch (currentState) {
            case LOADING_FILES:
                if (firstStart) {
                    createDirectories();
                    pthread_create(&loadingThread, NULL, LoadFilesThread, NULL);
                    firstStart = false;
                }
                break;

            case LOADING_WAVEFORMS:
                if (waveformLoaded) {
                    currentState = PLAYING;
                }
                break;

            case PLAYING:
                HandleKeyboardInputs();

                if (playing) {
                    elapsedTime += GetFrameTime();
                    logoRotation += 0.1f;
                    if (elapsedTime >= totalDurations[currentTrack]) {
                        PlayNextTrack(&currentTrack);
                        nextSongTextPosition = screenWidth;
                    }

                    if (!IsMusicStreamPlaying(tracks[currentTrack])) {
                        PlayMusicStream(tracks[currentTrack]);
                    }
                }

                mousePosition = GetMousePosition();
                UpdateMusicStream(tracks[currentTrack]);

                UpdateTitles();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) {
                    if (CheckCollisionPointRec(mousePosition, (Rectangle){0, 0, (float)screenWidth, 40})) {
                        dragWindow = true;
                        panOffset = mousePosition;
                    }
                }

                if (dragWindow) {
                    windowPosition.x += mousePosition.x - panOffset.x;
                    windowPosition.y += mousePosition.y - panOffset.y;
                    SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragWindow = false;
                }

                SetMasterVolume(powf(musicVolume, 2));

                switch (currentViewport) {
                    case WAVEFORM:
                        if (!waveformLoaded) currentState = LOADING_WAVEFORMS;
                        waveformCam.target.x = elapsedTime * 50;
                        break;

                    case STRING:
                        break;

                    case ORBIT:
                        UpdateCamera(&orbitalCam, CAMERA_ORBITAL);
                    

                    case BAR:
                        waveData = waves[currentTrack].data;
                        input = (complex double *)malloc(sizeof(complex double) * fftSize);
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case LOADING_FILES:
                DrawText("Loading music files...", screenWidth / 2 - MeasureText("Loading music files...", 20) / 2, screenHeight / 2 - 20, 20, RAYWHITE);
                if (filesLoaded != -1) {
                    DrawRectangle(150, screenHeight / 2 + 20, (screenWidth - 300) * filesLoaded / waveCount, 20, DARKGRAY);
                }
                break;

            case LOADING_WAVEFORMS:
                if (!waveformLoaded) {
                    ClearBackground(BLACK);
                    DrawAllWaveforms();
                    waveformLoaded = true;
                }
                break;

            case PLAYING:
                switch (currentViewport) {
                    case WAVEFORM:
                        BeginMode2D(waveformCam);
                        ClearBackground(BLANK);
                        DrawWaveformChunks(currentTrack, waveformCam);
                        EndMode2D();
                        DrawLine(screenWidth / 2, 50, screenWidth / 2, screenHeight - 60, Fade(IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)), 0.8f));
                        break;

                    case STRING:
                        DrawVibratingString();
                        break;

                    case BAR:
                        float scale = 8.0f;
                    // Prepare for FFT
                    // Fill the input with audio data
                    for (int i = 0; i < fftSize; i++) {
                        input[i] = waveData[(int) (elapsedTime * 2 * waves[currentTrack].sampleRate) + i] + 0.0 * I;
                    }
                    // Perform FFT
                    fft(input, fftSize);
                    for (int i = 0; i < fftSize / 2; i++) { // Only iterate up to half the FFT size due to symmetry in real signals
                        float magnitude = sqrt(input[i] * conj(input[i])); // Calculate the magnitude of the complex number
                        // Draw the bar for this frequency
                        DrawRectanglePro((Rectangle) {i * scale, 0, scale, magnitude},
                                         (Vector2) {10, screenHeight - 80}, 180, IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
                    }
                    // Free the allocated memory
                    free(input);
                        break;

                    case ORBIT:
                        BeginMode3D(orbitalCam);
                    ClearBackground(BLACK);
                    fftSize = 8000;
                    float radius = 5.0f; // Radius of the circle
                    float cubeSize = 0.01f; // Size of each cube
                    int numCubes = fftSize / 2; // Number of cubes, should match half the FFT size
                    complex double *input = (complex double*) malloc(sizeof(complex double) * fftSize);
                    for (int i = 0; i < fftSize / 2; i++) {
                        input[i] = waveData[(int) (elapsedTime * 2 * waves[currentTrack].sampleRate) + i] + 0.0 * I;
                    }
                    fft(input, fftSize);
                    for (int i = 0; i < numCubes; i++) {
                        float magnitude = sqrt(input[i] * conj(input[i]));
                        // Calculate the position of each cube in a circular pattern
                        float angle = (float)i / numCubes * 2 * PI; // Angle for this cube
                        float x = cos(angle) * radius;
                        float z = sin(angle) * radius;
                        float y = magnitude * 0.1f; // Scale the magnitude for visibility
                        DrawCube((Vector3){x, 0, z}, cubeSize, y+cubeSize, cubeSize, IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
                    }
                    free(input);
                        EndMode3D();
                        break;

                    default:
                        break;
                }

                DrawUI();
                break;
        }

        EndDrawing();
    }

    // Cleanup
    StopMusicStream(tracks[currentTrack]);
    UnloadMusicStream(tracks[currentTrack]);

    for (int i = 0; i < waveCount; i++) {
        UnloadWave(waves[i]);
        for (int j = 0; j < numChunks[i]; j++) {
            UnloadRenderTexture(waveformChunks[i][j]);
        }
        free(waveformChunks[i]);
    }

    free(waveformChunks);
    free(numChunks);
    free(waves);
    free(tracks);
    UnloadDirectoryFiles(files);
    free(totalDurations);
    free(directoryPath);
    free(filteredFiles);
    sem_destroy(&sem_fileLoader);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}




void DrawVibratingString() {
    // Parameters for the vibrating string
    static float t = 0; // Time parameter

    // Calculate the number of segments in the string
    int segments = screenWidth;
    Vector2 *points = malloc(segments * sizeof(Vector2));

    // Retrieve the waveform data for the current song at the current time
    float *waveData = waves[currentTrack].data;
    int numSamples = waves[currentTrack].frameCount;
    int sampleRate = waves[currentTrack].sampleRate;
    int currentTimeSample = (int) (elapsedTime * 2 * sampleRate);

    // Generate the points along the string
    for (int i = 0; i < segments; i++) {
        // Use the waveform data to calculate the y-coordinate
        float y = screenHeight / 2.0f;
        if (currentTimeSample + i < numSamples * 2) {
            y += waveData[currentTimeSample + i] * 200.0f;
        }
        points[i] = (Vector2) {(float) i, y};
    }

    // Draw the string
    DrawLineStrip(points, segments, IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    // Update time
    t += GetFrameTime();

    // Free the points array
    free(points);
}

void GUINextTheme() {
    if (currentTheme >= 0 &&
        currentTheme < sizeof(themeFunctions) / sizeof(*themeFunctions)) {
        themeFunctions[currentTheme]();
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize * 2);
        currentTextSize = GuiGetFont().baseSize * 2;
    }
}

void *LoadMusic(void *arg) { // Thread function to load music
    sem_wait(&sem_fileLoader);
    pathIndex *data = (pathIndex *) arg;
    char *path = data->path;
    int index = data->index;

    if (IsFileExtension(path, ".mp3") || IsFileExtension(path, ".wav") ||
        IsFileExtension(path, ".ogg") || IsFileExtension(path, ".qoa")) {
        waves[index] = LoadWave(path);

        if (waves[index].data == NULL || waves[index].sampleRate == 0 ||
            waves[index].sampleSize == 0 || waves[index].channels == 0 ||
            waves[index].frameCount == 0 ||
            waves[index].sampleRate != 44100 && waves[index].sampleRate != 48000) {

            filteredFiles[index] = NULL; // Set filteredFiles[index] to NULL
            UnloadWave(waves[index]);
            sem_post(&sem_fileLoader); // Release the semaphore
            return (void *) -1;         // return -1 for failure
        }

        tracks[index] = LoadMusicStream(path);
        filteredFiles[index] = path;
        totalDurations[index] = GetMusicTimeLength(tracks[index]);
        filesLoaded++;
    }

    sem_post(&sem_fileLoader);
    return (void *) 0; // return 0 for success
}

void DrawWaveformChunks(int currentTrack, Camera2D cam) {
    BeginMode2D(cam);

    float xOffset = 0.0f;

    for (int j = 0; j < numChunks[currentTrack]; j++) {
        DrawTextureRec(
            waveformChunks[currentTrack][j].texture,
            (Rectangle) {0, 0, (float) waveformChunks[currentTrack][j].texture.width, -(float) waveformChunks[currentTrack][j].texture.height},
            (Vector2) {xOffset, 0},
            IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))
        );

        xOffset += waveformChunks[currentTrack][j].texture.width;
    }

    EndMode2D();
}


void DrawWaveform(const float *waveData, int numSamples, int drawFactor,
                  int sampleRate) {

    int newNumSamples =
            numSamples / drawFactor * 2; //*2 cause every pixel of width needs the
    //start and endpoint of the wave

    Vector2 *points = malloc(newNumSamples * sizeof(Vector2));

    for (int i = 0; i < newNumSamples; i++) {
        points[i] =
                (Vector2) {(float) i * (float) drawFactor / (float) sampleRate / 2 * 50,
                           (float) screenHeight / 2 - waveData[i * drawFactor] * 100};
    }
    DrawLineStrip(points, newNumSamples, (Color) {255, 255, 255, 155});
    free(points); // Don't forget to free the allocated memory*/
}



void DrawAllWaveforms() {
    const int chunkWidth = 1024; // Width of each chunk (adjust based on your requirements)
    const int chunkHeight = 500;

    waveformChunks = malloc(waveCount * sizeof(RenderTexture2D *));
    numChunks = malloc(waveCount * sizeof(int));

    for (int i = 0; i < waveCount; i++) {
        int waveformWidth = (int) roundf((float) waves[i].frameCount / waves[i].sampleRate * 50);
        numChunks[i] = (waveformWidth + chunkWidth - 1) / chunkWidth; // Calculate number of chunks

        waveformChunks[i] = malloc(numChunks[i] * sizeof(RenderTexture2D));

        char cacheDir[1024];
        snprintf(cacheDir, sizeof(cacheDir), "cache/waveforms/%s", GetFileNameWithoutExt(filteredFiles[i]));

        // Create directory for waveform chunks if it doesn't exist
        if (!DirectoryExists(cacheDir)) {
            mkdir(cacheDir);
        }

        for (int j = 0; j < numChunks[i]; j++) {
            waveformChunks[i][j] = LoadRenderTexture(chunkWidth, chunkHeight);

            char chunkPath[1024];
            snprintf(chunkPath, sizeof(chunkPath), "%s/chunk_%d.png", cacheDir, j);

            if (FileExists(chunkPath)) {
                // Load cached chunk texture if it exists
                waveformChunks[i][j].texture = LoadTextureFromImage(LoadImage(chunkPath));
                continue;
            }

            BeginTextureMode(waveformChunks[i][j]);
            ClearBackground(BLANK);

            // Draw part of the waveform data for this chunk
            int startSample = j * 1024 * waves[i].sampleRate / 50 * 8;
            int numSamples = (chunkWidth * waves[i].sampleRate) / 50;

            DrawWaveform(&waves[i].data[startSample], numSamples, 50, waves[i].sampleRate);
            EndTextureMode();

            // Export chunk to PNG
            ExportImage(LoadImageFromTexture(waveformChunks[i][j].texture), chunkPath);
        }
    }
}


void LoadFiles() {
    FILE *file = fopen(PATH_FILE, "r");
    if (file != NULL) {
        char path[1024];
        fgets(path, 1024, file);
        directoryPath = malloc(strlen(path) * sizeof(char));
        strcpy(directoryPath, path);
        fclose(file);
    } else {

        refreshDirectory();
        file = fopen(PATH_FILE, "w");
        fprintf(file, "%s", directoryPath);
        fclose(file);
    }
    files = LoadDirectoryFiles(directoryPath);
}

void CountAudioFiles() {
    // printf("Counting audio files\n");
    waveCount = 0;
    for (int i = 0; i < files.count; i++) {
        if (IsFileExtension(files.paths[i], ".mp3") ||
            IsFileExtension(files.paths[i], ".wav") ||
            IsFileExtension(files.paths[i], ".ogg") ||
            IsFileExtension(files.paths[i], ".qoa")) {
            waveCount++;
        }
    }
    filteredFiles = malloc(waveCount * sizeof(char *));
}

void LoadAllMusic() {
    waves = malloc(waveCount * sizeof(Wave));
    tracks = malloc(waveCount * sizeof(Music));
    totalDurations = malloc(waveCount * sizeof(float));

    pthread_t *threads = malloc(files.count * sizeof(pthread_t));
    pathIndex *threadData = malloc(files.count * sizeof(pathIndex));
    filesLoaded = 0;
    int audioFileIndex = 0;
    for (int i = 0; i < files.count; i++) {
        if (IsFileExtension(files.paths[i], ".mp3") ||
            IsFileExtension(files.paths[i], ".wav") ||
            IsFileExtension(files.paths[i], ".ogg") ||
            IsFileExtension(files.paths[i], ".qoa")) {
            threadData[audioFileIndex].path = files.paths[i];
            threadData[audioFileIndex].index = audioFileIndex;
            pthread_create(&threads[audioFileIndex], NULL, LoadMusic,
                           &threadData[audioFileIndex]);
            audioFileIndex++;
        }
    }

    for (int i = 0; i < waveCount; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(threadData);
}

void RefreshDataAllocation() {
    int validCount = 0;
    for (int i = 0; i < waveCount; i++) {
        if (filteredFiles[i] != NULL) {
            validCount++;
        }
    }

    // Create new arrays for the valid files
    Music *newTracks = malloc(validCount * sizeof(Music));
    Wave *newWaves = malloc(validCount * sizeof(Wave));
    char **newFilteredFiles = malloc(validCount * sizeof(char *));
    float *newTotalDurations = malloc(validCount * sizeof(float));

    int j = 0;
    for (int i = 0; i < waveCount; i++) {
        if (filteredFiles[i] != NULL) {
            newTracks[j] = tracks[i];
            newWaves[j] = waves[i];
            newFilteredFiles[j] = filteredFiles[i];
            newTotalDurations[j] = totalDurations[i];
            j++;
        }
    }
    // Free the old arrays
    free(tracks);
    free(waves);
    free(filteredFiles);
    free(totalDurations);

    // Replace the old arrays with the new ones
    tracks = newTracks;
    waves = newWaves;
    filteredFiles = newFilteredFiles;
    waveCount = validCount;
    totalDurations = newTotalDurations;
}

void PlayNextTrack(int *index) {
    StopMusicStream(tracks[*index]);
    *index = (*index + 1) % waveCount;
    PlayMusicStream(tracks[*index]);
    waves[*index] = waves[*index];
    elapsedTime = 0.0f;
}

void SkipSecondsInMusicStream(const int *index, float seconds) {
    elapsedTime += seconds;
    if (elapsedTime > totalDurations[*index]) {
        elapsedTime = totalDurations[*index];
    } else if (elapsedTime < 0.0f) {
        elapsedTime = 0.0f;
    }

    SeekMusicStream(tracks[*index],
                    elapsedTime); // Seek to the new position
}

void *LoadFilesThread() {
    CountAudioFiles();
    LoadAllMusic();          // Load all music from the files
    RefreshDataAllocation(); // Refresh the memory allocation for the dinamic

    //all the files finished loading
    currentState = PLAYING;
    return NULL;
}

void HandleKeyboardInputs() {

    // if press esc close window
    if (IsKeyPressed(KEY_ESCAPE)) {
        exitWindow = true;
    }

    // advance 5 seconds when scrolling up with mouse wheel
    if (IsKeyPressed(KEY_RIGHT) || GetMouseWheelMove() > 0.0f) {
        SkipSecondsInMusicStream(&currentTrack, 5.0f);
    }

    // rewind 5 seconds when left arrow
    if (IsKeyPressed(KEY_LEFT) || GetMouseWheelMove() < 0.0f) {
        SkipSecondsInMusicStream(&currentTrack, -5.0f);
    }

    // pause
    if (IsKeyPressed(KEY_SPACE)) {
        if (playing) {
            PauseMusicStream(tracks[currentTrack]);
            playing = false;
        } else {
            ResumeMusicStream(tracks[currentTrack]);
            playing = true;
        }
    }

    if (IsKeyPressed(KEY_UP)) {
        musicVolume += (musicVolume < 1.0f)
                       ? 0.05f
                       : 0.0f; // Increase music volume (up to 1.0f
        musicVolume = roundf(musicVolume * 20) / 20;
        SetMasterVolume(powf(musicVolume, 2));
    }

    if (IsKeyPressed(KEY_DOWN)) {
        musicVolume -= (musicVolume > 0.0f)
                       ? 0.05f
                       : 0.0f; // Decrease music volume (up to 0.0f
        musicVolume = roundf(musicVolume * 20) / 20;
        SetMasterVolume(powf(musicVolume, 2));
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        exit(0);
    }

    if (IsKeyPressed(KEY_F1)) {
        currentTheme =
                (currentTheme + 1) % (int) (sizeof(themeFunctions) / sizeof(*themeFunctions));
        GUINextTheme();
    }
}

void UpdateTitles() {
    textHeaderPosition -= 1; // Adjust the value to change the speed
    if (textHeaderPosition +
        MeasureText(TextFormat("Playing: %s",
                               GetFileName(filteredFiles[currentTrack])),
                    currentTextSize) <
        0) {
        textHeaderPosition = screenWidth;
    }
    // only update nextSong it when it's less than 30s for the next song
    if (totalDurations[currentTrack] - elapsedTime < 30.0f) {
        nextSongTextPosition -= 2; // Adjust the value to change the speed
        if (nextSongTextPosition +
            MeasureText(
                    TextFormat(
                            "Next: %s",
                            GetFileName(filteredFiles[(currentTrack + 1) % waveCount])),
                    currentTextSize) <
            0) {
            nextSongTextPosition = screenWidth;
        }
    }
}

void DrawUI() {

    DrawRectangleGradientV(0, 0, screenWidth, (screenHeight / 2) - 20, BLACK,
                           BLANK); // Draw the top gradient
    DrawRectangleGradientV(0, (screenHeight / 2) + 20, screenWidth,
                           screenHeight / 2, BLANK,
                           BLACK); // Draw the bottom gradient

    // Draw a little header for the title
    GuiDrawRectangle(
            (Rectangle) {0, 0, (float) screenWidth, (float) currentTextSize * 1.5f}, 1,
            BLANK, (Fade(IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)), 0.1f)));

    GuiDrawText(
            TextFormat("Playing: %s", GetFileName(filteredFiles[currentTrack]), 20),
            (Rectangle) {(float) textHeaderPosition, 5,
                         (float) MeasureText(
                                 TextFormat("Playing: %s",
                                            GetFileName(filteredFiles[currentTrack])),
                                 currentTextSize),
                         (float) currentTextSize},
            TEXT_ALIGN_LEFT, IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    GuiDrawText(
            TextFormat("%02d:%02d", (int) elapsedTime / 60, (int) elapsedTime % 60),
            (Rectangle) {20, (float) screenHeight - 65,
                         (float) MeasureText("00:00", currentTextSize),
                         (float) currentTextSize},
            TEXT_ALIGN_LEFT, IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    GuiDrawText(TextFormat("%02d:%02d", (int) totalDurations[currentTrack] / 60,
                           (int) totalDurations[currentTrack] % 60),
                (Rectangle) {(float) screenWidth - 20 -
                             (float) MeasureText("00:00", currentTextSize),
                             (float) screenHeight - 65,
                             (float) MeasureText("00:00", currentTextSize),
                             (float) currentTextSize},
                TEXT_ALIGN_RIGHT,
                IntToColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    if (totalDurations[currentTrack] - elapsedTime < 30.0f) {
        GuiLabel(
                (Rectangle) {
                        (float) nextSongTextPosition, 30,
                        (float) MeasureText(
                                TextFormat(
                                        "Next: %s",
                                        GetFileName(filteredFiles[(currentTrack + 1) % waveCount])),
                                currentTextSize),
                        (float) currentTextSize},
                TextFormat("Next: %s",
                           GetFileName(filteredFiles[(currentTrack + 1) % waveCount])));
    }

    DrawTexturePro(logoTexture, (Rectangle) {0, 0, 256, 256},
                   (Rectangle) {20, 20, 100, 100}, (Vector2) {50, 50}, logoRotation,
                   logoColors[currentTheme]);

    // Draw the volume bar
    GuiSliderBar(volumeBar, NULL, "#122#", &musicVolume, 0.0f, 1.0f);

    // Progress bar logic
    switch (GuiSliderBar(progressBar, NULL, "#121#", &elapsedTime, 0.0f,
                         totalDurations[currentTrack])) {
        case 2:
            if (IsMusicStreamPlaying(tracks[currentTrack]))
                PauseMusicStream(tracks[currentTrack]);

            playButtonText = PLAYBUTTON;
            progressBarState = 2;
            break;

        case 1:
        case 0:
            if (progressBarState == 2) {
                SeekMusicStream(tracks[currentTrack], elapsedTime);
                if (playing) {
                    ResumeMusicStream(tracks[currentTrack]);
                    playing = true;
                    playButtonText = PAUSEBUTTON;
                }
                progressBarState = 0;
            }
            break;
    }

    // Draw the play/pause button in the middle of the latest rectangle
    if (GuiButton((Rectangle) {((float) screenWidth / 2) - 20,
                               (float) screenHeight - 55, 40, 30},
                  playButtonText)) {
        if (playing) {
            PauseMusicStream(tracks[currentTrack]);
            playing = false;
            playButtonText = PLAYBUTTON;
        } else {
            ResumeMusicStream(tracks[currentTrack]);
            playing = true;
            playButtonText = PAUSEBUTTON;
        }
    }
    // Draw the next song button
    if (GuiButton((Rectangle) {((float) screenWidth / 2) + 20,
                               (float) screenHeight - 55, 40, 30},
                  "#134#")) {
        PlayNextTrack(&currentTrack);
        playing = true;
        elapsedTime = 0.0f;
    }

    // Draw the before song button
    if (GuiButton((Rectangle) {((float) screenWidth / 2) - 60,
                               (float) screenHeight - 55, 40, 30},
                  "#129#")) {
        StopMusicStream(tracks[currentTrack]);
        currentTrack = (currentTrack - 1) % waveCount;
        if (currentTrack < 0) {
            currentTrack = waveCount - 1;
        }
        PlayMusicStream(tracks[currentTrack]);
        elapsedTime = 0.0f;
    }

    // Draw button to change to the next theme
    if (GuiButton((Rectangle) {((float) screenWidth / 2) - 90,
                               (float) screenHeight - 50, 20, 20},
                  "#12#")) {
        currentTheme =
                (currentTheme + 1) % (int) (sizeof(themeFunctions) / sizeof(*themeFunctions));
        GUINextTheme();
    }

    // Draw button to show the change the viewport
    if (GuiButton((Rectangle) {((float) screenWidth / 2) - 120,
                               (float) screenHeight - 50, 20, 20},
                  "#13#")) {

        currentViewport = currentViewport == NONE ? WAVEFORM : currentViewport + 1;
    }

    // button to refresh the directory
    if (GuiButton((Rectangle) {(float) screenWidth - 30, 50, 30, 30}, "#3#")) {
        refreshDirectory();
        LoadFiles();
        pthread_create(&loadingThread, NULL, LoadFilesThread, NULL);
        currentState = LOADING_FILES;
    }
}

Color IntToColor(int value) {
    Color color;
    color.r = (unsigned char) (value >> 24);
    color.g = (unsigned char) ((value >> 16) & 0xFF);
    color.b = (unsigned char) ((value >> 8) & 0xFF);
    color.a = (unsigned char) (value & 0xFF);
    return color;
}

void refreshDirectory() {
    directoryPath = (char *) tinyfd_selectFolderDialog(
            "Select a folder with audio files", NULL);
    if (directoryPath == NULL) {
        exit(0);
    }
    FILE *file = fopen(PATH_FILE, "w");
    fprintf(file, "%s", directoryPath);
    fclose(file);

    waveformLoaded = false;
}

void createDirectories() {
    if (!DirectoryExists("cache")) {
        mkdir("cache");
    }
    if (!DirectoryExists("cache/waveforms")) {
        mkdir("cache/waveforms");
    }
}



void fft(complex double* a, int n) {
    if (n == 1)
        return;

    complex double a0[n/2];
    complex double a1[n/2];
    for (int i = 0; i < n/2; i++) {
        a0[i] = a[i*2];
        a1[i] = a[i*2+1];
    }

    fft(a0, n/2);
    fft(a1, n/2);

    for (int i = 0; i < n/2; i++) {
        complex double t = cexp(-2.0 * I * M_PI * i / n) * a1[i];
        a[i] = a0[i] + t;
        a[i+n/2] = a0[i] - t;
    }
}
