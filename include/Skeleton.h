#ifndef SKELETON_H
#define	SKELETON_H
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Matrix4x4.h"

// skeleton with bones that influence mesh vertices
class Skeleton {
public:
    class Bone;
    void attach_bone(std::shared_ptr<Bone> child, std::shared_ptr<Bone> parent);
    std::shared_ptr<Bone> add_bone(std::string name);
    std::shared_ptr<Bone> get_bone(std::string name) const;
    std::shared_ptr<Bone> get_bone(int index) const;
    std::vector<std::shared_ptr<Bone> > get_bones() const;
    bool has_bones() const {
        return !bones.empty();
    }
private:
    std::shared_ptr<Bone> root;
    std::map<std::string, std::shared_ptr<Bone> > bones_by_name;
    std::vector<std::shared_ptr<Bone> > bones;
};

// bone
struct Skeleton::Bone {
public:
    Bone(std::string name, int id) : name(name), id(id) { }
    int get_id() {
        return id;
    }
    std::string get_name() {
        return name;
    }
    void set_parent(std::shared_ptr<Bone> parent) {
        this->parent = parent;
    }
    bool has_parent() const {
        return parent != nullptr;
    }
    std::shared_ptr<Bone> get_parent() const {
        return parent;
    }
    void add_child(std::shared_ptr<Bone> child) {
        children.push_back(child);
    }
    const std::vector<std::shared_ptr<Bone> > &get_children() const {
        return children;
    }
    void set_inverse_bind_pose(Matrix4x4 transform) {
        inverse_bind_pose = transform;
    }
    const Matrix4x4 &get_inverse_bind_pose() const {
        return inverse_bind_pose;
    }
    void set_local_transform(Matrix4x4 transform) {
        local_transform = transform;
    }
    const Matrix4x4 &get_local_transform() const {
        return local_transform;
    }
    void set_global_transform(Matrix4x4 transform) {
        global_transform = transform;
    }
    const Matrix4x4 &get_global_transform() const {
        return global_transform;
    }
private:
    // id
    std::string name;
    int id;

    // bone parent / children
    std::vector<std::shared_ptr<Bone> > children;
    std::shared_ptr<Bone> parent;

    // transforms
    Matrix4x4 inverse_bind_pose;
    Matrix4x4 local_transform;
    Matrix4x4 global_transform;
};

#endif

