#pragma once

namespace Battle
{

class BattleProfile;

class Effect
{
public:
    /**
     * @brief Constructor
     *
     * @param user user profile
     * @param target target profile
     * @param duration duration
     */
    Effect(BattleProfile& user, BattleProfile& target, int duration);

    /**
     * @brief Destructor
     */
    virtual ~Effect() = default;

    /**
     * @brief Action performed by the effect every turn
     */
    void Tick();

    /**
     * @brief Refresh the duration to the original value
     */
    void Refresh();

    /**
     * @brief Get the remaining duration
     */
    inline virtual int GetRemainingDuration() { return m_RemainingDuration; }

protected:
    /**
     * @brief Apply the effect to the target
     */
    virtual void Apply() = 0;

    /**
     * @brief De-apply the effect from the target
     */
    virtual void Remove() = 0;

    virtual void TickAction() = 0;

    constexpr static const int InfiniteDuration = -1;

    const int OriginalDuration;

    BattleProfile& m_User;
    BattleProfile& m_Target;
    int m_RemainingDuration;
};

} /* namespace Battle */