#include "SfmlGameEnvironment.h"

void SfmlGameEnvironment::init() {
    renderWindow_.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                         WINDOW_TITLE);
    renderWindow_.setFramerateLimit(60);
}

void SfmlGameEnvironment::beginFrame() {
    input_.reset();
    artist_.renderClear();

    sf::Event event{};
    while (renderWindow_.pollEvent(event)) {
        handleEvent(event);
    }
}

void SfmlGameEnvironment::endFrame() {
    artist_.render();
    dj_.removeStoppedSounds();
}

void SfmlGameEnvironment::handleEvent(sf::Event event) {
    switch (event.type) {

    case sf::Event::Closed:
        renderWindow_.close();
        break;

    case sf::Event::KeyPressed:
        handleKeyPressed(event.key);
        break;
    }
}

void SfmlGameEnvironment::handleKeyPressed(sf::Event::KeyEvent keyEvent) {
    switch (keyEvent.code) {
    case sf::Keyboard::F: {
        currentWindowScale_++;

        if (currentWindowScale_ > (int)WindowScale::FULL_SCREEN) {
            currentWindowScale_ = (int)WindowScale::X1;
        }

        scaleWindow((WindowScale)currentWindowScale_);

        break;
    }
    }

    input_.press(keyEvent);
}

void SfmlGameEnvironment::scaleWindow(WindowScale scale) {
    if (scale == WindowScale::FULL_SCREEN) {
        renderWindow_.create(sf::VideoMode::getDesktopMode(), WINDOW_TITLE,
                             sf::Style::Fullscreen);
    } else {
        auto intScale = (int)scale;
        renderWindow_.create(
            sf::VideoMode(WINDOW_WIDTH * intScale, WINDOW_HEIGHT * intScale),
            WINDOW_TITLE);
    }

    renderWindow_.setFramerateLimit(60);
    renderWindow_.setView(
        sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)));
}