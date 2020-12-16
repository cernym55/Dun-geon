#pragma once

#include "Entities/Character.h"
#include "Misc/Direction.h"
#include "Player/Controller.h"
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <string>
#include <vector>

namespace UI
{

class Screen;

class InputHandler
{
public:
    /**
     * @brief Types of ingame commands
     */
    enum class CommandType
    {
        None,
        Move,
        Get,
        Break,
        Battle,
        Talk,
        Trade,
        Turn
    };

    /**
     * @brief Types of UI commands
     */
    enum class UICommandType
    {
        Inventory,
        Skills,
        Map,
        Help,
        Quit
    };

    /**
     * @brief Constructor
     * 
     * @param screen screen
     * @param playerController player controller
     */
    InputHandler(Screen& screen, Player::Controller& playerController);

    /**
     * @brief Execute all commands in the queue
     */
    void ExecCommandQueue();

    /**
     * @brief Evaluate the input string and create commands
     * 
     * @param input input string
     */
    void Eval(const std::string& input);

    /**
     * @brief Check if the player has requested to quit the game
     * 
     * @return true if should quit
     */
    bool ShouldQuit() const;

    /**
     * @brief Set the flag signalling the game should quit
     */
    void SetQuit();

    /**
     * @brief Write the controls config file
     * @TODO: Move this functionality to a config class
     */
    void makeKeyConf() const;

    /**
     * @brief Load the controls from the controls config file
     * @TODO: Move this functionality to a config class
     */
    void loadKeyConf();

    /**
     * @brief Handle the next key input
     */
    void ProcessKeypress();

    /**
     * @brief Read a keypress and return the key or empty optional if not in list of valid inputs
     * @param validInput list of valid inputs
     * @param window window to read from
     */
    static std::optional<chtype> ReadKeypress(const std::vector<chtype>& validInput, WINDOW* window = stdscr);

private:
    /**
     * @brief A game command issued by the player
     */
    struct Command
    {
        CommandType Type;
        Direction Dir;
        int Repeats;

        /**
         * @brief Constructor
         */
        Command();

        /**
         * @brief Constructor
         * 
         * @param type type
         * @param dir direction
         * @param repeats number of repeats
         */
        Command(CommandType type, Direction dir, int repeats);
    };

    Screen& m_Screen;
    Player::Controller& m_PlayerController;
    std::string m_InputString;
    Command m_LastCommand;
    std::queue<InputHandler::Command> m_CommandQueue;
    std::map<std::string, Direction> m_DirDict;
    std::map<std::string, CommandType> m_CmdDict;
    std::map<std::string, UICommandType> m_UICmdDict;
    std::vector<std::string> m_AndKeywords;
    std::vector<std::string> m_LastKeywords;
    bool m_ShouldQuit;

    /**
     * @brief Execute the given command
     * 
     * @param command command
     * @return true if completed successfully
     */
    bool ExecCommand(Command& command);

    /**
     * @brief Execute a UI command of the given type
     * 
     * @param type UI command type
     */
    void ExecUICommand(UICommandType type);

    /**
     * @brief Evaluate input from the world view
     * 
     * @param words input split into words
     */
    void EvalWorld(std::vector<std::string>& words);

    /**
     * @brief Get long-form text input via visual prompt
     * 
     * @return std::string input
     */
    std::string CommandInput();
};

} /* namespace UI */
