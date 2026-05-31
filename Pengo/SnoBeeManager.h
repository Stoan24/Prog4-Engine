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
        void Clear();

        void RegisterSnoBee(GameObject* snoBee);

        void RegisterEgg(GameObject* eggBlock);

        void CreateSnoBee(int col, int row);

        void Notify(const Event& e) override;

        bool IsLevelComplete() const;

        void HatchNextEgg();

        const std::vector<GameObject*>& GetEggs() const { return m_Eggs; }

    private:
        friend class Singleton<SnoBeeManager>;
        SnoBeeManager() = default;

        void CleanupDead();

        GridComponent* m_pGrid{ nullptr };
        Scene* m_pScene{ nullptr };

        std::vector<GameObject*> m_ActiveSnoBees;
        std::vector<GameObject*> m_Eggs;
    };
}