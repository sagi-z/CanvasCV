Windows installation {#installwindows}
====================

## Windows build and install
### Build dependencies
Make sure these are installed first and that their executables are **in your path**:
* **cmake** - should already be installed if you built OpenCV by yourself (configure *OPENCV\_DIR* as required for build using OpenCV with cmake).
* **Visual Studio** - a C++ compiler - I'm using *Visual Studio 2015*.
* **git** (optional) - you can also download the sources from GitHub.
* **NSIS** (optional) - if you want to build a package installer.

### Building

* This should be done from a **"Developer Command Prompt for VS2015"** (Open it from the *Start* menu):
```
REM check dependencies are in path/exist
REM ====================================
cd c:\users\sagiz
where git
where cmake
echo %OPENCV_DIR%
where msbuild
dir "c:\Program Files (x86)\NSIS\NSIS.exe"

REM get the sources
REM ===============
cd c:\users\sagiz
git clone https://github.com/sagi-z/CanvasCV.git
cd CanvasCV
git checkout tags/1.0.0
mkdir build
cd build

REM configure the build
REM ===================
cmake -G "Visual Studio 14 2015 Win64" -DBUILD_EXAMPLES=ON ..

REM build the sources
REM =================
msbuild ALL_BUILD.vcxproj /p:Configuration=Release

REM build the installer
REM ===================
msbuild PACKAGE.vcxproj

REM run the installer
REM =================
.\canvascv-1.0.0-win64.exe

```