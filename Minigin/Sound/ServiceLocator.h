#pragma once

#include "Sound.h"
#include <memory>

namespace dae
{
    class ServiceLocator final
    {
        static std::unique_ptr<SoundSystem> soundSystemInstance;

    public:

        static SoundSystem& GetSoundSystem() { return *soundSystemInstance; }

        static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
        {
            soundSystemInstance = std::move(ss);
        }


        ServiceLocator() = delete;
        ServiceLocator(const ServiceLocator&) = delete;
        ServiceLocator(ServiceLocator&&) = delete;
        ServiceLocator& operator=(const ServiceLocator&) = delete;
        ServiceLocator& operator=(ServiceLocator&&) = delete;

    };
}