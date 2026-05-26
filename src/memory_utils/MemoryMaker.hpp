#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "../memory_utils/Memory.hpp"

// ─────────────────────────────────────────────
//  MemoryMaker  –  constructs and manages an
//  interactive Memory grid.
//
//  Each cell is rendered as two triangles
//  (SFML 3 removed the Quads primitive).
//
//  Vertex layout per cell (6 verts, 2 tris):
//
//    0──1
//    │ ╱│
//    │╱ │
//    5  2
//    │╲ │    tri0: 0,1,2
//    │ ╲│    tri1: 3,4,5  (3==0, 4==2, 5=bottom-left)
//    4──3 ← (3 is conceptually bottom-right shared corner)
//
//  Indices in memory:  0 1 2 | 3 4 5
//                      TL TR BR | TL BR BL
// ─────────────────────────────────────────────
class MemoryMaker {
public:
    // ── Construction ─────────────────────────
    MemoryMaker(Rect border = Rect{},
        int  cols = 10,
        int  rows = 10)
        : m_cols(cols)
        , m_rows(rows)
    {
        m_memory.set_border(border);
        m_border = border;

        build_cells();
        build_vertex_array();
    }

    // ── Public interface ──────────────────────

    void draw(sf::RenderWindow& window) const {
        window.draw(m_memory.vertex_array);
    }

    void draw_with_transform(sf::RenderWindow& window,
        const sf::Transform& transform) const {
        window.draw(m_memory.vertex_array, transform);
    }

    void draw_border(sf::RenderWindow& window) const {
        m_border.draw(window);
    }

    // Set every cell back to the inactive (-1) state.
    void reset() {
        for (auto& col : m_memory.cells)
            for (Cell& cell : col) {
                cell.state = -1.f;
                cell.sync_color(m_memory.vertex_array);
            }
    }

    // Flip the sign of every cell's state (invert the pattern).
    void invert() {
        for (auto& col : m_memory.cells)
            for (Cell& cell : col) {
                cell.state *= -1.f;
                cell.sync_color(m_memory.vertex_array);
            }
    }

    // Process a mouse event.
    // Returns false if the mouse is outside the grid.
    // large_brush: paint a 5-cell radius rather than a single cell.
    bool handle_click(sf::Vector2i mouse_pos,
        bool         is_pressed,
        bool         draw_mode,
        bool         large_brush)
    {
        if (!m_memory.border.checkRectCollision(mouse_pos))
            return false;

        if (!is_pressed)
            return true; // inside grid but button not held

        // Map pixel → grid index
        const int col = static_cast<int>(
            (mouse_pos.x - m_memory.border.x) / m_cell_w);
        const int row = static_cast<int>(
            (mouse_pos.y - m_memory.border.y) / m_cell_h);

        if (large_brush)
            paint_radius(col, row, draw_mode, /*radius=*/5);
        else
            paint_cell(col, row, draw_mode);

        return true;
    }

    Memory get_memory() const { return m_memory.copy(); }

private:
    // ── State ─────────────────────────────────
    Memory m_memory{};
    Rect   m_border{};
    float  m_cell_w{};
    float  m_cell_h{};
    int    m_cols{};
    int    m_rows{};

    // ── Grid construction ─────────────────────

    void build_cells() {
        m_cell_w = m_memory.border.w / static_cast<float>(m_cols);
        m_cell_h = m_memory.border.h / static_cast<float>(m_rows);

        std::vector<std::vector<Cell>> cells(m_cols,
            std::vector<Cell>(m_rows));
        for (int c = 0; c < m_cols; ++c) {
            for (int r = 0; r < m_rows; ++r) {
                const float x = m_memory.border.x + c * m_cell_w;
                const float y = m_memory.border.y + r * m_cell_h;
                cells[c][r].rect = { x, y, m_cell_w, m_cell_h };
            }
        }

        m_memory.set_cells(std::move(cells));
    }

    // Build the VertexArray (must be called after build_cells).
    //
    // SFML 3 has no Quads primitive, so each cell is two triangles:
    //   tri0: top-left (TL), top-right (TR), bottom-right (BR)
    //   tri1: top-left (TL), bottom-right (BR), bottom-left (BL)
    void build_vertex_array() {
        sf::VertexArray va(sf::PrimitiveType::Triangles);
        int next_index = 0;

        for (auto& col : m_memory.cells) {
            for (Cell& cell : col) {
                const float x = cell.rect.x;
                const float y = cell.rect.y;
                const float x2 = x + cell.rect.w;
                const float y2 = y + cell.rect.h;

                // Triangle 0 — upper-right half
                va.append({ sf::Vector2f{ x,  y  } });   // TL  index+0
                va.append({ sf::Vector2f{ x2, y  } });   // TR  index+1
                va.append({ sf::Vector2f{ x2, y2 } });   // BR  index+2

                // Triangle 1 — lower-left half
                va.append({ sf::Vector2f{ x,  y  } });   // TL  index+3
                va.append({ sf::Vector2f{ x2, y2 } });   // BR  index+4
                va.append({ sf::Vector2f{ x,  y2 } });   // BL  index+5

                cell.vertex_indices = {
                    next_index + 0, next_index + 1, next_index + 2,
                    next_index + 3, next_index + 4, next_index + 5
                };
                cell.sync_color(va);

                next_index += 6;
            }
        }

        m_memory.set_vertex_array(std::move(va));
    }

    // ── Painting helpers ──────────────────────

    bool is_valid(int col, int row) const {
        return col >= 0 && row >= 0
            && col < static_cast<int>(m_memory.cells.size())
            && row < static_cast<int>(m_memory.cells[0].size());
    }

    void paint_cell(int col, int row, bool draw_mode) {
        if (is_valid(col, row))
            m_memory.cells[col][row].apply_stroke(m_memory.vertex_array,
                draw_mode);
    }

    // Paint every cell within a Chebyshev (square) radius.
    void paint_radius(int center_col, int center_row,
        bool draw_mode, int radius)
    {
        for (int c = center_col - radius; c <= center_col + radius; ++c)
            for (int r = center_row - radius; r <= center_row + radius; ++r)
                paint_cell(c, r, draw_mode);
    }
};