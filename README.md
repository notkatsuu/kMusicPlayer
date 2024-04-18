# kMusicPlayer

kMusicPlayer is a powerful music player built with C and Raylib. It provides a sleek and intuitive interface for playing your favorite music tracks.

![Main Interface](images/demo.gif)
*<center>kMusicPlayer Main Interface</center>*
## Features

- **Music Playback**: Play your favorite tracks with ease. Supports `.mp3` and `.wav` formats.
- **Waveform Visualization**: Visualize your music with waveform rendering.
- **Threaded Music Loading**: Load your music in the background without interrupting your workflow.

## How to Use

1. **Load Music**: Use the file dialog to select your music directory. kMusicPlayer will automatically load all `.mp3` and `.wav` files in the directory.

2. **Control Playback**: Use the playback controls to play, pause, and skip tracks. You can also seek through the current track by clicking on the waveform.

    ![Control Playback](path_to_your_image)

3. **View Waveform**: The waveform of the current track is displayed in the main window. It updates in real-time as the track plays.

    ![View Waveform](images/waveform.png)
<p align="center" style="font-size: 12px;"><i>Frank Sinatra's Come Fly With Me</i></p>

## Building from Source

kMusicPlayer uses CMake for its build system. To build from source, follow these steps:

1. Clone the repository: `git clone https://github.com/your_username/kMusicPlayer.git`
2. Navigate to the project directory: `cd kMusicPlayer`
3. Create a build directory and navigate to it: `mkdir build && cd build`
4. Run CMake to generate the build files: `cmake ..`
5. Build the project: `cmake --build .`

## License


This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License. See the `LICENSE` file for details.
