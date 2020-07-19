#include <stdexcept>

#include "hex.hpp"

namespace hex
{

    Menu::MenuList Menu::mainMenu_;
    Menu::MenuList Menu::optionsMenu_;
    Menu::MenuList Menu::newGameMenu_;

    void Menu::Initialize()
    {
        mainMenu_.AddItem("New game", 1);
        mainMenu_.AddItem("Options", 2);
        mainMenu_.AddItem("Exit", 3);

        newGameMenu_.AddOption("Grid size:", 1, {"7", "9", "11"});
        newGameMenu_.AddOption("Difficulty:", 2, {"Easy", "Medium", "Hard"});
        newGameMenu_.AddItem("Start", 4);
        newGameMenu_.AddItem("Back", 6);

        optionsMenu_.AddOption("Resolution:", 0, {"800x600", "1280x720", "1600x900", "1920x1080"});
        optionsMenu_.AddOption("Color theme:", 1, hex::Palette::GetThemeNames());
        optionsMenu_.AddItem("Apply", 4);
        optionsMenu_.AddItem("Back", 6);
    }

    void Menu::Update()
    {
        switch(Engine::GetGameState())
        {
            case Engine::GameState::MAIN_MENU: {
                SDL_Event clickEvent = Input::GetClick();
                if(clickEvent.type == SDL_MOUSEBUTTONDOWN)
                {
                    if(clickEvent.button.button == SDL_BUTTON_LEFT)
                    {
                        switch(mainMenu_.Click(clickEvent.motion.x, clickEvent.motion.y))
                        {
                            case 1:
                                Engine::SetGameState(Engine::GameState::NEW_GAME_MENU);
                                break;
                            case 2:
                                Engine::SetGameState(Engine::GameState::OPTIONS_MENU);
                                break;
                            case 3:
                                quit_ = true;
                            default:
                                break;
                        }
                    }
                }
                break;
            }
            case Engine::GameState::NEW_GAME_MENU: {
                SDL_Event clickEvent = Input::GetClick();
                if(clickEvent.type == SDL_MOUSEBUTTONDOWN)
                {
                    if(clickEvent.button.button == SDL_BUTTON_LEFT)
                    {
                        switch(newGameMenu_.Click(clickEvent.motion.x, clickEvent.motion.y))
                        {
                            case 4: {
                                std::string gridSize = newGameMenu_.GetSelection("Grid size:");
                                // grid = hex::Grid(std::stoi(gridSize));
                                hex::Score::Start();
                                Engine::SetGameState(Engine::GameState::GAME);
                                break;
                            }
                            case 6:
                                Engine::SetGameState(Engine::GameState::MAIN_MENU);
                                break;
                            default:
                                break;
                        }
                    }
                }
                break;
            }
            case Engine::GameState::OPTIONS_MENU: {
                SDL_Event clickEvent = Input::GetClick();
                if(clickEvent.type == SDL_MOUSEBUTTONDOWN)
                {
                    switch(optionsMenu_.Click(clickEvent.motion.x, clickEvent.motion.y))
                    {
                        case 1: {
                            std::string theme = optionsMenu_.GetSelection("Color theme:");
                            hex::Palette::ChangeTheme(theme);
                            break;
                        }
                        case 4: {
                            std::string resolution = optionsMenu_.GetSelection("Resolution:");
                            int x = resolution.find("x");
                            std::string resolutionX = resolution.substr(0, x);
                            std::string resolutionY = resolution.substr(x + 1, resolution.length() - x - 1);
                            hex::Engine::ChangeResolution(std::stoi(resolutionX), std::stoi(resolutionY));
                            break;
                        }
                        case 6:
                            Engine::SetGameState(Engine::GameState::MAIN_MENU);
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    void Menu::Draw()
    {
        switch(Engine::GetGameState())
        {
            case Engine::GameState::MAIN_MENU:
                mainMenu_.Draw(Input::cursorX, Input::cursorY);
                break;
            case Engine::GameState::NEW_GAME_MENU:
                newGameMenu_.Draw(Input::cursorX, Input::cursorY);
                break;
            case Engine::GameState::OPTIONS_MENU:
                optionsMenu_.Draw(Input::cursorX, Input::cursorY);
                break;
            default:
                break;
        }
    }

    void Menu::MenuList::AddItem(const std::string& title, uint8_t index)
    {
        int x = Engine::windowWidth_ / 2;
        int y = spacing_ * (index + 1);
        SDL_Rect bounds = FC_GetBounds(font_, x, y, FC_AlignEnum::FC_ALIGN_CENTER, {1, 1}, "%s", title.c_str());
        items_[title] = MenuItem{MenuItemType::Item, title, index, bounds, {}, 0};
    }
    void Menu::MenuList::AddOption(const std::string& title, uint8_t index, const std::vector<std::string>& valueNames)
    {
        int x = Engine::windowWidth_ / 5;
        int y = spacing_ * (index + 1);
        SDL_Rect bounds = FC_GetBounds(font_, x, y, FC_AlignEnum::FC_ALIGN_CENTER, {1, 1}, "%s", title.c_str());
        items_[title] = MenuItem{MenuItemType::Option, title, index, bounds, std::move(valueNames), 0};
    }

    bool Menu::MenuList::IsMouseInside(int16_t x, int16_t y, const SDL_Rect& box) const
    {
        return (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h);
    }

    int8_t Menu::MenuList::MouseOverItemIndex(int16_t cursorX, int16_t cursorY) const
    {
        for(const auto& item : items_)
        {
            if(IsMouseInside(cursorX, cursorY, item.second.box)) return item.second.index;
        }
        return -1;
    }

    std::string Menu::MenuList::MouseOverItemName(int16_t cursorX, int16_t cursorY) const
    {
        for(const auto& item : items_)
        {
            if(IsMouseInside(cursorX, cursorY, item.second.box)) return item.second.title;
        }
        return "";
    }

    void Menu::MenuList::NextOption(const std::string& itemName)
    {
        items_.at(itemName).selected++;
        if(items_.at(itemName).selected >= items_.at(itemName).options.size()) items_.at(itemName).selected = 0;
    }

    int8_t Menu::MenuList::Click(int16_t cursorX, int16_t cursorY)
    {
        std::string selected = MouseOverItemName(cursorX, cursorY);
        if(selected.length() > 0)
        {
            switch(items_.at(selected).type)
            {
                case MenuItemType::Item: {
                    return items_[selected].index;
                    break;
                }
                case MenuItemType::Option: {
                    NextOption(selected);
                    return items_[selected].index;
                    break;
                }
                default:
                    break;
            }
        }
        return -1;
    }

    std::string Menu::MenuList::GetSelection(const std::string& item) const
    {
        return items_.at(item).options.at(items_.at(item).selected);
    }

    void Menu::MenuList::Draw(int16_t cursorX, int16_t cursorY) const
    {

        Palette::Color f = Palette::GetColor(Palette::Element::Foreground);
        Palette::Color c = Palette::GetColor(Palette::Element::A);

        uint8_t hover = MouseOverItemIndex(cursorX, cursorY);
        for(const auto& item : items_)
        {
            SDL_Color color = {f.r, f.g, f.b, 255};
            if(item.second.index == hover) color = {c.r, c.g, c.b, 255};

            switch(item.second.type)
            {
                case MenuItemType::Item: {
                    FC_SetDefaultColor(font_, color);
                    FC_DrawScale(font_, gRenderer_, item.second.box.x, item.second.box.y, {1, 1}, "%s",
                                 item.second.title.c_str());

                    break;
                }
                case MenuItemType::Option: {
                    FC_SetDefaultColor(font_, {f.r, f.g, f.b, 255});
                    FC_DrawScale(font_, gRenderer_, item.second.box.x, item.second.box.y, {1, 1}, "%s",
                                 item.second.title.c_str());

                    int x = item.second.box.x + item.second.box.w + 20;
                    int y = item.second.box.y;
                    FC_SetDefaultColor(font_, color);
                    FC_DrawScale(font_, gRenderer_, x, y, {1, 1}, "%s",
                                 item.second.options.at(item.second.selected).c_str());

                    break;
                }
                default:
                    break;
            }
        }
    }

} // namespace hex
