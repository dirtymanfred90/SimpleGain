cmake_minimum_required(VERSION 3.22)
project(SimpleGain VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pass -DJUCE_DIR="C:/path/to/JUCE" or set JUCE_DIR in CMake GUI.
if (NOT DEFINED JUCE_DIR)
    message(FATAL_ERROR "Set JUCE_DIR to the folder containing JUCE's CMakeLists.txt")
endif()
add_subdirectory("${JUCE_DIR}" "${CMAKE_BINARY_DIR}/JUCE")

juce_add_plugin(SimpleGain
    COMPANY_NAME "Independent Audio"
    BUNDLE_ID "com.independentaudio.simplegain"
    PLUGIN_MANUFACTURER_CODE IndA
    PLUGIN_CODE Sg01
    FORMATS VST3
    PRODUCT_NAME "Simple Gain"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(SimpleGain)

target_sources(SimpleGain PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

target_compile_definitions(SimpleGain PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(SimpleGain PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags)
