#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

constexpr int ROWS { 50 };
constexpr int COLS { 50 };
constexpr int CELL_SIZE { 15 };

int countLiveNeighbors(std::vector<std::vector<bool>>& grid, int x, int y) {
    int count{};
    for (int dx { -1 }; dx <= 1; ++dx) {
        for (int dy { -1 }; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx { x + dx }, ny { y + dy };
            if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS) {
                count += grid[nx][ny];
            }
        }
    }
    return count;
}

void updateGrid(std::vector<std::vector<bool>>& grid) {
    auto newGrid = grid;

    for (int x { 0 }; x < ROWS; ++x) {
        for (int y { 0 }; y < COLS; ++y) {
            int neighbors { countLiveNeighbors(grid, x, y) };
            if (grid[x][y]) {
                newGrid[x][y] = (neighbors == 2 || neighbors == 3);
            } else {
                newGrid[x][y] = (neighbors == 3);
            }
        }
    }
    grid = newGrid;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ COLS * CELL_SIZE, ROWS * CELL_SIZE }), "Game of Life");

    std::vector<std::vector<bool>> grid(ROWS, std::vector(COLS, false));
    for (int x { 0 }; x < ROWS; ++x) {
        for (int y { 0 }; y < COLS; ++y) {
            grid[x][y] = (rand() % 4 == 0);
        }
    }

    while (window.isOpen()) {
        while (const std::optional event { window.pollEvent() }) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        updateGrid(grid);

        window.clear();
        for (int x { 0 }; x < ROWS; ++x) {
            for (int y { 0 }; y < COLS; ++y) {
                sf::RectangleShape cell({ CELL_SIZE - 1, CELL_SIZE - 1 });
                cell.setPosition({ y * CELL_SIZE * 1.f, x * CELL_SIZE * 1.f});
                cell.setFillColor(grid[x][y] ? sf::Color::White : sf::Color::Black);
                window.draw(cell);
            }
        }
        window.display();
        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
