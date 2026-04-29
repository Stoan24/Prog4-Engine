#pragma once

#include "Sound.h"
#include <memory>
#include <string>

namespace dae
{
    struct PlayRequest
    {
        sound_id id;
        float volume;
    };

    class SDLSoundSystem final : public SoundSystem
    {
        class SDLSoundSystemImpl;
        std::unique_ptr<SDLSoundSystemImpl> Pimpl;

    public:
        SDLSoundSystem();
        ~SDLSoundSystem() override;

        SDLSoundSystem(const SDLSoundSystem&) = delete;
        SDLSoundSystem(SDLSoundSystem&&) = delete;
        SDLSoundSystem& operator=(const SDLSoundSystem&) = delete;
        SDLSoundSystem& operator=(SDLSoundSystem&&) = delete;

        void Play(sound_id id, float volume) override;
        void StopAll() override;
        void SetVolume(float volume) override;

        void RegisterSound(const sound_id id, const std::string& filepath) override;
    };
}