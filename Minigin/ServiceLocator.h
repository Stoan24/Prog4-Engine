#pragma once

#include "Sound/Sound.h"
#include <memory>

namespace dae
{
    class ServiceLocator final
    {
        static std::unique_ptr<ISoundSystem> soundSystemInstance;

    public:

        static ISoundSystem& GetSoundSystem() { return *soundSystemInstance; }

        static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
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