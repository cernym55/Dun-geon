#pragma once

#include <string>

namespace Battle
{

class BattleProfile;

class Effect
{
public:
    /**
     * @brief Constructor
     *
     * @param name effect name
     * @param user user profile
     * @param target target profile
     * @param duration duration
     */
    Effect(const std::string& name, const BattleProfile& user, BattleProfile& target, int duration);

    /**
     * @brief Destructor
     */
    virtual ~Effect() = default;

    /**
     * @brief Apply the effect to the target
     */
    virtual void Apply() = 0;

    /**
     * @brief De-apply the effect from the target
     */
    virtual void Remove() = 0;

    /**
     * @brief Perform the tick action and decrease duration
     */
    void Tick();

    /**
     * @brief Refresh the duration to the original value
     */
    void Refresh();

    /**
     * @brief Get the name
     *
     * @return const std::string& name
     */
    inline const std::string& GetName() const { return m_Name; }

    /**
     * @brief Get the remaining duration
     */
    inline virtual int GetRemainingDuration() { return m_RemainingDuration; }

protected:
    constexpr static const int InfiniteDuration = -1;

    const int OriginalDuration;

    std::string m_Name;
    const BattleProfile& m_User;
    BattleProfile& m_Target;
    int m_RemainingDuration;

    /**
     * @brief Action performed every turn the effect is active
     */
    virtual void TickAction() = 0;
};

} /* namespace Battle */