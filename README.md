# Simple Gain 2.0 — cyan glow edition

A minimal-futuristic JUCE 8 VST3 for Windows / Ableton Live 10.1+.

- Gain processing and automation unchanged from the working v1.
- Custom JUCE-painted knob, cyan arc, level-dependent glow and dB readout.
- Double-click resets to 0 dB.
- The visual glow reflects the **gain setting**, not measured audio level.

## GitHub Actions build

This ZIP is **repository-root ready**. Upload the *contents* of this ZIP into the repository root, **not** inside another `SimpleGain` folder. The workflow must be at `.github/workflows/build-windows.yml`. Commit all changes and open Actions > Build Windows VST3. Download the successful run's artifact. The VST3 bundle is built but not tested here.

If replacing an older repository that already contains a nested `SimpleGain` directory, delete the obsolete nested copy or use the new root-level files. Do not mix old and new sources.

Builds JUCE 8.0.8 using the available Windows CMake generator. `juce_generate_juce_header` enables `JuceHeader.h`.
