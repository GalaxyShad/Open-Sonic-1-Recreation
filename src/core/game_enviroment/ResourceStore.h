#ifndef OS1R_RESOURCESTORE_H
#define OS1R_RESOURCESTORE_H

#include <vector>
#include <memory>
#include "IStorableResource.h"

struct ResourceID {
    unsigned long id;
};

class ResourceStore {
public:
    ResourceID load(std::unique_ptr<IStorableResource> resource) {
        resourceList_.push_back(std::move(resource));

        return ResourceID{.id = currentId_++};
    }

    void loadMapping(std::unique_ptr<IStorableResource> map) {
        map_ = std::move(map);
    }

    template <class T> const T &map() { return *((T *)map_.get()); }

    template <class T> T &get(const ResourceID &id) {
        auto &res = resourceList_.at(id.id);

        return *((T *)res.get());
    }

private:
    std::vector<std::unique_ptr<IStorableResource>> resourceList_;
    std::unique_ptr<IStorableResource> map_;
    unsigned long currentId_ = 0;
};

#endif // OS1R_RESOURCESTORE_H
