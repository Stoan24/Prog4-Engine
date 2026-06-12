#include "ServiceLocator.h"

namespace dae
{
    std::unique_ptr<ISoundSystem> ServiceLocator::soundSystemInstance{ std::make_unique<NullSoundSystem>() };
}