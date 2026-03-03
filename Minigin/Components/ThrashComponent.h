#pragma once
#include "GameComponent.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include "imgui.h"

struct Transform 
{
    float matrix[16] = { 
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class GameObject3D 
{
public:
    int ID{ 1 };
    Transform transform;
};

struct GameObject3DAlt 
{
    int ID{ 1 };
    Transform* transforms;
};

namespace dae
{

    class ThrashComponent final : public GameComponent
    {
    public:
        ThrashComponent(GameObject* pGameObject)
            : GameComponent(pGameObject) 
        {
        }

        virtual ~ThrashComponent() = default;

        void RenderGUI() override;
        void RenderPlot(const char* label, const std::vector<float>& scores, ImVec4 color, float scaleMax, bool isCombined = false);

    private:

        const int m_BufferSize = 1000000;
        int m_SamplesInt = 10;
        int m_SamplesGameObjects = 100;

        std::vector<float> m_PlotInt;
        std::vector<float> m_PlotGO3D;
        std::vector<float> m_PlotAlt;

        float m_PlotSize{ 80.f };

        //DRY
        template <typename T, typename Lambda>
        void ThrashTheCache(std::vector<float>& scores, int sampleAmount, int bufferSize, Lambda operation)
        {
            scores.clear();
            std::vector<T> array(bufferSize);

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
            {
                float totalTime = 0.f;

                for (int sample = 0; sample < sampleAmount; sample++)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (size_t i = 0; i < array.size(); i += stepsize)
                    {
                        operation(array[i]);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    totalTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                }

                scores.push_back(totalTime / sampleAmount);
            }
        }

    };
}