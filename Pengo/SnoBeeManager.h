#pragma once
#include "Singleton.h"
#include "Observers/Observer.h"
#include "Events/EventManager.h"
#include "GameObject.h"
#include "Components/GridComponent.h"
#include "Scene.h"
#include <vector>
#include <functional>

namespace dae
{
    class SnoBeeManager final : public Singleton<SnoBeeManager>, public Observer
    {
    public:
        void Initialize(GridComponent* grid);

        void RegisterSnoBee(GameObject* snoBee);

        void RegisterEgg(GameObject* eggBlock);

        void CreateSnoBee(int col, int row);

        void Notify(const Event& e) override;

        //int GetActiveSnoBeeCount() const { return static_cast<int>(m_ActiveSnoBees.size()); }
        //int GetEggCount() const { return static_cast<int>(m_Eggs.size()); }

        void HatchNextEgg();
    private:
        friend class Singleton<SnoBeeManager>;
        SnoBeeManager() = default;

        void OnSnoBeeKilled(GameObject* snoBee);
        void CleanupDead();

        GridComponent* m_pGrid{ nullptr };
        Scene* m_pScene{ nullptr };

        std::vector<GameObject*> m_ActiveSnoBees;
        std::vector<GameObject*> m_Eggs;
    };
}