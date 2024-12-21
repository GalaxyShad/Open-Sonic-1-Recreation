#ifndef OS1R_AUDIOLOADER_H
#define OS1R_AUDIOLOADER_H

#include <memory>
#include <string>
#include "DjResources.h"

namespace dj {
struct AudioLoader {
    virtual std::unique_ptr<Sound> loadSound(const std::string& soundFilePath) = 0;
    virtual std::unique_ptr<Music> loadMusic(const std::string& soundFilePath) = 0;

    virtual ~AudioLoader() = default;
};
} // namespace dj

#endif // OS1R_AUDIOLOADER_H
