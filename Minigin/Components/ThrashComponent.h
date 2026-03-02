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

    class TrashComponent final : public GameComponent
    {
    public:
        TrashComponent(GameObject* pGameObject)
            : GameComponent(pGameObject) 
        {
        }

        virtual ~TrashComponent() = default;

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

        void ThrashInt(std::vector<float>& scores, int sampleAmount, int bufferSize);
        void ThrashGameObject(std::vector<float>& scores, int sampleAmount, int bufferSize);
        void ThrashAlt(std::vector<float>& scores, int sampleAmount, int bufferSize);

    };
}