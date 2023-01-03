#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>

template <typename T>
class ObjectManager {
public:
    // Constructor
    ObjectManager() {}

    // Destructor
    virtual ~ObjectManager() {}

    // Add an object to the manager
    void AddObject(const std::string& name, T object) {
        objects_[ToLower(name)] = object;
    }

    // Remove an object from the manager
    void RemoveObject(const std::string& name) {
        objects_.erase(ToLower(name));
    }

    T* FindObject(std::string name) {
        auto itr = objects_.find(ToLower(name));
        return itr != objects_.end() ? &itr->second : nullptr;
    }


    // Get a vector of all the values in the map
    std::vector<T*> GetValues() {
        std::vector<T*> values;
        values.reserve(objects_.size());

        for (auto& object : objects_) {
            values.emplace_back(&object.second);
        }
        return values;
    }

private:
    // Map of objects in the manager, with string names as keys
    std::map<std::string, T> objects_;

    // Convert a string to lowercase
    std::string ToLower(const std::string& str) {
        // Create a lowercase string by transforming each character in the input string
        std::string lowercaseStr;
        lowercaseStr.resize(str.size());
        std::transform(str.begin(), str.end(), lowercaseStr.begin(), ::tolower);

        return lowercaseStr;
    }
};
