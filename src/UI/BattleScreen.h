#pragma once

#include "Battle/Battle.h"
#include "ColorPairs.h"
#include "Components/LogWindow.h"
#include "Components/Nameplate.h"
#include "Screen.h"
#include "Subscreen.h"

namespace UI
{

class BattleScreen : public Subscreen
{
public:
    /**
     * @brief Width of the world panel
     */
    constexpr static const int ArenaPanelWidth = 50;

    /**
     * @brief Height of the world panel
     */
    constexpr static const int TopPanelHeight = 17;

    /**
     * @brief Width of the HUD panel
     */
    constexpr static const int LogPanelWidth = Screen::ScreenWidth - ArenaPanelWidth;

    /**
     * @brief Width of a nameplate in the arena panel
     */
    constexpr static const int ArenaNameplateWidth = 36;

    /**
     * @brief Height of the bottom panel
     */
    constexpr static const int BottomPanelHeight = Screen::ScreenHeight - TopPanelHeight;

    /**
     * @brief Constructor
     *
     * @param battle battle
     * @param screen screen
     */
    BattleScreen(Battle::Battle& battle, Screen& screen, InputHandler& inputHandler);

    /**
     * @brief Destructor
     */
    ~BattleScreen();

    /**
     * @brief Initialize the subscreen
     */
    virtual void Init() override;

    /**
     * @brief Clean up after the subscreen
     */
    virtual void Terminate() override;

    /**
     * @brief Select the player action via menu
     *
     * @return int action code
     */
    int SelectPlayerAction(const std::map<int, std::string>& actions);

    /**
     * @brief Select an option within the bottom panel with a hover action
     *
     * @param options options
     * @param hoverAction hover action
     * @return int option code
     */
    int SelectWithHoverAction(const std::map<int, std::string>& options,
                              std::function<void(std::map<int, std::string>::iterator)> hoverAction = {});

    /**
     * @brief Write a message in the upper left corner of the bottom panel
     *
     * @param message message
     */
    void PostMessage(const std::string& message);

    /**
     * @brief Project an attack against the enemy
     *
     * @param hitChancePercent chance to hit in percent
     */
    void ProjectAttack(int hitChancePercent);

    /**
     * @brief Clear the projection area
     */
    void ClearProjectionArea();

    /**
     * @brief Clear the skill thumbnail area
     */
    void ClearThumbnailArea();

    /**
     * @brief Animate a player attack
     *
     * @param damage damage number
     * @param hit whether the attack hit or missed
     */
    void AnimatePlayerAttack(int damage, bool hit);

    /**
     * @brief Animate an enemy attack
     *
     * @param damage damage number
     * @param hit whether the attack hit or missed
     */
    void AnimateEnemyAttack(int damage, bool hit);

    /**
     * @brief Display the message after the battle
     *
     * @param result battle result
     */
    void BattleEndMessage(Battle::Battle::Result result);

    /**
     * @brief Display the player stats in the stat window
     */
    void DisplayPlayerStats();

    /**
     * @brief Draw the skill thumbnail for the skill being hovered over
     */
    void DrawSkillHoverThumbnail();

    /**
     * @brief Add a message to the battle log
     * 
     * @param message message
     */
    void AppendToLog(const std::string& message);

private:
    Battle::Battle& m_Battle;
    WINDOW* m_ArenaPanelWindow;
    WINDOW* m_BottomPanelWindow;
    WINDOW* m_StatPanelWindow;
    Components::Nameplate m_PlayerNameplate;
    Components::Nameplate m_EnemyNameplate;
    Components::LogWindow m_LogWindow;

    /**
     * @brief Draw the layout of the panels
     */
    void DrawScreenLayout();

    /**
     * @brief Draw the arena panel
     */
    void DrawArenaPanel();

    /**
     * @brief Draw the log panel
     */
    void DrawLogPanel();

    /**
     * @brief Draw the bottom panel
     */
    void DrawBottomPanel();

    /**
     * @brief Draw the stat panel
     */
    void DrawStatPanel();

    /**
     * @brief Clear the bottom panel contents
     */
    void ClearBottomPanel();
};

} /* namespace UI */