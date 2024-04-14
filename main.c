#include <stdlib.h>

#include "tinyfiledialogs.h"
#include <raylib.h>
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <pthread.h>
#include <semaphore.h>

#define MAX_LOADING_THREADS 4

void setGuiStyles();
void loadFiles();
void countAudioFiles();
void loadAllMusic();
void refreshDataAllocation();
void DrawSong(const float *waveData, int numSamples, int drawFactor,
              int sampleRate);
void loadAllWaveforms(RenderTexture2D *waveforms, Wave *waveToDraw, int count);
void *loadMusic(void *arg);
void playNextTrack(int *currentTrack);
void seekInMusicStream(int *currentTrack, float seconds);

const int screenWidth = 800;
const int screenHeight = 450;

Wave *waves;
Music *tracks;
FilePathList files;
char const *directoryPath;

pthread_mutex_t waveCountMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_fileLoader;

int waveCount = 0;
float elapsedTime = 0.0f;

typedef struct {
  char *path;
  int index;
} ThreadData;

char **filteredFiles;

int main(void) {

  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(screenWidth, screenHeight, "Practica 4 - Raylib Maze 3D");

  setGuiStyles();

  SetTargetFPS(144); // Set our game to run at 60 frames-per-second
  sem_init(&sem_fileLoader, 0, MAX_LOADING_THREADS);
  Vector2 mousePosition = {0};
  Vector2 panOffset = mousePosition;

  bool dragWindow = false;
  bool exitWindow = false;

  Vector2 windowPosition = {500, 200};
  SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

  RenderTexture2D camTarget = LoadRenderTexture(screenWidth, screenHeight);

  InitAudioDevice();     // Initialize audio device
  SetMasterVolume(1.0f); // Set volume for music (1.0 is max level
  bool playing = true;

  loadFiles();
  countAudioFiles();
  loadAllMusic();
  refreshDataAllocation();

  RenderTexture2D *waveforms = malloc(waveCount * sizeof(RenderTexture2D));

  loadAllWaveforms(waveforms, waves, waveCount);

  int currentTrack = 0;

  elapsedTime = 0.0f;
  float totalDuration = (float)waves[currentTrack].frameCount /
                        (float)waves[currentTrack].sampleRate;

  PlayMusicStream(tracks[currentTrack]); // Start music playing

  // Initialize camera
  Camera2D camera =
      (Camera2D){{(float)screenWidth / 2, (float)screenHeight / 2},
                 {(float)screenWidth / 2, (float)screenHeight / 2},
                 0.0f,
                 0.75f};

  // Main game loop
  while (!exitWindow && !WindowShouldClose()) {
    if (playing) {
      elapsedTime += GetFrameTime();
      if (elapsedTime >= totalDuration) {
        StopMusicStream(tracks[currentTrack]);
        currentTrack = (currentTrack + 1) % waveCount;
        tracks[currentTrack] = tracks[currentTrack];
        PlayMusicStream(tracks[currentTrack]);
        waves[currentTrack] = waves[currentTrack];
        totalDuration = (float)waves[currentTrack].frameCount /
                        (float)waves[currentTrack].sampleRate;
        elapsedTime = 0.0f;
      }
    }

    mousePosition = GetMousePosition();
    UpdateMusicStream(
        tracks[currentTrack]); // Update music buffer with new stream data

    if (IsKeyPressed(KEY_P)) {
      playNextTrack(&currentTrack);
      totalDuration = (float)waves[currentTrack].frameCount /
                      (float)waves[currentTrack].sampleRate;
      elapsedTime = 0.0f;
    }

    // advance 5 seconds when right arrow
    if (IsKeyPressed(KEY_RIGHT)) {
      seekInMusicStream(&currentTrack, 5.0f);
    }

    // rewind 5 seconds when left arrow
    if (IsKeyPressed(KEY_LEFT)) {
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

    // Update camera
    camera.target.x = elapsedTime / totalDuration *
                      (float)waves[currentTrack].frameCount /
                      (float)waves[currentTrack].sampleRate * 200;
    camera.offset = (Vector2){(float)screenWidth / 2,
                              (float)screenHeight / 2}; // Center the camera

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) {
      if (CheckCollisionPointRec(mousePosition,
                                 (Rectangle){0, 0, (float)screenWidth, 20})) {
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
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);    // Begin 2D mode with camera
    ClearBackground(BLANK); // Clear the texture background

    DrawTextureRec(waveforms[currentTrack].texture,
                   (Rectangle){0, 0,
                               (float)waveforms[currentTrack].texture.width,
                               (float)-waveforms[currentTrack].texture.height},
                   (Vector2){0, 0}, WHITE);

    EndMode2D(); // End 2D mode

    BeginDrawing();

    DrawLine(screenWidth / 2, 25, screenWidth / 2, screenHeight,
             WHITE); // Draw the bar from top to bottom of the screen

    DrawText(
        TextFormat("Playing: %s", GetFileName(filteredFiles[currentTrack])), 10,
        10, 20, DARKGRAY);
    DrawText(TextFormat("Duration: %.2f", totalDuration), 10, 30, 20, DARKGRAY);
    DrawText(TextFormat("Time: %.2f", elapsedTime), 10, 50, 20, DARKGRAY);
    DrawText(TextFormat("Zoom: %.2f", camera.zoom), 10, 70, 20, DARKGRAY);
    // Draw index x of total
    DrawText(TextFormat("Track %d of %d", currentTrack + 1, waveCount), 10, 90,
             20, DARKGRAY);

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

  int newNumSamples = (numSamples / drawFactor);
  Vector2 *points = malloc(newNumSamples * sizeof(Vector2));
  for (int i = 0; i < newNumSamples; i++) {
    
    points[i] =
        (Vector2){(float)i * (float)drawFactor / (float)sampleRate * 100,
                  (float)screenHeight / 2 - waveData[i * drawFactor] * 300};
  }
  DrawLineStrip(points, newNumSamples, DARKGRAY);
  free(points); // Don't forget to free the allocated memory

}

void setGuiStyles() {
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BLACK));
}

