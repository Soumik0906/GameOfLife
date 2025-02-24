#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>

// Custom hash function for std::pair
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

using Grid = std::unordered_map<std::pair<int, int>, bool, pair_hash>;

constexpr int CELL_SIZE { 15 };

int countLiveNeighbors(Grid& grid, int x, int y) {
    int count{};
    for (int dx { -1 }; dx <= 1; ++dx) {
        for (int dy { -1 }; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx { x + dx }, ny { y + dy };
            count += grid[{nx, ny}];
        }
    }
    return count;
}

void updateGrid(Grid& grid) {
    Grid newGrid;
    std::unordered_map<std::pair<int, int>, int, pair_hash> neighborCount;

    for (const auto& [cell, alive] : grid) {
        if (!alive) continue;

        for (int dx { -1 }; dx <= 1; ++dx) {
            for (int dy { -1 }; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;

                int nx { cell.first + dx }, ny { cell.second + dy };
                ++neighborCount[{nx, ny}];
            }
        }
    }

    for (const auto& [cell, count] : neighborCount) {
        if (grid[cell]) {
            newGrid[cell] = (count == 2 || count == 3);
        } else {
            newGrid[cell] = (count == 3);
        }
    }

    grid = std::move(newGrid);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Game of Life");
    window.setFramerateLimit(60);

    Grid grid;
    for (int x { 0 }; x <= 50; ++x) {
        for (int y { 0 }; y <= 50; ++y) {
            grid[{x, y}] = (rand() % 4 == 0);
        }
    }

    sf::View view(sf::FloatRect({0, 0}, {1920, 1080}));
    bool panning = false;
    sf::Vector2i oldMousePosition;

    while (window.isOpen()) {
        window.handleEvents(
            [&](const sf::Event::Closed&) {
                window.close();
            },
            [&](const sf::Event::MouseButtonPressed& event) {
                if (event.button == sf::Mouse::Button::Middle) {
                    panning = true;
                    oldMousePosition = sf::Mouse::getPosition(window);
                }
            },
            [&](const sf::Event::MouseButtonReleased& event) {
                if (event.button == sf::Mouse::Button::Middle) {
                    panning = false;
                }
            },
            [&](const sf::Event::MouseMoved& event) {
                if (panning) {
                    sf::Vector2i newMousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f delta = window.mapPixelToCoords(oldMousePosition) - window.mapPixelToCoords(newMousePosition);
                    view.move(delta);
                    oldMousePosition = newMousePosition;
                }
            },
            [&](const sf::Event::MouseWheelScrolled& event) {
                if (event.delta > 0) {
                    view.zoom(0.9f);
                } else {
                    view.zoom(1.1f);
                }
            }
        );

        updateGrid(grid);

        window.clear();
        window.setView(view);
        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f viewSize = view.getSize();
        int left = static_cast<int>(viewCenter.x - viewSize.x / 2) / CELL_SIZE - 1;
        int right = static_cast<int>(viewCenter.x + viewSize.x / 2) / CELL_SIZE + 1;
        int top = static_cast<int>(viewCenter.y - viewSize.y / 2) / CELL_SIZE - 1;
        int bottom = static_cast<int>(viewCenter.y + viewSize.y / 2) / CELL_SIZE + 1;

        for (int x { left }; x <= right; ++x) {
            for (int y { top }; y <= bottom; ++y) {
                sf::RectangleShape cell({ CELL_SIZE - 1, CELL_SIZE - 1 });
                cell.setPosition({ x * CELL_SIZE * 1.f, y * CELL_SIZE * 1.f});
                cell.setFillColor(grid[{x, y}] ? sf::Color::White : sf::Color::Black);
                window.draw(cell);
            }
        }

        window.display();
        sf::sleep(sf::milliseconds(50));
    }

    return 0;
}