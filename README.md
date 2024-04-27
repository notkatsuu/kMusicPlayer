# kMusicPlayer

### kMusicPlayer is a visual music player built with C and Raylib. It provides a simple visual interface for playing your favorite music tracks.
### The target of this project is developing a software to be able to enjoy music also in a visual way.
# 

<p align="center">
  <img src="images/demo.gif" alt="Main Interface of kMusicPlayer" width="400">
  <br>
  <i style="font-size: 12px;">kMusicPlayer Interface</i>
</p>

# Features

- **Music Playback**: Play your favorite tracks with ease. Supports `.mp3` and `.wav` formats.
- **Waveform Visualization**: Visualize your music with waveform rendering.
- **Threaded Music Loading**: Load your music fast with threaded loading.
- **Simple Controls**: Control playback with simple keyboard shortcuts using RayGUI.

# How to Use

## 1. **Installation**:
- Download the latest release from the [Releases](https://github.com/notkatsuu/kMusicPlayer/releases) page.
- Extract the contents of the archive to a folder of your choice.
- Run the executable file to start the application. 

The program will ask you to select a folder containing your music files. Select a folder and click "Open".

It will load all music files in the selected and will store their waveforms in the "cache/waveforms" folder.

## 2. **Controls**:
### All the controls available in the application are implemented with RayGUI. The following are the available controls:
- **Play/Pause**: Click the play/pause button or press `Space` to play or pause the current track.
- **Next Track**: Click the next track button or press `P` to skip to the next track.
- **Previous Track**: Click the previous track button to go back to the previous track.
- **Volume Control**: Use the volume slider to adjust the volume of the current track.
- **Seek Track**: Use the seek slider or Right/Left Arrows to seek to a specific position in the current track.
- **Info Button**: Click the info button to view information about the current track.
- **Theme Button**: Click the theme button to switch between light and dark themes.


## 3. **View Waveform**: 
#### The waveform of the current track is displayed in the main window. It shows the amplitude of the audio signal over time. The waveform is loaded at the start of the track and is displayed in real-time as the track plays.
#### All Waveforms are stored in the "cache/waveforms" folder. If you want to delete them, you can do it manually. The program will generate them again when you play the track.
![Waveform Visualization](images/waveform.png "Waveform Visualization")
<p align="center" style="font-size: 12px;"><i>Frank Sinatra's Come Fly With Me</i></p>

## 4. **Themes**:
#### The application uses RayGui and supports a variety of themes to customize the look and feel of the interface. 
#### You can switch between themes by clicking the theme button in the top-right corner of the window. 


# Building from Source

### kMusicPlayer uses CMake for its build system. To build from source, follow these steps:

### 1. Clone the repository: `git clone https://github.com/your_username/kMusicPlayer.git`
### 2. Navigate to the project directory: `cd kMusicPlayer`
### 3. Create a build directory and navigate to it: `mkdir build && cd build`
### 4. Run CMake to generate the build files: `cmake ..`
### 5. Build the project: `cmake --build .`

# License


### This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License. See the `LICENSE` file for details.
