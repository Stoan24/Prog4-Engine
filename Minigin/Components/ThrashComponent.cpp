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
            ThrashTheCache<int>(m_PlotInt, m_SamplesInt, m_BufferSize, [](int& val) { val *= 2; });
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
            ThrashTheCache<GameObject3D>(m_PlotGO3D, m_SamplesGameObjects, m_BufferSize, [](GameObject3D& val) { val.ID *= 2; });
        }

        float maxGO = m_PlotGO3D.empty() ? 1.0f : *std::max_element(m_PlotGO3D.begin(), m_PlotGO3D.end());
        RenderPlot("##GO", m_PlotGO3D, ImColor(0, 255, 0), maxGO);



        if (ImGui::Button("Thrash GameObject3DAlt"))
        {
            ThrashTheCache<GameObject3DAlt>(m_PlotAlt, m_SamplesGameObjects, m_BufferSize, [](GameObject3DAlt& val) { val.ID *= 2; });
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
