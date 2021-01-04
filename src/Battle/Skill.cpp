#include "Skill.h"
#include "Misc/RNG.h"

namespace Battle
{

Skill::Skill(Category category,
             Target targetType,
             const std::string& name,
             const std::string& flavorText,
             const std::string& longDescription,
             int baseManaCost)
    : m_Category(category),
      m_TargetType(targetType),
      m_Name(name),
      m_FlavorText(flavorText),
      m_LongDescription(longDescription),
      m_BaseManaCost(baseManaCost)
{
}

int Skill::GetManaCost() const
{
    return m_BaseManaCost;
}

} /* namespace Battle */