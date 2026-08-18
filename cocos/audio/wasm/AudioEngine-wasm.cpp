/****************************************************************************
 Silent audio engine for the WebAssembly build.

 The desktop engine is built on fmod, which ships as a prebuilt binary with no wasm variant. The
 tests and the samples call the audio API all over, so it answers here as an engine with no output:
 every playback reports the invalid id and every query reports an unknown time.
 ****************************************************************************/

#include "platform/CCPlatformConfig.h"

#if defined(__EMSCRIPTEN__)

#include "audio/include/AudioEngine.h"

using namespace cocos2d;

const int AudioEngine::INVALID_AUDIO_ID = -1;
const float AudioEngine::TIME_UNKNOWN = -1.0f;

bool AudioEngine::lazyInit()
{
    return false;
}

void AudioEngine::end()
{
}

int AudioEngine::play2d(const std::string& filePath, bool loop, float volume, const AudioProfile* profile)
{
    return INVALID_AUDIO_ID;
}

void AudioEngine::setLoop(int audioID, bool loop)
{
}

void AudioEngine::setVolume(int audioID, float volume)
{
}

void AudioEngine::pause(int audioID)
{
}

void AudioEngine::pauseAll()
{
}

void AudioEngine::resume(int audioID)
{
}

void AudioEngine::resumeAll()
{
}

void AudioEngine::stop(int audioID)
{
}

void AudioEngine::stopAll()
{
}

bool AudioEngine::setCurrentTime(int audioID, float sec)
{
    return false;
}

float AudioEngine::getCurrentTime(int audioID)
{
    return TIME_UNKNOWN;
}

float AudioEngine::getDuration(int audioID)
{
    return TIME_UNKNOWN;
}

void AudioEngine::setFinishCallback(int audioID, const std::function<void(int, const std::string&)>& callback)
{
}

void AudioEngine::uncache(const std::string& filePath)
{
}

void AudioEngine::uncacheAll()
{
}

void AudioEngine::preload(const std::string& filePath, std::function<void(bool isSuccess)> callback)
{
    if (callback)
        callback(false);
}

#endif
