#include "SDLSoundSystem.h"
#include "Events/EventManager.h"
#include "Events/Event.h"
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace dae
{
    class SDLSoundSystem::SDLSoundSystemImpl
    {
    public:

        SDLSoundSystemImpl()
        {
            if (!MIX_Init()) {
                std::cerr << "Failed to initialize mixer " << SDL_GetError() << std::endl;
            }

            m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

            if (!m_mixer)
            {
                std::cerr << "Failed to create mixer " << SDL_GetError() << '\n';
                return;
            }

#ifndef __EMSCRIPTEN__
            m_thread = std::jthread([this](std::stop_token stop_token) { UpdateQueue(stop_token); });
#else
            UpdateQueue();
#endif
        }

        ~SDLSoundSystemImpl()
        {
#ifndef __EMSCRIPTEN__
            
            //Stop thread safely
            m_thread.request_stop();
            m_condition.notify_one();

            if (m_thread.joinable())
            {
                m_thread.join();
            }
#endif

            m_loadedSounds.clear();
        }

        sound_id LoadSound(const sound_id id, const std::string& filepath)
        {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(m_mutex);
#endif
            


            if (m_loadedSounds.find(id) != m_loadedSounds.end())
            {
                return id;
            }

            MIX_Audio* audio = MIX_LoadAudio(m_mixer, filepath.c_str(), true);

            if (audio == nullptr)
            {
                std::cerr << "Failed to load sound " << filepath << " " << SDL_GetError() << '\n';
                return 0;
            }

            m_loadedSounds[id] = audio;
            return id;
        }

        void Play(sound_id id, float volume)
        {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(m_mutex);
#endif

            m_playQueue.push({ id, volume });

#ifndef __EMSCRIPTEN__
            m_condition.notify_one();
#endif
        }

        void StopAll()
        {
            MIX_StopAllTracks(m_mixer, 0);
        }

        void SetVolume(float volume)
        {
            m_volume = std::clamp(volume, 0.0f, 1.0f);
            MIX_SetMixerGain(m_mixer, m_volume);
        }

    private:
        struct PlayRequest
        {
            sound_id id;
            float volume;
        };

#ifndef __EMSCRIPTEN__
        void UpdateQueue(std::stop_token stop_token)
        {
            while (!stop_token.stop_requested())
            {

                std::unique_lock<std::mutex> lock(m_mutex);

                m_condition.wait(lock, [this, &stop_token] {
                    return !m_playQueue.empty() || stop_token.stop_requested();
                    });

                if (stop_token.stop_requested())
                {
                    break;
                }
                

                while (!m_playQueue.empty())
                {
                    PlayRequest request = m_playQueue.front();
                    m_playQueue.pop();

                    auto it = m_loadedSounds.find(request.id);
                    if (it != m_loadedSounds.end())
                    {
                        MIX_Audio* audio = it->second;

                        MIX_PlayAudio(m_mixer, audio);
                    }
                }
            }
        }
#endif

        void UpdateQueue()
        {
            while (!m_playQueue.empty())
            {
                PlayRequest request = m_playQueue.front();
                m_playQueue.pop();

                auto it = m_loadedSounds.find(request.id);
                if (it != m_loadedSounds.end())
                {
                    MIX_Audio* audio = it->second;

                    MIX_PlayAudio(m_mixer, audio);
                }
            }
        }

        MIX_Mixer* m_mixer;

        std::unordered_map<sound_id, MIX_Audio*> m_loadedSounds;
        std::queue<PlayRequest> m_playQueue;

        float m_volume{ 1.0f };


#ifndef __EMSCRIPTEN__
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::jthread m_thread;
#endif
    };



#pragma region Pimpl

    SDLSoundSystem::SDLSoundSystem()
        : Pimpl(std::make_unique<SDLSoundSystemImpl>())
    {
    }

    SDLSoundSystem::~SDLSoundSystem() = default;

    void SDLSoundSystem::Play(sound_id id, float volume)
    {
        Pimpl->Play(id, volume);
    }

    void SDLSoundSystem::StopAll()
    {
        Pimpl->StopAll();
    }

    void SDLSoundSystem::SetVolume(float volume)
    {
        Pimpl->SetVolume(volume);
    }

    sound_id SDLSoundSystem::LoadSound(const sound_id id, const std::string& filepath)
    {
        return Pimpl->LoadSound(id, filepath);
    }

#pragma endregion Pimpl
}