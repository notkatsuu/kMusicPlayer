#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#include <synchapi.h>
#include <pthread.h>

#define A0 27.50f
#define Bb0 29.14f
#define B0 30.87f
#define C1 32.70f
#define Db1 34.65f
#define D1 36.71f
#define Eb1 38.89f
#define E1 41.20f
#define F1 43.65f
#define Gb1 46.25f
#define G1 49.00f
#define Ab1 51.91f
#define A1 55.00f
#define Bb1 58.27f
#define B1 61.74f
#define C2 65.41f
#define Db2 69.30f
#define D2 73.42f
#define Eb2 77.78f
#define E2 82.41f
#define F2 87.31f
#define Gb2 92.50f
#define G2 98.00f
#define Ab2 103.83f
#define A2 110.00f
#define Bb2 116.54f
#define B2 123.47f
#define C3 130.81f
#define Db3 138.59f
#define D3 146.83f
#define Eb3 155.56f
#define E3 164.81f
#define F3 174.61f
#define Gb3 185.00f
#define G3 196.00f
#define Ab3 207.65f
#define A3 220.00f
#define Bb3 233.08f
#define B3 246.94f
#define C4 261.63f
#define Db4 277.18f
#define D4 293.66f
#define Eb4 311.13f
#define E4 329.63f
#define F4 349.23f
#define Gb4 369.99f
#define G4 392.00f
#define Ab4 415.30f
#define A4 440.00f
#define Bb4 466.16f
#define B4 493.88f
#define C5 523.25f
#define Db5 554.37f
#define D5 587.33f
#define Eb5 622.25f
#define E5 659.25f
#define F5 698.46f
#define Gb5 739.99f
#define G5 783.99f
#define Ab5 830.61f
#define A5 880.00f
#define Bb5 932.33f
#define B5 987.77f
#define C6 1046.50f
#define Db6 1108.73f
#define D6 1174.66f
#define Eb6 1244.51f
#define E6 1318.51f
#define F6 1396.91f
#define Gb6 1479.98f
#define G6 1567.98f
#define Ab6 1661.22f
#define A6 1760.00f
#define Bb6 1864.66f
#define B6 1975.53f
#define C7 2093.00f
#define Db7 2217.46f
#define D7 2349.32f
#define Eb7 2489.02f
#define E7 2637.02f
#define F7 2793.83f
#define Gb7 2959.96f
#define G7 3135.96f
#define Ab7 3322.44f
#define A7 3520.00f
#define Bb7 3729.31f
#define B7 3951.07f
#define C8 4186.01f

#define TWO_PI (2.0f * PI)

typedef struct {
  float* buffer;
  int length;
  float frequency;
  int sampleRate;
  float volume;
} SoundProperties;


void freeSound(SoundProperties sound) {
  free(sound.buffer);
}

SoundProperties createSound(int sampleRate, float wavePeriod, float frequency, float volume) {
  SoundProperties sound;
  sound.sampleRate = sampleRate;
  sound.frequency = frequency;
  sound.length = sampleRate / wavePeriod;
  sound.buffer = (float *)malloc(sound.length * sizeof(float));
  sound.volume = volume;
  return sound;
}



void generateDrumSound(float* buffer, int length, float decayFactor, float frequency, int sampleRate) {
  float increment = frequency * TWO_PI / sampleRate;
  float waveValue = 0.0f;
  for (int i = 0; i < length; i++){
    // Generate a low-frequency sine wave with harmonics
    float sineWave = 0.0f;
    for (int harmonic = 1; harmonic <= 3; harmonic++) {
      sineWave += sinf(waveValue * harmonic) / harmonic;
    }
    waveValue += increment;

    // Apply exponential decay
    float decay = expf(-decayFactor * i / length);

    buffer[i] = sineWave * decay;
  }
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
  float *args = (float *)arg;
  float note = args[0];
  float duration = args[1];

  // Check if the note is a rest
  if (note == 0.0f) {
    // If it's a rest, just wait for the duration of the rest
    Sleep(duration * 1000);  // Sleep function expects milliseconds
  } else {
    // If it's not a rest, play the note as usual
    SoundProperties noteSoundProps = createSound(48000, duration, note, 0.04f);
    generatePianoSound(noteSoundProps.buffer, noteSoundProps.length,
                       noteSoundProps.frequency, noteSoundProps.sampleRate);
    Wave noteWave = {
        .frameCount = noteSoundProps.length,
        .sampleRate = noteSoundProps.sampleRate,
        .sampleSize = 32,
        .channels = 1,
        .data = noteSoundProps.buffer
    };
    Sound noteSound = LoadSoundFromWave(noteWave);
    SetSoundVolume(noteSound, noteSoundProps.volume);
    PlaySound(noteSound);
  }

  return NULL;
}


void main(){
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "Sound Generation");

  InitAudioDevice();

  // Variables for timing
  float BPM = 90.0f;  // Beats per minute
  float beatDuration = 30.0f / BPM;  // Duration of a single beat in seconds
  float timeSinceLastBeat = 0.0f;
  int currentNote = 0;
  float timeSinceLastNote = 0.0f;
  float currentNoteDuration = 0.0f;

  while (!WindowShouldClose()){
    // Update the timer
    timeSinceLastBeat += GetFrameTime();
    timeSinceLastNote += GetFrameTime();

    // If the time since the last note exceeds the note duration, play the next note
    if (timeSinceLastNote >= currentNoteDuration){
      // Define the notes and durations for the first part of the song
      float notes1[] = {C4, C4, G4, G4, A4, A4, G4};
      float notes2[] = {E4, 0.0f, B4, 0.0f, C5, C5, B4};
      float durations[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f};  // In beats

      // Calculate the duration of a single beat in seconds
      float beatDuration = 60.0f / BPM;

      // Start a new thread to play the note
      pthread_t thread1, thread2;
      float args1[] = {notes1[currentNote], durations[currentNote] * beatDuration};
      float args2[] = {notes2[currentNote], durations[currentNote] * beatDuration};
      pthread_create(&thread1, NULL, playNote, args1);
      pthread_create(&thread2, NULL, playNote, args2);

      // Update the current note duration and reset the timer
      currentNoteDuration = durations[currentNote] * beatDuration;
      timeSinceLastNote = 0.0f;

      // Move to the next note
      currentNote = (currentNote + 1) % (sizeof(notes1) / sizeof(float));
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  CloseWindow();
}