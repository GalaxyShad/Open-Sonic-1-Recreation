#pragma once

class GameLoopTicker {
  public:
    static GameLoopTicker& instance() {
        static GameLoopTicker inst;
        return inst;
    }

    void reset() { m_tick = 0; }
    void tick() { m_tick++; }
    int getTick() { return m_tick; }
    float getTickFloat() { return (float)m_tick; }

  private:
    GameLoopTicker() = default;
    GameLoopTicker(const GameLoopTicker &) = default;
    GameLoopTicker(GameLoopTicker &&) = default;
    GameLoopTicker &operator=(const GameLoopTicker &) = default;
    GameLoopTicker &operator=(GameLoopTicker &&) = default;
    
    GameLoopTicker* m_instance = nullptr;
    int m_tick = 0;
};