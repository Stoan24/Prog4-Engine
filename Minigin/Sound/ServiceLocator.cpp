#include "ServiceLocator.h"

namespace dae
{
    std::unique_ptr<SoundSystem> ServiceLocator::soundSystemInstance{ std::make_unique<NullSoundSystem>() };
}