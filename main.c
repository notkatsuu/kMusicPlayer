#include <stdlib.h>

#include "tinyfiledialogs.h"
#include <raylib.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION

#include "pthread.h"
#include "raygui.h"
#include "semaphore.h"

#define MAX_LOADING_THREADS 4

void setGuiStyles();

void loadFiles();

void countAudioFiles();

void loadAllMusic();

void refreshDataAllocation();

void DrawSong(const float *waveData, int numSamples, int drawFactor,
              int sampleRate);

void loadAllWaveforms();

void *loadMusic(void *arg);

void playNextTrack(int *currentTrack);

void seekInMusicStream(const int *currentTrack, float seconds);

const int screenWidth = 800;
const int screenHeight = 450;
char const *directoryPath;

Wave *waves; //Array for storing wave data
Music *tracks; //Array for storing music tracks
FilePathList files; //Raw list of files in the directory
RenderTexture2D *waveforms; //Array for storing waveforms' textures
float *totalDurations; //Array for storing the total duration of each song

sem_t sem_fileLoader;

int waveCount = 0;
float elapsedTime = 0.0f;
float musicVolume = 0.5f;

typedef struct {
    char *path;
    int index;
} ThreadData;

char **filteredFiles;

int main(void) {

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "kMusicPlayer");

    setGuiStyles();

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    sem_init(&sem_fileLoader, 0, MAX_LOADING_THREADS); //Set up semaphore for file loading
    Vector2 mousePosition = {0};
    Vector2 panOffset = mousePosition;

    bool dragWindow = false;
    bool exitWindow = false;

    Vector2 windowPosition = {500, 200}; // Set window position on startup
    SetWindowPosition((int) windowPosition.x, (int) windowPosition.y);

    //Draw a enter directory screen
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText("Please enter your music folder directory", screenWidth / 2 - MeasureText("Please enter your music folder directory", 20) / 2,
             screenHeight / 2 - 20, 20, RAYWHITE);
    EndDrawing();


    RenderTexture2D camTarget = LoadRenderTexture(screenWidth, screenHeight); // Load a render texture to draw the camera target in it

    InitAudioDevice();
    SetMasterVolume(musicVolume);
    bool playing = false;

    // Loading files functions-----------------
    loadFiles(); // Load files from directory
    countAudioFiles();

    //Draw a loading screen
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Loading music files...", screenWidth / 2 - MeasureText("Loading music files...", 20) / 2,
             screenHeight / 2 - 20, 20, RAYWHITE);
    EndDrawing();

    loadAllMusic(); // Load all music from the files
    refreshDataAllocation(); // Refresh the memory allocation for the dinamic arrays, removing the corrupted files
    //-----------------------------------------

    //load waveforms
    printf("Loading waveforms\n");
    loadAllWaveforms(); // Load all waveforms of the music into array of render textures

    int currentTrack = 0; // To track the song that's playing



    totalDurations = malloc(waveCount * sizeof(float));

    for (int i = 0; i < waveCount; i++) {
        totalDurations[i] = (float) waves[i].frameCount /
                            (float) waves[i].sampleRate;
    }


    // Initialize camera
    Camera2D camera =
            (Camera2D) {{(float) screenWidth / 2, (float) screenHeight / 2},
                        {(float) screenWidth / 2, (float) screenHeight / 2},
                        0.0f,
                        0.75f};

    int textHeaderPosition = screenWidth; //Starting position for the title text (comes from the right to left)
    int nextSongTextPosition = screenWidth; //Starting position for the next song text
    elapsedTime = 0.0f; // To track the current time of the song

    PlayMusicStream(tracks[currentTrack]); // Play the first song
    PauseMusicStream(tracks[currentTrack]); // Pause the first song (to be played when the user presses space)

    // Main game loop
    while (!exitWindow && !WindowShouldClose()) {
        if (playing) {
            elapsedTime += GetFrameTime(); // Update the elapsed time
            if (elapsedTime >= totalDurations[currentTrack]) { // If the song ends, play the next one, updating the duration, elapsed time, etc.
                StopMusicStream(tracks[currentTrack]);
                elapsedTime = 0.0f;
                nextSongTextPosition = screenWidth;
                currentTrack = (currentTrack + 1) % waveCount;
                PlayMusicStream(tracks[currentTrack]);
            }
        }

        mousePosition = GetMousePosition();
        UpdateMusicStream(
                tracks[currentTrack]); // Update music stream with new track

        if (IsKeyPressed(KEY_P)) { // Play next track
            playNextTrack(&currentTrack);
            elapsedTime = 0.0f;
            playing = true;
        }

        // if press esc close window
        if (IsKeyPressed(KEY_ESCAPE)) {
            exitWindow = true;
        }

        // advance 5 seconds when scrolling up with mosue wheel
        if (IsKeyPressed(KEY_RIGHT ) ||  GetMouseWheelMove() > 0.0f){
            seekInMusicStream(&currentTrack, 5.0f);
        }

        // rewind 5 seconds when left arrow
        if (IsKeyPressed(KEY_LEFT) || GetMouseWheelMove() < 0.0f){
            seekInMusicStream(&currentTrack, -5.0f);
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

        if (IsKeyPressed(KEY_UP) && musicVolume < 1.0f) {
            musicVolume += 0.05f;
            musicVolume = roundf(musicVolume * 20) / 20;
            SetMasterVolume(powf(musicVolume, 2));
        }

        if (IsKeyPressed(KEY_DOWN) && musicVolume > 0.0f) {
            musicVolume -= 0.05f;
            musicVolume = roundf(musicVolume * 20) / 20;
            SetMasterVolume(powf(musicVolume, 2));
        }

        // Update camera target position
        camera.target.x = elapsedTime * 100;
        camera.offset = (Vector2) {(float) screenWidth / 2,
                                   (float) screenHeight / 2}; // Center the camera

        camera.zoom = 2.0f; // Set camera zoom to 1.0f
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

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) { // Start dragging the window
            if (CheckCollisionPointRec(mousePosition,
                                       (Rectangle) {0, 0, (float) screenWidth, 20})) {
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

        BeginDrawing();
        BeginMode2D(camera);    // Begin 2D mode with camera
        ClearBackground(BLANK); // Clear the texture background

        DrawTextureRec(waveforms[currentTrack].texture,
                       (Rectangle) {0, 0,
                                    (float) waveforms[currentTrack].texture.width,
                                    (float) -waveforms[currentTrack].texture.height},
                       (Vector2) {0, 0}, WHITE);


        EndMode2D(); // End 2D mode

        DrawLine(screenWidth / 2, 25, screenWidth / 2, screenHeight,
                 WHITE); // Draw the bar from top to bottom of the screen

        DrawRectangleGradientV(0, 0, screenWidth, (screenHeight/2)-20, BLACK, BLANK); // Draw the top bar
        DrawRectangleGradientV(0, (screenHeight/2)+20, screenWidth, screenHeight/2, BLANK, BLACK); // Draw the bottom bar
        DrawText(
                TextFormat("Playing: %s", GetFileName(filteredFiles[currentTrack])),
                textHeaderPosition, 10, 20, DARKGRAY); // Draw the title of the song
        DrawText(TextFormat("Duration: %02d:%02d", (int) totalDurations[currentTrack] / 60,
                            (int) totalDurations[currentTrack] % 60),
                 screenWidth -
                 MeasureText(TextFormat("Duration: %02d:%02d",
                                        (int) totalDurations[currentTrack] / 60,
                                        (int) totalDurations[currentTrack] % 60),
                             20) -
                 10,
                 screenHeight - 50, 20, DARKGRAY); // Draw the duration of the song
        DrawText(TextFormat("Time: %02d:%02d", (int) elapsedTime / 60,
                            (int) elapsedTime % 60),
                 10, screenHeight - 50, 20, DARKGRAY); // Draw the current time of the song
        // Draw index x of total
        DrawText(TextFormat("Track %d of %d", currentTrack + 1, waveCount), 10,
                 screenHeight - 80, 20, DARKGRAY); // Draw the current track index
        if (totalDurations[currentTrack] - elapsedTime < 30.0f) {
            DrawText(TextFormat(
                             "Next: %s",
                             GetFileName(filteredFiles[(currentTrack + 1) % waveCount])),
                     nextSongTextPosition, 30, 20, DARKGRAY); // Draw the next song text header
        }
        DrawRectangle(0, screenHeight - 20,
                      screenWidth * elapsedTime / totalDurations[currentTrack], 20, DARKGRAY); // Draw the progress bar

        DrawText(TextFormat("Volume: %02d%%", (int)(roundf(musicVolume * 20) / 20 * 100)),
                 10, screenHeight - 110, 20, DARKGRAY); // Draw the volume percentage

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

    CloseAudioDevice(); // Close audio device (music streaming is automatically
    // stopped)
    CloseWindow();      // Close window and OpenGL context

    return 0;
}

void DrawSong(const float *waveData, int numSamples, int drawFactor,
              int sampleRate) {

    int newNumSamples = numSamples / drawFactor * 2;
    printf("New num samples: %d\n", newNumSamples);
    Vector2 *points = malloc(newNumSamples * sizeof(Vector2));
    for (int i = 0; i < newNumSamples; i++) {
        // Calculate the position of the point
        //print point x of totalpoints

        points[i] =
                (Vector2) {(float) i * (float) drawFactor / (float) sampleRate / 2 * 100,
                           (float) screenHeight / 2 - waveData[i * drawFactor] * 200};
    }
    printf("Drawing line strip\n");
    DrawLineStrip(points, newNumSamples, DARKGRAY);
    free(points); // Don't forget to free the allocated memory*/
}

void setGuiStyles() {
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BLACK));
}

