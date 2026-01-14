# bad apple but every ___ frames its another version

you heart that correctly

usage: first install MSVC and cmake or GCC and cmake from visual studio 2022/2025 or idk where
to use: first install dependencies from dependencies.txt and then run download.py
make sure you have atleast 5gb of storage left because this will install a lot of files
its reccomended to copy the built executable to where the downloads folder is at so /

install these dependencies:
```python
pip install ffmpeg yt_dlp time
```
download Visual Studio 2022 or Visual Studio 2025 from trusted sources, install it with ``Desktop development with C++`` or Modify -> and add that
later, search for "Developer Command Prompt for VS 2022" or 2025 whichever version you just downloaded
then, cd (change directory) to the source code, run these commands:
```
mkdir build && cd build
cmake ..
cmake --build . --config Release
```
the application will be at ``build/Releases/bag.exe`` or ``Releases/bag.exe``
you can use ``copy build/Release/bag.exe .\`` inside the root directory of the program source code to copy it so you can easily just type bag into the terminal and it will automatically do it
now, you can run ``python download.py`` and wait at least 1 hour to 30 minutes, after thats done just run ``.\bag.exe`` or ``bag`` in the terminal
