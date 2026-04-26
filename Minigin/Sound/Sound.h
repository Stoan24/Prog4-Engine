#pragma once
#include <string>


namespace dae
{
    using sound_id = unsigned int;

    class SoundSystem
    {

    public:

        virtual ~SoundSystem() = default;

        virtual void Play(sound_id id, float volume) = 0;
        virtual void StopAll() = 0;
        virtual void SetVolume(float volume) = 0;

        virtual sound_id LoadSound(const sound_id id, const std::string& filepath) = 0;

    };

    class NullSoundSystem final : public SoundSystem
    {

    public:

        void Play(sound_id, float) override {}
        void StopAll() override {}
        void SetVolume(float) override {}

        sound_id LoadSound(const sound_id, const std::string&) override { return 0; }

    };
}