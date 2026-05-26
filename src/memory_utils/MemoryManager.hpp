#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "../neuron_utils/Neuron.hpp"
#include "../memory_utils/MemoryMaker.hpp"

// ─────────────────────────────────────────────
//  MemoryManager  –  stores, displays, and
//  selects from a grid of MemoryWrappers.
//
//  Responsibilities:
//    - Accept Memory objects from MemoryMaker
//    - Lay them out in a scaled grid
//    - Handle click-to-select / delete
//    - Export selected memories as flat float vectors
// ─────────────────────────────────────────────

// Pre-computed layout data for one memory slot.
struct MemorySlot {
    int          id{};
    sf::Transform transform{};
    sf::Vector2f  position{};
};


class MemoryManager {
public:
    MemoryManager() = default;

    void init(Rect         border,
        sf::Vector2f scale,
        sf::Vector2f grid_amounts)
    {
        m_border = border;
        m_scale = scale;
        m_grid_amounts = grid_amounts;

        // A slightly expanded border used for click-region detection
        m_click_border = border;
        m_click_border.y = scale_vec(border.getPosition()).y;
        m_click_border.w += 200;

        build_slots(border.getPosition(), grid_amounts, /*spacing=*/{ 650.f, 650.f });
    }

    // ── Memory management ─────────────────────

    bool can_add() const {
        return m_memories.size() < m_slots.size();
    }

    void add_memory(Memory memory) {
        const int id = static_cast<int>(m_memories.size());
        m_memories.emplace_back(memory, id,
            m_slots[id].transform,
            make_border(id, memory));
    }

    // Erase all selected wrappers, then reassign IDs and layout data.
    void delete_selected() {
        std::erase_if(m_memories, [](const MemoryWrapper& w) {
            return w.selected;
            });

        for (int id = 0; id < static_cast<int>(m_memories.size()); ++id) {
            m_memories[id].id = id;
            m_memories[id].screen_border = make_border(id, m_memories[id].memory);
            m_memories[id].transform = m_slots[id].transform;
        }
    }

    // Returns the flat state vectors of every selected memory.
    std::vector<std::vector<float>> selected_memories() const {
        std::vector<std::vector<float>> out;
        for (const MemoryWrapper& w : m_memories)
            if (w.selected)
                out.push_back(w.memory.to_flat_vector());
        return out;
    }

    // ── Input ─────────────────────────────────

    // Returns true if the click was consumed by any wrapper.
    bool handle_click(sf::Vector2i mouse_pos) 
    {
        if (!m_click_border.checkRectCollision(mouse_pos))
        {
            return false;
        }

        for (MemoryWrapper& w : m_memories)
        {
            if (w.handle_click(mouse_pos))
            {
                return true;
            }
        }

        return false;
    }

    // ── Rendering ─────────────────────────────

    void draw(sf::RenderWindow& window) const {
        for (const MemoryWrapper& w : m_memories)
            w.draw(window);
    }

private:
    // ── State ─────────────────────────────────
    Rect         m_border{};
    Rect         m_click_border{};
    sf::Vector2f m_scale{};
    sf::Vector2f m_grid_amounts{};

    std::vector<MemorySlot>    m_slots{};
    std::vector<MemoryWrapper> m_memories{};

    // ── Layout helpers ────────────────────────

    // Scale a vector by m_scale (or invert if reverse == true).
    sf::Vector2f scale_vec(sf::Vector2f v, bool reverse = false) const {
        if (reverse)
            return { v.x / m_scale.x, v.y / m_scale.y };
        return { v.x * m_scale.x, v.y * m_scale.y };
    }

    sf::Transform make_transform(sf::Vector2f world_pos) const {
        sf::Transform t;
        t.translate(scale_vec(world_pos));
        t.scale(m_scale);
        return t;
    }

    // Build the screen-space highlight border for a given memory slot.
    Rect make_border(int id, const Memory& memory) const {
        sf::Vector2f scaled_pos = scale_vec(m_slots[id].position);
        sf::Vector2f scaled_size = scale_vec(memory.border.getSize());

        scaled_pos.x += scaled_size.x + 9.f;
        scaled_pos.y += 13.f;

        return Rect(scaled_pos.x, scaled_pos.y,
            scaled_size.x, scaled_size.y,
            { 0, 0, 0, 0 }, { 20, 100, 122 }, 1);
    }

    // Pre-compute a slot for every grid position.
    void build_slots(sf::Vector2f origin,
        sf::Vector2f amounts,
        sf::Vector2f spacing)
    {
        int id = 0;
        for (int col = 0; col < static_cast<int>(amounts.x); ++col) {
            for (int row = 0; row < static_cast<int>(amounts.y); ++row) {
                sf::Vector2f pos = {
                    origin.x + col * spacing.x,
                    origin.y + row * spacing.y
                };
                m_slots.push_back({ id, make_transform(pos), pos });
                ++id;
            }
        }
    }
};