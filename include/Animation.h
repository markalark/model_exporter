/*
 * File:   Animation.h
 * Author: Markalark
 *
 */

#ifndef ANIMATION_H
#define	ANIMATION_H

#include <map>
#include "glm_headers.h"
#include "Skeleton.h"

//
class Animation {
public:
    Animation();
    void add_scale(float time, std::shared_ptr<Skeleton::Bone> bone, Vec3f scale) {
        pose_transforms[to_milliseconds(time)][bone].scale = scale;
    }
    void add_orientation(float time, std::shared_ptr<Skeleton::Bone> bone, glm::quat orientation) {
        pose_transforms[to_milliseconds(time)][bone].orientation = orientation;
    }
    void add_offset(float time, std::shared_ptr<Skeleton::Bone> bone, Vec3f offset) {
        pose_transforms[to_milliseconds(time)][bone].translation = offset;
    }
    Matrix4x4 get_pose_transform(float time, std::shared_ptr<Skeleton::Bone> bone) const {
        float key_time = get_nearest_key_time(to_milliseconds(time));
        if (pose_transforms.find(key_time) == pose_transforms.end()) {
            return bone->get_local_transform();
        } else if (pose_transforms.at(key_time).find(bone) == pose_transforms.at(key_time).end()) {
            return bone->get_local_transform();
        } else {
            const Transform &pose = pose_transforms.at(key_time).at(bone);
            return Matrix4x4::Translation(pose.translation) * Matrix4x4(pose.orientation) * Matrix4x4::Scale(pose.scale);
        }
    }
    std::vector<unsigned int> get_key_times() const {
        std::vector<unsigned int> key_times;
        for (const auto &key : pose_transforms) {
            key_times.push_back(key.first);
        }
        return key_times;
    }
    bool has_keys() const {
        return !pose_transforms.empty();
    }
private:
    float get_nearest_key_time(unsigned int time) const {
        unsigned int nearest_key_time = 0;
        for (const auto &key : pose_transforms) {
            if (key.first == time) {
                return key.first;
            } else if (key.first > nearest_key_time && key.first < time) {
                nearest_key_time = key.first;
            }
        }
        return nearest_key_time;
    }
    struct Transform {
        Vec3f scale;
        glm::quat orientation;
        Vec3f translation;
    };
    // round to milliseconds
    unsigned int to_milliseconds(float seconds) const {
        return (unsigned int) (seconds * 1000);
    }
    // the pose transforms (indexed like pose_transforms[time][bone])
    std::map<unsigned int, std::map<std::shared_ptr<Skeleton::Bone>, Transform> > pose_transforms;
};

#endif	/* ANIMATION_H */

