#pragma once

#include <SFML/Audio.hpp>

#include <cstdint>
#include <map>
#include <cstring>

#include "AudioMappings.h"

class Audio {
    public:
        ~Audio() { free(); }

        float getVolumeSound() { return volumeSnd; }
        void setVolumeSound(float vol) 
            { (vol >= 0.f || vol <= 100.f) ? volumeSnd = vol : volumeSnd = 0; }

        float getVolumeMusic() { return volumeMus; }
        void setVolumeMusic(float vol) 
            { (vol >= 0.f || vol <= 100.f) ? volumeMus = vol : volumeMus = 0; }

        int loadSound(const char* file, uint8_t key) {
             if (soundsBuff.count(key))
                return 0;

            sf::SoundBuffer* sb = new sf::SoundBuffer();
            if (!sb->loadFromFile(file)) {
                printf("Audio loading error > %s\n", file);
                return 0;
            }

            soundsBuff[key] = sb;

            return 1;
        }

        int addMusic(uint8_t key, string file) {
             if (musics.count(key))
                return 0;

            musics[key] = file;

            return 1;
        }

        void playSound(uint8_t key) {
            if (!soundsBuff.count(key)) {
                return;
            }

            sf::Sound* snd = new sf::Sound(*soundsBuff[key]);
            snd->setVolume(volumeSnd);
            snd->play();

            sounds.push_back(snd);
        }

        void stopSound(uint8_t key) {
            // if (!soundsBuff.count(key))
            //     return;

            // sf::Sound s(*soundsBuff[key]);
            // snd.stop();
        }

        void playMusic(uint8_t key) {
            if (mus.getStatus() == sf::Music::Playing)
                mus.stop();

            mus.setVolume(volumeMus); 
            mus.openFromFile(musics[key]);
            mus.play();
        }

        void stopMusic() {
            mus.stop();
        }

        void update() {
            std::list<sf::Sound*>::iterator it;
            for (it = sounds.begin(); it != sounds.end();) {
                sf::Sound* snd = *it;
                if (snd->getStatus() == sf::Sound::Playing)
                    it++;
                else {
                    it = sounds.erase(it);
                    delete snd;
                }
            }
        }

        void free() {
            std::list<sf::Sound*>::iterator it;
            for (it = sounds.begin(); it != sounds.end();) {
                it = sounds.erase(it);
                delete &it;
            }

            std::map<uint8_t, sf::SoundBuffer*>::iterator itb;
            for (itb = soundsBuff.begin(); itb != soundsBuff.end();) {
                itb = soundsBuff.erase(itb);
                delete &itb;
            }
        }
    private:
        std::map<uint8_t, sf::SoundBuffer*> soundsBuff;
        std::map<uint8_t, std::string> musics;
        std::list<sf::Sound*> sounds;
        sf::Music mus;
        float volumeSnd = 50.f, volumeMus = 50.f;

};