void *loadMusic(void *arg) {
  sem_wait(&sem_fileLoader);
  ThreadData *data = (ThreadData *)arg;
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
      return (void *)-1; // return -1 for failure
    }

    waves[index] = LoadWave(path);
    tracks[index] = LoadMusicStream(path);
    filteredFiles[index] = path;

    UnloadWave(candidateWave);
  }
  sem_post(&sem_fileLoader);
  return (void *)0; // return 0 for success
}

void loadAllWaveforms(RenderTexture2D *waveforms, Wave *waveToDraw, int count) {
  int drawFactor = 100;
  for (int i = 0; i < count; i++) {
    waveforms[i] = LoadRenderTexture(
        (int)(waveToDraw[i].frameCount / waveToDraw[i].sampleRate) * 150, 500);
    BeginTextureMode(waveforms[i]);
    ClearBackground(BLACK);
    DrawSong(waveToDraw[i].data, (int)waveToDraw[i].frameCount, 100,
             (int)waveToDraw[i].sampleRate);
    EndTextureMode();

    //export the waveform texture to a file
        char *filename = malloc(strlen(filteredFiles[i]) + 5);
        strcpy(filename, filteredFiles[i]);
        strcat(filename, ".png");
        ExportImage(LoadImageFromTexture(waveforms[i].texture), filename);
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

void loadAllMusic() {
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

void refreshDataAllocation() {
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
}

void playNextTrack(int *currentTrack) {
  StopMusicStream(tracks[*currentTrack]);
  *currentTrack = (*currentTrack + 1) % waveCount;
  tracks[*currentTrack] = tracks[*currentTrack];
  PlayMusicStream(tracks[*currentTrack]);
  waves[*currentTrack] = waves[*currentTrack];
}

void seekInMusicStream(int *currentTrack, float seconds) {
  elapsedTime += seconds;
  if (elapsedTime > waves[*currentTrack].frameCount / waves[*currentTrack].sampleRate) {
    elapsedTime = waves[*currentTrack].frameCount / waves[*currentTrack].sampleRate;
  } else if (elapsedTime < 0.0f) {
    elapsedTime = 0.0f;
  }
  StopMusicStream(tracks[*currentTrack]); // Stop the current music stream
  SeekMusicStream(tracks[*currentTrack],
                  elapsedTime); // Seek to the new position
  PlayMusicStream(
      tracks[*currentTrack]); // Start playing again from the new position
}
