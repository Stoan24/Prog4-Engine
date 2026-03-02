#include "ThrashComponent.h"
#include <algorithm>

void dae::ThrashComponent::RenderGUI()
{
    ImGui::Begin("Thrash The Cache");

    ImGui::Columns(2, "Exercises", true);

    //Ex1
    if (ImGui::CollapsingHeader("Exercise 1", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        ImGui::InputInt("# samples##1", &m_SamplesInt);
        if (ImGui::Button("Thrash the cache##1"))
        {
            ThrashInt(m_PlotInt, m_SamplesInt, m_BufferSize);
        }


        float max1 = m_PlotInt.empty() ? 1.0f : *std::max_element(m_PlotInt.begin(), m_PlotInt.end());
        RenderPlot("##Int", m_PlotInt, ImColor(255, 128, 0), max1);
    }

    ImGui::NextColumn();


    //Ex2
    if (ImGui::CollapsingHeader("Exercise 2", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        ImGui::InputInt("# samples##2", &m_SamplesGameObjects);


        if (ImGui::Button("Thrash GameObject3D"))
        {
            ThrashGameObject(m_PlotGO3D, m_SamplesGameObjects, m_BufferSize);
        }

        float maxGO = m_PlotGO3D.empty() ? 1.0f : *std::max_element(m_PlotGO3D.begin(), m_PlotGO3D.end());
        RenderPlot("##GO", m_PlotGO3D, ImColor(0, 255, 0), maxGO);



        if (ImGui::Button("Thrash GameObject3DAlt"))
        {
            ThrashAlt(m_PlotAlt, m_SamplesGameObjects, m_BufferSize);
        }

        float maxAlt = m_PlotAlt.empty() ? 1.0f : *std::max_element(m_PlotAlt.begin(), m_PlotAlt.end());
        RenderPlot("##Alt", m_PlotAlt, ImColor(0, 255, 255), maxAlt);

        //Combined
        if (!m_PlotGO3D.empty() && !m_PlotAlt.empty())
        {
            ImGui::Text("Combined:");
            float combinedMax = std::max(maxGO, maxAlt);
            RenderPlot("##CombBase", m_PlotGO3D, ImColor(0, 255, 0), combinedMax);
            RenderPlot("##CombOverlay", m_PlotAlt, ImColor(0, 255, 255), combinedMax, true);
        }
    }

    ImGui::Columns(1);
    ImGui::End();
}

void dae::ThrashComponent::RenderPlot(const char* label, const std::vector<float>& scores, ImVec4 color, float scaleMax, bool isCombined)
{
    if (scores.empty()) return;

    if (isCombined)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - m_PlotSize);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    }

    ImGui::PushStyleColor(ImGuiCol_PlotLines, color);

    ImGui::PlotLines(label, scores.data(), static_cast<int>(scores.size()), 0, nullptr, 0, scaleMax, ImVec2(0, m_PlotSize));

    ImGui::PopStyleColor();

    if (isCombined) ImGui::PopStyleColor();
}

void dae::ThrashComponent::ThrashInt(std::vector<float>& scores, int sampleAmount, int bufferSize)
{
    scores.clear();

    std::vector<int> array(bufferSize);

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        float sampleTime = 0.f;

        for (int sample = 0; sample < sampleAmount; sample++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < array.size(); i += stepsize)
            {
                array[i] *= 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sampleTime += elapsedTime;
        }

        float averageTime = sampleTime / sampleAmount;

        scores.push_back(averageTime);
    }
}

void dae::ThrashComponent::ThrashGameObject(std::vector<float>& scores, int sampleAmount, int bufferSize)
{
    scores.clear();

    std::vector<GameObject3D> array(bufferSize);

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        float sampleTime = 0.f;

        for (int sample = 0; sample < sampleAmount; sample++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < array.size(); i += stepsize)
            {
                array[i].ID *= 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sampleTime += elapsedTime;
        }

        float averageTime = sampleTime / sampleAmount;

        scores.push_back(averageTime);
    }
}

void dae::ThrashComponent::ThrashAlt(std::vector<float>& scores, int sampleAmount, int bufferSize)
{
    scores.clear();

    std::vector<GameObject3DAlt> array(bufferSize);

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        float sampleTime = 0.f;

        for (int sample = 0; sample < sampleAmount; sample++)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < array.size(); i += stepsize)
            {
                array[i].ID *= 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sampleTime += elapsedTime;
        }

        float averageTime = sampleTime / sampleAmount;

        scores.push_back(averageTime);
    }
}
