#pragma once

#include <SFML/Window.hpp>

class IInputMgr {
    public:
        virtual bool isKeyLeft() = 0;
        virtual bool isKeyRight() = 0;
        virtual bool isKeyUp() = 0;
        virtual bool isKeyDown() = 0;
        virtual bool isKeyAction() = 0;
        virtual bool isKeySpindash() = 0;
        virtual bool isKeyDebug() = 0;
};

class Keyboard : public IInputMgr {
    public:
        bool isKeyLeft()     { return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);  }
        bool isKeyRight()    { return sf::Keyboard::isKeyPressed(sf::Keyboard::Right); }
        bool isKeyUp()       { return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);    }
        bool isKeyDown()     { return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);  }
        bool isKeyAction()   { return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);     }
        bool isKeySpindash() { return sf::Keyboard::isKeyPressed(sf::Keyboard::X);     }
        bool isKeyDebug()    { return sf::Keyboard::isKeyPressed(sf::Keyboard::C);     }
};