void *loadMusic(void *arg) { // Thread function to load music
    sem_wait(&sem_fileLoader);
    ThreadData *data = (ThreadData *) arg;
    char *path = data->path;
    int index = data->index;

    printf("Loading file %s\n", path);
    if (IsFileExtension(path, ".mp3") || IsFileExtension(path, ".wav")) {
        Wave candidateWave = LoadWave(path);

        if (candidateWave.data == NULL || candidateWave.sampleRate == 0 ||
            candidateWave.sampleSize == 0 || candidateWave.channels == 0 ||
            candidateWave.frameCount == 0) {
            printf("Corrupted file %s\n", path);
            waveCount--;
            UnloadWave(candidateWave);
            sem_post(&sem_fileLoader);
            return (void *) -1; // return -1 for failure
        }

        waves[index] = LoadWave(path);
        tracks[index] = LoadMusicStream(path);
        filteredFiles[index] = path;

        printf("\n\nLoaded file %s\n", path);
        printf("Wave frame count: %i\n", (int) waves[index].frameCount);
        printf("Wave sample rate: %i\n\n", (int) waves[index].sampleRate);

        UnloadWave(candidateWave);
    }
    sem_post(&sem_fileLoader);
    return (void *) 0; // return 0 for success
}

void loadAllWaveforms() {
    for (int i = 0; i < waveCount; i++) {
        //print waveform x of total waveforms
        printf("\n\nLoading waveform %d of %d\n", i+1, waveCount);
        //printf("Load file %s\n", filteredFiles[i]); Gives exception on corrupt examples (Work in progress)
        printf("Wave frame count: %i\n", (int) waves[i].frameCount);
        printf("Wave sample rate: %i\n", (int) waves[i].sampleRate);


        if (waves[i].sampleRate == 0 ||waves[i].frameCount == 0 || (waves[i].sampleRate != 44100 && waves[i].sampleRate != 48000)) {
            printf("Corrupted file %s\n", filteredFiles[i]); //This should be implemented on the filter on loadFiles but haven't figured out why it doesn't really work...
            continue;
        }

        waveforms[i] = LoadRenderTexture(
                (int) roundf(((waves[i].frameCount) / waves[i].sampleRate) * 100), 500);
        //print texture size
        printf("Texture size: %d x %d\n", waveforms[i].texture.width, waveforms[i].texture.height);

        BeginTextureMode(waveforms[i]);
        ClearBackground(BLACK);
        printf("Drawing DrawSong %d\n", i);
        DrawSong(waves[i].data, (int) waves[i].frameCount, 100,
                 (int) waves[i].sampleRate);
        printf("End DrawSong %d\n", i);
        EndTextureMode();

    }

}

