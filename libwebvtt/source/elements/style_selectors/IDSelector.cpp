#include "elements/style_objects/IDSelector.h"

namespace WebVTT
{
    StyleSelector::SelectorType
    IDSelector::getSelectorType() const
    {
        return StyleSelector::SelectorType::ID;
    }

    bool
    IDSelector::shouldApply(const NodeObject &node, const Cue &cue) const
    {
        return false;
    }

} // namespace name
