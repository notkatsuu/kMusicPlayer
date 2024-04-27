#include <stdlib.h>

#include "tinyfiledialogs.h"
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION

#include "pthread.h"
#include "raygui.h"
#include "semaphore.h"

#define MAX_LOADING_THREADS 4


// Function Declarations --------------------------------------
void SetGuiStyles();

void LoadFiles();

void CountAudioFiles();

void LoadAllMusic();

void RefreshDataAllocation();

void DrawSong(const float *waveData, int numSamples, int drawFactor, int sampleRate);

void DrawAllWaveforms();

void *LoadMusic(void *arg);

void PlayNextTrack(int *index);

void SeekInMusicStream(const int *index, float seconds);

void *LoadFilesThread();

void ManageInputs();

void UpdateTitles();

void DrawUI();


typedef enum {
    LOADING_FILES,
    LOADING_WAVEFORMS,
    PLAYING
} PlayerState;

typedef enum {
    WAVEFORM,
    SPECTRUM,
    BAR
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

char const *directoryPath;

Wave *waves; //Array for storing wave data
Music *tracks; //Array for storing music tracks
FilePathList files; //Raw list of files in the directory
RenderTexture2D *waveforms; //Array for storing waveforms' textures
float *totalDurations; //Array for storing the total duration of each song

sem_t sem_fileLoader;
pthread_t loadingThread;

int waveCount = 0;
float elapsedTime = 0.0f;
float musicVolume = 0.0f;

// Struct for passing data to the thread
typedef struct {
    char *path;
    int index;
} pathIndex;

PlayerState currentState = LOADING_FILES;
ViewportType currentViewport = WAVEFORM;

Vector2 mousePosition = {0};
Vector2 panOffset;

int textHeaderPosition = screenWidth; //Starting position for the title text (comes from the right to left)
int nextSongTextPosition = screenWidth; //Starting position for the next song text

char **filteredFiles;

int main(void) { // Main function

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "kMusicPlayer");

    SetGuiStyles();

    SetTargetFPS(144); // Set our game to run at 60 frames-per-second
    sem_init(&sem_fileLoader, 0, MAX_LOADING_THREADS); //Set up semaphore for file loading


    Vector2 windowPosition = {500, 200}; // Set window position on startup
    SetWindowPosition((int) windowPosition.x, (int) windowPosition.y);

    InitAudioDevice();
    SetMasterVolume(musicVolume);

    // Initialize camera
    Camera2D camera =
            (Camera2D) {{(float) screenWidth / 2, (float) screenHeight / 2},
                        {(float) screenWidth / 2, (float) screenHeight / 2},
                        0.0f,
                        3.0f};

    RenderTexture2D camTarget = LoadRenderTexture(screenWidth,
                                                  screenHeight); // Load a render texture to draw the camera target in it

    // Main game loop
    while (!exitWindow && !WindowShouldClose()) {
        switch (currentState) {
            case LOADING_FILES:
                if (firstStart) {
                    pthread_create(&loadingThread, NULL, LoadFilesThread, NULL);
                    firstStart = false;
                }
                break;

            case PLAYING:

                ManageInputs();

                if (playing) {
                    elapsedTime += GetFrameTime(); // Update the elapsed time
                    if (elapsedTime >=
                        totalDurations[currentTrack]) { // If the song ends, play the next one, updating the duration, elapsed time, etc.
                        StopMusicStream(tracks[currentTrack]);
                        elapsedTime = 0.0f;
                        nextSongTextPosition = screenWidth;
                        currentTrack = (currentTrack + 1) % waveCount;
                        PlayMusicStream(tracks[currentTrack]);
                    }

                    if (IsMusicStreamPlaying(tracks[currentTrack]) == false) {
                        PlayMusicStream(tracks[currentTrack]);
                    }
                }

                mousePosition = GetMousePosition();
                UpdateMusicStream(
                        tracks[currentTrack]); // Update music stream with new track

                // Update camera target position
                camera.target.x = elapsedTime * 50;

                UpdateTitles();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) { // Start dragging the window
                    if (CheckCollisionPointRec(mousePosition,
                                               (Rectangle) {0, 0, (float) screenWidth, 40})) {
                        dragWindow = true;
                        panOffset = mousePosition;
                    }
                }

                if (dragWindow) { // While dragging the window
                    windowPosition.x += mousePosition.x - panOffset.x;
                    windowPosition.y += mousePosition.y - panOffset.y;
                    SetWindowPosition((int) windowPosition.x, (int) windowPosition.y);
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                        dragWindow = false;
                }

                break;

            default:
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            case LOADING_FILES:
                DrawText("Loading music files...",
                         screenWidth / 2 - MeasureText("Loading music files...", 20) / 2,
                         screenHeight / 2 - 20, 20, RAYWHITE); // Draw the loading text

                if (filesLoaded != -1) {
                    DrawRectangle(150, screenHeight / 2 + 20,
                                  (screenWidth - 300) * filesLoaded / waveCount, 20,
                                  DARKGRAY); // Draw the progress bar
                }

                break;

            case LOADING_WAVEFORMS:
                DrawText("Loading waveforms...", screenWidth / 2 - MeasureText("Loading waveforms...", 20) / 2,
                         screenHeight / 2 - 20, 20, RAYWHITE);

                if (!waveformLoaded) {
                    DrawAllWaveforms();
                    waveformLoaded = true;
                }
                break;

            case PLAYING:
                switch (currentViewport) {
                    case WAVEFORM:
                        BeginMode2D(camera);    // Begin 2D mode with camera
                        ClearBackground(BLANK); // Clear the texture background
                        DrawTextureRec(waveforms[currentTrack].texture,
                                       (Rectangle) {0, 0,
                                                    (float) waveforms[currentTrack].texture.width,
                                                    (float) -waveforms[currentTrack].texture.height},
                                       (Vector2) {0, 0}, WHITE);


                        EndMode2D(); // End 2D mode
                        break;

                    default:
                        break;

                }
                DrawUI();

                break;

        }

        EndDrawing();

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    StopMusicStream(tracks[currentTrack]);   // Stop music stream
    UnloadMusicStream(tracks[currentTrack]); // Unload music stream
    for (int i = 0; i < waveCount; i++) {
        UnloadWave(waves[i]); // Unload wave data
        UnloadRenderTexture(waveforms[i]);
    }
    free(waveforms); // Unload waveforms memory
    free(waves);  // Unload waves memory
    free(tracks); // Unload sounds memory
    UnloadDirectoryFiles(files);
    UnloadRenderTexture(camTarget); // Unload render texture
    free(filteredFiles);            // Unload filtered files memory
    sem_destroy(&sem_fileLoader);

    CloseAudioDevice(); // Close audio device (music streaming is automatically stopped)
    CloseWindow();      // Close window and OpenGL context

    return 0;
}

