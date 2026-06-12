#pragma once
#include <string>
#include <iostream>
#include <memory>

namespace dae
{
    using sound_id = unsigned int;

    class ISoundSystem
    {

    public:

        virtual ~ISoundSystem() = default;

        virtual void Play(sound_id id, float volume) = 0;
        virtual void StopAll() = 0;
        virtual void SetVolume(float volume) = 0;
        virtual float GetVolume() = 0;

        virtual void RegisterSound(const sound_id id, const std::string& filepath) = 0;

    };

    class NullSoundSystem final : public ISoundSystem
    {

    public:

        void Play(sound_id, float) override {}
        void StopAll() override {}
        void SetVolume(float) override {}
        float GetVolume() override { return  0.f; }

        void RegisterSound(const sound_id, const std::string&) override { return; }

    };

    class LoggingSoundSystem final : public ISoundSystem
    {
        std::unique_ptr<ISoundSystem> m_soundSystem;

    public:

        explicit LoggingSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
            : m_soundSystem(std::move(ss))
        {
        }

        void Play(sound_id id, float volume) override
        {
            std::cout << "Playing sound id: " << id << " at volume: " << volume << '\n';
            m_soundSystem->Play(id, volume);
        }

        void StopAll() override
        {
            std::cout << "Stopping all sounds\n";
            m_soundSystem->StopAll();
        }

        void SetVolume(float volume) override
        {
            std::cout << "Changing volume to: " << volume << '\n';
            m_soundSystem->SetVolume(volume);
        }

        float GetVolume() override
        {
            std::cout << "Current volume: " << m_soundSystem->GetVolume() << '\n';
            return m_soundSystem->GetVolume();
        }

        void RegisterSound(const sound_id id, const std::string& filepath) override
        {
            m_soundSystem->RegisterSound(id, filepath);
            std::cout << "Loaded sound: " << filepath << " with id: " << id << '\n';
        }
    };
}