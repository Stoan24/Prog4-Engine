#include "SDLSoundSystem.h"
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
            if (!MIX_Init()) return;

            m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

            if (!m_mixer) return;

#ifndef __EMSCRIPTEN__
            m_thread = std::jthread([this](std::stop_token stop_token) { PlayQueue(stop_token); });
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

        void RegisterSound(const sound_id id, const std::string& filepath)
        {
            if (m_paths.find(id) != m_paths.end())
            {
                return;
            }

            m_paths[id] = filepath;
        }

        void Play(sound_id id, float volume)
        {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(m_mutex);
            m_playQueue.push({ id, volume });
            m_condition.notify_one();
#else
            PlaySingleTrack({ id, volume });
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

        void PlaySingleTrack(PlayRequest request)
        {
            MIX_Audio* audio{};

            if (m_loadedSounds.find(request.id) != m_loadedSounds.end())
            {
                auto path_it = m_paths.find(request.id);
                if (path_it == m_paths.end()) return;

                audio = MIX_LoadAudio(m_mixer, path_it->second.c_str(), true);

                m_loadedSounds[request.id] = audio;
            }

            if (audio == nullptr)
            {
                audio = m_loadedSounds[request.id];
            }

            MIX_PlayAudio(m_mixer, audio);
        }

#ifndef __EMSCRIPTEN__
        void PlayQueue(std::stop_token stop_token)
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

                    lock.unlock();
                    PlaySingleTrack(request);
                    lock.lock();
                }
            }
        }
#endif

        MIX_Mixer* m_mixer{ nullptr };

        std::unordered_map<sound_id, MIX_Audio*> m_loadedSounds;
        std::unordered_map<sound_id, std::string> m_paths;
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

    void SDLSoundSystem::RegisterSound(const sound_id id, const std::string& filepath)
    {
        Pimpl->RegisterSound(id, filepath);
    }

#pragma endregion Pimpl
}