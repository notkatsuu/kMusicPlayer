#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#include <pthread.h>
#include <notes.h>

#define TWO_PI (2.0f * PI)
#define MAX_BUFFERS 100
#define SAMPLE_RATE 48000
#define VOLUME 0.04f

typedef struct {
  float* buffer;
  int length;
  float frequency;
  int sampleRate;
  float volume;
} SoundProperties;

typedef struct {
  float frequency;
  float moment;
  float duration;
} Note;

typedef struct NoteNode {
  Note note;
  bool played;
  struct NoteNode* next;
} NoteNode;

SoundProperties bufferPool[MAX_BUFFERS];
int nextBuffer = 0;
float BPM = 90.0f;  // Beats per minute
float timeCounter = 0.0f;
float loopTime = 30.0f;

// Function declarations
void freeSound(SoundProperties sound);
void createSound(SoundProperties* sound, int sampleRate, float wavePeriod, float frequency, float volume);
SoundProperties* getBuffer();
void generatePianoSound(float* buffer, int length, float frequency, int sampleRate);
void *playNote(void *arg);

// Main function
void main(){
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "Sound Generation");
  InitAudioDevice();

  NoteNode* notes1 = NULL;
  NoteNode* lastNote = NULL;

  // Initialize the notes
  Note notesArray[] = {{C3, 0.0f, 1.0f}, {E3, 1.0f, 1.0f}, {G3, 2.0f, 1.0f}, {C4, 3.0f, 1.0f}, {G3, 4.0f, 1.0f}, {E3, 5.0f, 1.0f}};
  for (int i = 0; i < sizeof(notesArray) / sizeof(Note); i++) {
    NoteNode* newNode = (NoteNode*)malloc(sizeof(NoteNode));
    newNode->note = notesArray[i];
    newNode->played = false;
    newNode->next = NULL;
    if (lastNote == NULL) {
      notes1 = newNode;
    } else {
      lastNote->next = newNode;
    }
    lastNote = newNode;
  }

  //set looptime to the moment + duration of the last note
  loopTime = lastNote->note.moment + lastNote->note.duration;
  loopTime*= (60.0f / BPM);

  while (!WindowShouldClose()){
    // Update the timer
    float frameTime = GetFrameTime();
    timeCounter += frameTime;
    pthread_t thread1;

    for (NoteNode* node = notes1; node != NULL; node = node->next) {
      // Only play the note if its moment has come and it hasn't been played yet
      if (fmod(timeCounter, loopTime) >= node->note.moment * (60.0f / BPM) && !node->played){
        // Wait for the previous note's thread to finish before starting a new one
        if (node != notes1) {
          pthread_join(thread1, NULL);
        }
        // Start a new thread to play the note
        node->played = true;
        pthread_create(&thread1, NULL, playNote, &node->note);
      }
    }

    // Reset the notes1Played array and the timeCounter variable when the loop restarts
    if (timeCounter >= loopTime) {
      for (NoteNode* node = notes1; node != NULL; node = node->next) {
        node->played = false;
      }
      timeCounter -= loopTime;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }

  CloseWindow();
}

// Function definitions
void freeSound(SoundProperties sound) {
  free(sound.buffer);
}

void createSound(SoundProperties* sound, int sampleRate, float wavePeriod, float frequency, float volume) {
  sound->sampleRate = sampleRate;
  sound->frequency = frequency;
  sound->length = sampleRate / wavePeriod;
  sound->buffer = (float *)malloc(sound->length * sizeof(float));
  sound->volume = volume;
}

SoundProperties* getBuffer() {
  if (nextBuffer >= MAX_BUFFERS) {
    nextBuffer = 0;  // Wrap around to the start of the pool
  }
  return &bufferPool[nextBuffer++];
}

void generatePianoSound(float* buffer, int length, float frequency, int sampleRate) {
  // Parameters for the piano sound
  int numOvertones = 6;  // Number of overtones
  float decayFactor = 0.0004f * 2.0f * PI * frequency;  // Decay factor for the exponential decay

  // Generate the fundamental frequency and overtones
  for (int i = 0; i < length; i++) {
    float time = (float)i / sampleRate;
    float value = 0.0f;
    for (int overtone = 1; overtone <= numOvertones; overtone++) {
      float overtoneFrequency = frequency * overtone;
      float increment = overtoneFrequency * TWO_PI;
      value += sinf(increment * time) * expf(-decayFactor * time * overtone) / powf(2, overtone - 1);
    }

    // Add saturation
    value += value * value * value;

    // Apply an envelope to make the sound more realistic
    value *= 1 + 16 * time * expf(-6 * time);

    buffer[i] = value;
  }
}

void *playNote(void *arg) {
  // Extract the note and duration from the argument
  Note *note = (Note *)arg;

  // Convert the duration from beats to seconds
  float durationInSeconds = note->duration * (60.0f / BPM);

  // If it's not a rest, play the note as usual
  SoundProperties* noteSoundProps = getBuffer();
  createSound(noteSoundProps, SAMPLE_RATE, durationInSeconds, note->frequency, VOLUME);
  generatePianoSound(noteSoundProps->buffer, noteSoundProps->length,
                     noteSoundProps->frequency, noteSoundProps->sampleRate);
  Wave noteWave = {
      .frameCount = noteSoundProps->length,
      .sampleRate = noteSoundProps->sampleRate,
      .sampleSize = 32,
      .channels = 1,
      .data = noteSoundProps->buffer
  };
  Sound noteSound = LoadSoundFromWave(noteWave);
  SetSoundVolume(noteSound, noteSoundProps->volume);
  PlaySound(noteSound);

  return NULL;
}