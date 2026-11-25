# Encore
A tool for checking frame sequences for visual errors.

Written by hand in C++ using wxWidgets and OpenEXR.  Based on a prototype developed with GPT and Gemini ([repo here](https://github.com/TicTac-93/Encore))

Built and tested on Windows 11, using both G++ and MSVC compilers.

### How to use
- Download the latest build from the [Releases](https://github.com/TicTac-93/Encore/releases) section
- Copy the whole `bin` folder from the .zip to somewhere convenient.  Builds are currently portable, not installed.
- `Encore.exe` launches the program
- Browse to a folder containing frames (currently, only EXR files are supported).
  - Frame sequences will be detected and shown in the main UI.
  - Select a sequence to analyze, optionally adjust the "Processing Resolution" to improve speed and memory usage, and click "Load Frames"
- The program will switch to the "Review Frames" tab, and automatically calculate a starting threshold to filter the frames.
  - Filtering threshold can be set manually by unchecking the "Auto" box on the right
  - You can mark individual frames as OK or an Error, overriding any threshold-based filtering
  - Clicking thumbnails will expand them to their loaded sized, based on the chosen "Processing Resolution"
  -  Frames either caught by the filter or marked as Error will be included in the list at the bottom of the UI, which can be used for re-submitting renders or other patches
- A new sequence can be loaded by returning to the "Select Shots" tab and either navigating to a new folder or picking a new sequence from the list.

### Known limitations
- The detection algorithm will sometimes generate false-positives on an OK frame that is next to a glitch
  - It may be possible to filter this out, but a few false positives are easy for the user to review and manually flag, and is better than missing a legitimate error in the filter.
- Currently, only EXR files are supported
- For use in a production environment, an API for automation or a method for processing multiple frame folders simultaneously would be invaluable.

### Attribution
- Graphic assets for the logo from [Muhammad Usman](https://www.flaticon.com/free-icons/magnifying-glass) and [Magnific](https://www.flaticon.com/free-icons/motion)
- Demo frames generated using "Mr. Elephant" blender scene by [Glenn Melenhorst](http://www.glennmelenhorst.com)