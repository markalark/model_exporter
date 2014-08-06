#include "Skeleton.h"
#include <memory>

//
void Skeleton::attach_bone(std::shared_ptr<Skeleton::Bone> child, std::shared_ptr<Skeleton::Bone> parent) {
    // connect parent and child
    child->set_parent(parent);
    if (parent != nullptr) {
        parent->add_child(child);
    }
}
std::shared_ptr<Skeleton::Bone> Skeleton::add_bone(std::string name) {
    if (bones_by_name.find(name) != bones_by_name.end()) {
        return bones_by_name[name];
    } else {
        std::shared_ptr<Skeleton::Bone> bone(new Skeleton::Bone(name, bones.size()));
        bones_by_name[name] = bone;
        bones.push_back(bone);
        return bone;
    }
}
std::shared_ptr<Skeleton::Bone> Skeleton::get_bone(std::string name) const {
    if (bones_by_name.find(name) != bones_by_name.end()) {
        return bones_by_name.at(name);
    } else {
        return std::shared_ptr<Skeleton::Bone>(nullptr);
    }
}
std::shared_ptr<Skeleton::Bone> Skeleton::get_bone(int index) const {
    return bones.at(index);
}
std::vector<std::shared_ptr<Skeleton::Bone> > Skeleton::get_bones() const {
    return bones;
}