void DrawSong(const float *waveData, int numSamples, int drawFactor,
              int sampleRate) {

    int newNumSamples =
            numSamples / drawFactor * 2; //*2 cause every pixel of width needs the start and endpoint of the wave

    Vector2 *points = malloc(newNumSamples * sizeof(Vector2));

    for (int i = 0; i < newNumSamples; i++) {
        points[i] =
                (Vector2) {(float) i * (float) drawFactor / (float) sampleRate / 2 * 50,
                           (float) screenHeight / 2 - waveData[i * drawFactor] * 100};
    }
    DrawLineStrip(points, newNumSamples, DARKGRAY);
    free(points); // Don't forget to free the allocated memory*/
}

void SetGuiStyles() {
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BLACK));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
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
            waves[index].frameCount == 0 || waves[index].sampleRate != 44100 && waves[index].sampleRate != 48000) {

            filteredFiles[index] = NULL; // Set filteredFiles[index] to NULL
            UnloadWave(waves[index]);
            sem_post(&sem_fileLoader); // Release the semaphore
            return (void *) -1; // return -1 for failure
        }

        tracks[index] = LoadMusicStream(path);
        filteredFiles[index] = path;
        totalDurations[index] = GetMusicTimeLength(tracks[index]);
        filesLoaded++;
    }

    sem_post(&sem_fileLoader);
    return (void *) 0; // return 0 for success
}

void DrawAllWaveforms() {
    waveforms = malloc(waveCount * sizeof(RenderTexture2D));
    for (int i = 0; i < waveCount; i++) {
        waveforms[i] = LoadRenderTexture(
                (int) roundf((float) ((float) (waves[i].frameCount) / (float) waves[i].sampleRate) * 50), 500);
        BeginTextureMode(waveforms[i]);
        DrawSong(waves[i].data, (int) waves[i].frameCount, 50,
                 (int) waves[i].sampleRate);
        EndTextureMode();

        //Draw Progress bar
        DrawRectangle(150, screenHeight / 2 + 20,
                      (screenWidth - 300) * i / waveCount, 20,
                      DARKGRAY); // Draw the progress bar

    }

    currentState = PLAYING;

}

void LoadFiles() {
    //printf("Loading files from directory\n");
    directoryPath = tinyfd_selectFolderDialog("Select a directory", NULL);
    files = LoadDirectoryFiles(directoryPath);
}

void CountAudioFiles() {
    //printf("Counting audio files\n");
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
            pthread_create(&threads[audioFileIndex], NULL, LoadMusic, &threadData[audioFileIndex]);
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
}

