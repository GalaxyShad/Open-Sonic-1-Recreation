#ifndef OS1R_TITLEMENUENTITY_H
#define OS1R_TITLEMENUENTITY_H

#include "EntityV3.h"

#include "TitleScreen.h"
#include "core/game_enviroment/GameEnvironment.h"

#include <format>
#include <functional>
#include <utility>

struct MenuElement {
    virtual void onLeft() {};
    virtual void onRight() {};
    virtual void onSelect() {};
    virtual void onHover() {};
    virtual void onUnhover() {}; // FIXME i dont know how to call this :D

    virtual const std::string &displayName() = 0;

    virtual ~MenuElement() = default;
};

class MenuButtonElement : public MenuElement {
  public:
    explicit MenuButtonElement(const std::string &text,
                               std::function<void()> onSelectFunction)
        : text_(text), onSelectFunction_(std::move(onSelectFunction)) {
        hoveredText_ = std::format("* {}", text_);
    }

    void onSelect() override { onSelectFunction_(); }
    void onHover() override { isHovered_ = true; }
    void onUnhover() override { isHovered_ = false; }

    const std::string &displayName() override {
        return isHovered_ ? hoveredText_ : text_;
    }

  private:
    std::function<void()> onSelectFunction_;
    std::string text_;
    std::string hoveredText_;
    bool isHovered_ = false;
};

class MenuSliderElement : public MenuElement {
  public:
    struct Props {
        std::function<void(int)> onDecrement;
        std::function<void(int)> onIncrement;
        std::function<void(int)> onSelect;
        int min;
        int max;
        int step;
    };

    explicit MenuSliderElement(const string &originalText, Props props)
        : props_(std::move(props)), originalText_(originalText) {
        if (props_.min == 0)
            props_.min = 0;

        if (props_.max == 0)
            props_.max = 100;

        if (props_.step == 0)
            props_.step = 1;
    }

    inline void onLeft() override {
        value_ -= props_.step;
        if (value_ < props_.min)
            value_ = props_.min;

        if (props_.onDecrement)
            props_.onDecrement(value_);
    }

    inline void onRight() override {
        value_ += props_.step;
        if (value_ > props_.max)
            value_ = props_.max;

        if (props_.onIncrement)
            props_.onIncrement(value_);
    }

    inline void onSelect() override { props_.onSelect(value_); }

    inline void onHover() override { isHovered_ = true; }

    inline void onUnhover() override { isHovered_ = false; }

    const std::string &displayName() override {
        text_ = std::format("{} {}    {} {}", isHovered_ ? "<" : " ",
                            originalText_, value_, isHovered_ ? ">" : " ");
        return text_;
    }

  private:
    Props props_;
    int value_;
    bool isHovered_;
    std::string originalText_;
    std::string text_;
};

class MenuEntity : public entity_v3::Entity {
  public:
    MenuEntity(std::vector<std::unique_ptr<MenuElement>> elementList)
        : elementList_(std::move(elementList)) {}

    entity_v3::EntityID type() override { return 0; }

    void onUpdate(const entity_v3::UpdateContext &ctx) override {
        int prevCursor = cursor_;

        if (ctx.input.isKeyPressed(InputKey::UP))
            cursor_--;
        if (ctx.input.isKeyPressed(InputKey::DOWN))
            cursor_++;

        if (ctx.input.isKeyPressed(InputKey::LEFT))
            elementList_[cursor_]->onLeft();
        if (ctx.input.isKeyPressed(InputKey::RIGHT))
            elementList_[cursor_]->onRight();
        if (ctx.input.isKeyPressed(InputKey::SELECT))
            elementList_[cursor_]->onSelect();

        if (cursor_ < 0)
            cursor_ = (int)elementList_.size() - 1;

        if (cursor_ >= elementList_.size())
            cursor_ = 0;

        if (prevCursor != cursor_) {
            elementList_[prevCursor]->onUnhover();
            elementList_[cursor_]->onHover();
            // TODO play sound
        }
    }

    void onDraw(const entity_v3::DrawContext &ctx) override {
        auto &scr = ctx.deprecatedScreen;
        auto scrCenterPos =
            v2f(scr.getSize().width / 2, scr.getSize().height / 2);
        auto pos = scrCenterPos;
        pos.y += 72;
        pos.x -= 32;

        for (auto &el : elementList_) {
            auto &str = el->displayName();

            ctx.deprecatedScreen.drawText(0, str.c_str(), pos);

            pos.y += 14.f;
        }
    }

    bool hasTag(entity_v3::TagID tag) override { return entity_v3::NO_TAGS; }
    const std::vector<entity_v3::Component> &components() override {
        return entity_v3::NO_COMPONENTS;
    }

  private:
    std::vector<std::unique_ptr<MenuElement>> elementList_;
    int cursor_ = 0;
};

class TitlePressStartEntity : public entity_v3::Entity {
  public:
    entity_v3::EntityID type() override { return 2; }

    void onUpdate(const entity_v3::UpdateContext &ctx) override { tick_++; }

    void onDraw(const entity_v3::DrawContext &ctx) override {
        if (tick_ % 40 < 20)
            return;

        ctx.deprecatedScreen.drawTextureRect(
            254, irect(56, 158, 144, 8),
            v2f((float)ctx.deprecatedScreen.getSize().width / 2,
                (float)ctx.deprecatedScreen.getSize().height / 2));
    }

    bool hasTag(entity_v3::TagID tag) override { return entity_v3::NO_TAGS; }
    const std::vector<entity_v3::Component> &components() override {
        return entity_v3::NO_COMPONENTS;
    }

  private:
    int tick_ = 0;
};

class TitleMenuEntity : public entity_v3::Entity {
  public:
    explicit TitleMenuEntity(TitleScreen &titleScreen, GameEnvironment& env)
        : titleScreen_(titleScreen), env_(env), mainMenu_(mainMenuConstruct()) {}

    void onUpdate(const entity_v3::UpdateContext &ctx) override {
        pressStartEntity_.onUpdate(ctx);
        mainMenu_.onUpdate(ctx);
    }

    void onDraw(const entity_v3::DrawContext &ctx) override {
        titleScreen_.draw();
        pressStartEntity_.onDraw(ctx);
        mainMenu_.onDraw(ctx);
    }

    entity_v3::EntityID type() override { return 1; }

    bool hasTag(entity_v3::TagID tag) override { return entity_v3::NO_TAGS; }
    const std::vector<entity_v3::Component> &components() override {
        return entity_v3::NO_COMPONENTS;
    }

  private:
    TitleScreen &titleScreen_;
    GameEnvironment& env_;

    TitlePressStartEntity pressStartEntity_;
    MenuEntity mainMenu_;

  private:
    std::vector<std::unique_ptr<MenuElement>> mainMenuConstruct() {
        std::vector<std::unique_ptr<MenuElement>> res;

        res.emplace_back(new MenuButtonElement("play", []() {}));
        res.emplace_back(new MenuButtonElement("options", []() {}));
        res.emplace_back(new MenuSliderElement("volume", {}));
        res.emplace_back(new MenuButtonElement("exit", [this]() {
            env_.exitGame();
        }));

        return res;
    }
};

#endif // OS1R_TITLEMENUENTITY_H
