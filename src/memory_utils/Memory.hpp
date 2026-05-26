#pragma once

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../neuron_utils/Neuron.hpp"

// ─────────────────────────────────────────────
//  Cell  –  one pixel/tile in a memory grid
// ─────────────────────────────────────────────
struct Cell {
    Rect  rect{};
    float state = -1.f;

    // Six vertex indices: two triangles that cover the cell's quad.
    //   tri0: [0,1,2]  (top-left → top-right → bottom-right)
    //   tri1: [3,4,5]  (top-left → bottom-right → bottom-left)
    std::array<int, 6> vertex_indices = { 0, 1, 2, 3, 4, 5 };

    // ── Colour ───────────────────────────────
    void sync_color(sf::VertexArray& va) const {
        const sf::Color color = (state > 0.f)
            ? sf::Color::White
            : sf::Color::Black;

        for (int i : vertex_indices)
            va[i].color = color;
    }

    // ── State helpers ────────────────────────

    // Flip only when the desired mode differs from the current state.
    // drawMode == true  → painting ON  (flip if currently off)
    // drawMode == false → painting OFF (flip if currently on)
    void flip_if_needed(bool draw_mode) {
        const bool currently_on = (state > 0.f);
        if (draw_mode != currently_on)
            state *= -1.f;
    }

    // Apply one paint stroke and refresh the vertex colour.
    void apply_stroke(sf::VertexArray& va, bool draw_mode) {
        flip_if_needed(draw_mode);
        sync_color(va);
    }
};


// ─────────────────────────────────────────────
//  Memory  –  grid of cells + shared VertexArray
// ─────────────────────────────────────────────
struct Memory {
    Rect  border{};
    std::vector<std::vector<Cell>> cells{};
    sf::VertexArray vertex_array{};

    // ── Initialisation ───────────────────────
    void set_cells(std::vector<std::vector<Cell>> new_cells) { cells = std::move(new_cells); }
    void set_vertex_array(sf::VertexArray        new_va) { vertex_array = std::move(new_va); }
    void set_border(Rect                         new_border) { border = new_border; }

    void draw(sf::RenderWindow& window) const {
        window.draw(vertex_array);
    }

    Memory copy() const {
        return { border, cells, vertex_array };
    }

    // Flatten cell states in column-major order (x inner, y outer).
    std::vector<float> to_flat_vector() const {
        std::vector<float> out;
        out.reserve(cells.size() * (cells.empty() ? 0 : cells[0].size()));

        for (std::size_t col = 0; col < cells.size(); ++col)
            for (std::size_t row = 0; row < cells[col].size(); ++row)
                out.push_back(cells[col][row].state);

        return out;
    }
};


// ─────────────────────────────────────────────
//  MemoryWrapper  –  a Memory with a world transform
//                    and a click-to-select highlight
// ─────────────────────────────────────────────
struct MemoryWrapper {
    Memory       memory{};
    Rect         screen_border{};   // border in screen space (no transform applied)
    sf::Transform transform{};
    unsigned int id = 0;
    bool         selected = false;

    MemoryWrapper(Memory         mem,
        unsigned int   id,
        sf::Transform  xform,
        Rect           border)
        : memory(std::move(mem))
        , screen_border(border)
        , transform(xform)
        , id(id)
    {}

    // Returns true and toggles selection when the click lands inside the border.
    bool handle_click(sf::Vector2i mouse_pos) {
        if (!screen_border.checkRectCollision(mouse_pos))
            return false;
        selected = !selected;
        return true;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(memory.vertex_array, transform);
        if (selected)
            screen_border.draw(window);
    }
};