void loadFiles() {
    directoryPath = tinyfd_selectFolderDialog("Select a directory", NULL);

    files = LoadDirectoryFiles(directoryPath);
}

void countAudioFiles() {
    for (int i = 0; i < files.count; i++) {
        if (IsFileExtension(files.paths[i], ".mp3") ||
            IsFileExtension(files.paths[i], ".wav")) {
            waveCount++;
        }
    }
}

void loadAllMusic() { //calls the loadMusic function for each file
    filteredFiles = malloc(waveCount * sizeof(char *));
    waves = malloc(waveCount * sizeof(Wave));
    tracks = malloc(waveCount * sizeof(Music));

    pthread_t *threads = malloc(files.count * sizeof(pthread_t));
    ThreadData *threadData = malloc(files.count * sizeof(ThreadData));

    for (int i = 0; i < files.count; i++) {
        threadData[i].path = files.paths[i];
        threadData[i].index = i;
        pthread_create(&threads[i], NULL, loadMusic, &threadData[i]);
    }

    for (int i = 0; i < files.count; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(threadData);
}

void refreshDataAllocation() { //reallocates memory for the dinamic arrays
    char **tempFilteredFiles = realloc(filteredFiles, waveCount * sizeof(char *));
    if (tempFilteredFiles == NULL) {
        // handle error, e.g., by freeing filteredFiles and setting it to NULL
        free(filteredFiles);
        filteredFiles = NULL;
    } else {
        filteredFiles = tempFilteredFiles;
    }

    Music *tempTracks = realloc(tracks, waveCount * sizeof(Music));
    if (tempTracks == NULL) {
        // handle error, e.g., by freeing filteredFiles and setting it to NULL
        free(tracks);
        tracks = NULL;
    } else {
        tracks = tempTracks;
    }

    Wave *tempWaves = realloc(waves, waveCount * sizeof(Wave));
    if (tempWaves == NULL) {
        // handle error, e.g., by freeing filteredFiles and setting it to NULL
        free(waves);
        waves = NULL;
    } else {
        waves = tempWaves;
    }
    RenderTexture2D *tempWaveforms = realloc(waveforms, waveCount * sizeof(RenderTexture2D));
    if (tempWaveforms == NULL) {
        // handle error, e.g., by freeing filteredFiles and setting it to NULL
        free(waveforms);
        waveforms = NULL;
    } else {
        waveforms = tempWaveforms;
    }
}

void playNextTrack(int *currentTrack) {
    StopMusicStream(tracks[*currentTrack]);
    *currentTrack = (*currentTrack + 1) % waveCount;
    tracks[*currentTrack] = tracks[*currentTrack];
    PlayMusicStream(tracks[*currentTrack]);
    waves[*currentTrack] = waves[*currentTrack];
}

void seekInMusicStream(const int *currentTrack, float seconds) {
    elapsedTime += seconds;
    if (elapsedTime > totalDurations[*currentTrack]) {
        elapsedTime = totalDurations[*currentTrack];
    } else if (elapsedTime < 0.0f) {
        elapsedTime = 0.0f;
    }

    SeekMusicStream(tracks[*currentTrack],
                    elapsedTime); // Seek to the new position
}
