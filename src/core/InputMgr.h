#pragma once

#include <SFML/Window.hpp>

enum class InputKey : int {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ACTION,
    SPINDASH,
    SELECT,
    DEBUG_MENU,
    LENGTH_,
};

struct IInputMgr {
    virtual bool isKeyLeft() = 0;
    virtual bool isKeyRight() = 0;
    virtual bool isKeyUp() = 0;
    virtual bool isKeyDown() = 0;
    virtual bool isKeyAction() = 0;
    virtual bool isKeySpindash() = 0;
    virtual bool isKeyDebug() = 0;

    virtual bool isKey(InputKey k) = 0;
    virtual bool isKeyPressed(InputKey k) = 0;
    virtual bool isKeyReleased(InputKey k) = 0;

    virtual ~IInputMgr() = default;
};

class SfmlInput : public IInputMgr {
  public:
    bool isKeyLeft() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    }
    bool isKeyRight() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    }
    bool isKeyUp() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    }
    bool isKeyDown() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    }
    bool isKeyAction() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    }
    bool isKeySpindash() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    }
    bool isKeyDebug() override {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::C);
    }

    bool isKey(InputKey k) override {
        switch (k) {

        case InputKey::LEFT:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        case InputKey::RIGHT:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        case InputKey::UP:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        case InputKey::DOWN:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        case InputKey::ACTION:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
        case InputKey::SPINDASH:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::X);
        case InputKey::SELECT:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
        case InputKey::DEBUG_MENU:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        }
    }

    bool isKeyPressed(InputKey k) override {
        return isKeyboardPressed_[(int)k];
    }

    bool isKeyReleased(InputKey k) override {
        return isKeyboardReleased_[(int)k];
    }

    void reset() {
        for (int i = 0; i < (int)InputKey::LENGTH_; i++) {
            isKeyboardPressed_[i] = false;
            isKeyboardReleased_[i] = false;
        }
    }

    void setPressed(InputKey k) {
        isKeyboardPressed_[(int)k] = true;
    }

    void setReleased(InputKey k) {
        isKeyboardReleased_[(int)k] = true;
    }

  private:
    bool isKeyboardPressed_[(int)InputKey::LENGTH_] = {};
    bool isKeyboardReleased_[(int)InputKey::LENGTH_] = {};
};