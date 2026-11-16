# Simple Gain — JUCE 8 VST3

A basic gain VST3 plugin for Windows, intended for testing in Ableton Live 10.1+ (64-bit).

## Build on GitHub without installing developer tools

1. Create a **public, empty** GitHub repository called `SimpleGain` (private works too, subject to Actions limits).
2. Upload the **contents** of this folder to the root of the repository, including the hidden `.github/workflows/build-windows.yml` file. GitHub's web upload supports dragging folders/files from an extracted ZIP; check that `.github` was included.
3. Commit the files to the `main` branch.
4. Open the repository's **Actions** tab, choose **Build Windows VST3**, then **Run workflow** (or wait for the automatic push build).
5. Once the run succeeds, open its page and download the `SimpleGain-Windows-VST3` artifact ZIP.
6. Extract the artifact ZIP. Copy the entire `Simple Gain.vst3` **folder** to `C:\Program Files\Common Files\VST3\` (administrator permission may be required).
7. Open Ableton Live 10.1 or later, enable VST3 scanning in Preferences > Plug-Ins, rescan and load **Simple Gain** on an audio track.

**Important:** This is source code and an automated build configuration, not a precompiled or verified Windows plugin. If the workflow fails, copy the error log into ChatGPT for troubleshooting.

## Local build (optional)

Install CMake, Visual Studio with C++ workload and JUCE 8, then run:

```powershell
cmake -S . -B build -DJUCE_DIR="C:/SDK/JUCE"
cmake --build build --config Release --target SimpleGain_VST3
```

## Features

- Gain: -60 dB to +12 dB, default 0 dB
- 25 ms gain smoothing
- DAW automation and parameter state saving
- Mono/stereo support
- Dark, simple JUCE editor (the more elaborate futuristic concept art is **not** yet implemented)

## Notes

The GitHub workflow downloads JUCE 8.0.8 and builds on GitHub's Windows runner. The workflow has not yet been executed or verified in this environment. The plugin is intended for testing; do not rely on it for critical sessions until tested.