void SeekInMusicStream(const int *index, float seconds) {
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

    LoadFiles(); // Load files from directory
    CountAudioFiles();
    LoadAllMusic(); // Load all music from the files
    RefreshDataAllocation(); // Refresh the memory allocation for the dinamic arrays, removing the corrupted files
    currentState = LOADING_WAVEFORMS;
    return NULL;
}

void ManageInputs() {

    if (IsKeyPressed(KEY_P)) { // Play next track
        PlayNextTrack(&currentTrack);
        elapsedTime = 0.0f;
        playing = true;
    }

    // if press esc close window
    if (IsKeyPressed(KEY_ESCAPE)) {
        exitWindow = true;
    }

    // advance 5 seconds when scrolling up with mosue wheel
    if (IsKeyPressed(KEY_RIGHT) || GetMouseWheelMove() > 0.0f) {
        SeekInMusicStream(&currentTrack, 5.0f);
    }

    // rewind 5 seconds when left arrow
    if (IsKeyPressed(KEY_LEFT) || GetMouseWheelMove() < 0.0f) {
        SeekInMusicStream(&currentTrack, -5.0f);
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
        musicVolume += (musicVolume < 1.0f) ? 0.05f : 0.0f; // Increase music volume (up to 1.0f
        musicVolume = roundf(musicVolume * 20) / 20;
        SetMasterVolume(powf(musicVolume, 2));
    }

    if (IsKeyPressed(KEY_DOWN)) {
        musicVolume -= (musicVolume > 0.0f) ? 0.05f : 0.0f; // Decrease music volume (up to 0.0f
        musicVolume = roundf(musicVolume * 20) / 20;
        SetMasterVolume(powf(musicVolume, 2));
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        exit(0);
    }
}

void UpdateTitles() {
    textHeaderPosition -= 1; // Adjust the value to change the speed
    if (textHeaderPosition +
        MeasureText(TextFormat("Playing: %s",
                               GetFileName(filteredFiles[currentTrack])),
                    20) <
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
                            GetFileName(
                                    filteredFiles[(currentTrack + 1) % waveCount])),
                    20) <
            0) {
            nextSongTextPosition = screenWidth;
        }
    }
}

void DrawUI() {
    
    DrawLine(screenWidth / 2, 25, screenWidth / 2, screenHeight,
             WHITE); // Draw the bar from top to bottom of the screen

    DrawRectangleGradientV(0, 0, screenWidth, (screenHeight / 2) - 20, BLACK,
                           BLANK); // Draw the top gradient
    DrawRectangleGradientV(0, (screenHeight / 2) + 20, screenWidth, screenHeight / 2, BLANK,
                           BLACK); // Draw the bottom gradient

    //Draw a little header for the title
    DrawRectangle(0, 0, screenWidth, 40, (Color) {10, 10, 10, 255});
    DrawText(
            TextFormat("Playing: %s", GetFileName(filteredFiles[currentTrack])),
            textHeaderPosition, 10, 20, LIGHTGRAY); // Draw the title of the song

    DrawText(TextFormat("Duration: %02d:%02d", (int) totalDurations[currentTrack] / 60,
                        (int) totalDurations[currentTrack] % 60),
             screenWidth -
             MeasureText(TextFormat("Duration: %02d:%02d",
                                    (int) totalDurations[currentTrack] / 60,
                                    (int) totalDurations[currentTrack] % 60),
                         20) -
             20,
             screenHeight - 50, 20, LIGHTGRAY); // Draw the duration of the song

    DrawText(TextFormat("Time: %02d:%02d", (int) elapsedTime / 60,
                        (int) elapsedTime % 60),
             20, screenHeight - 50, 20, LIGHTGRAY); // Draw the current time of the song



    // Draw index x of total
    DrawText(TextFormat("Track %d of %d", currentTrack + 1, waveCount), 20,
             screenHeight - 80, 20, LIGHTGRAY); // Draw the current track index


    if (totalDurations[currentTrack] - elapsedTime < 30.0f) {
        DrawText(TextFormat(
                         "Next: %s",
                         GetFileName(filteredFiles[(currentTrack + 1) % waveCount])),
                 nextSongTextPosition, 30, 20, LIGHTGRAY); // Draw the next song text header
    }


    float progress = ((float)screenWidth * elapsedTime) / totalDurations[currentTrack];
    DrawRectangle(0, screenHeight - 20, (int)progress, 20, DARKGRAY); // Draw the progress bar

    DrawText(TextFormat("%2d%%", (int) (roundf(musicVolume * 20) / 20 * 100)),
             30, screenHeight / 2, 20, RAYWHITE); // Draw the volume percentage

    //Show volume as a vertical bar on the left side of the screen
    float height = (float) (screenHeight - 200) * (musicVolume);
    DrawRectangle(5, (screenHeight - 100) - (int) height, 10, (int) height, LIGHTGRAY);
}


