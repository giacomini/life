// Copyright 2019 Istituto Nazionale di Fisica Nucleare
// Licensed under the EUPL

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

enum class State : char { Dead, Alive };

class Board
{
  using Data = std::vector<State>;
  int N;
  Data m_board;

 public:
  Board(int n) : N(n), m_board(N * N)
  {
  }

  int size() const
  {
    return N;
  }
  State operator()(int i, int j) const noexcept
  {
    return (i >= 0 && i < N && j >= 0 && j < N) ? m_board[i * N + j]
                                                : State::Dead;
  }
  State& operator()(int i, int j) noexcept
  {
    assert(i >= 0 && i < N && j >= 0 && j < N);
    return m_board[i * N + j];
  }
};

int neigh_count(Board const& board, int const r, int const c)
{
  int result = -static_cast<int>(board(r, c));
  for (int i = r - 1; i != r + 2; ++i) {
    for (int j = c - 1; j != c + 2; ++j) {
      result += static_cast<int>(board(i, j));
    }
  }
  return result;
}

Board evolve(Board const& current)
{
  int const N = current.size();

  Board next(N);

  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) {
      int const c = neigh_count(current, i, j);
      if (c == 3) {
        next(i, j) = State::Alive;
      } else if (c == 2) {
        next(i, j) = current(i, j);
      }
    }
  }

  return next;
}

#include <SFML/Graphics.hpp>

constexpr int board_size = 64;
constexpr int cell_size = 10;
// don't show the borders of the board
constexpr int wsize_x = (board_size - 6) * cell_size;
constexpr int wsize_y = (board_size - 6) * cell_size;

class Display
{
  sf::RenderWindow window{
      sf::VideoMode(wsize_x, wsize_y), "Life", sf::Style::Close};

 public:
  void draw(Board const& board)
  {
    window.clear(sf::Color::White);

    sf::RectangleShape rect(sf::Vector2f(cell_size, cell_size));
    rect.setFillColor(sf::Color::Red);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::White);

    int const N = board.size();

    for (int i = 3; i != N - 3; ++i) {
      for (int j = 3; j != N - 3; ++j) {
        if (board(i, j) == State::Alive) {
          // note inversion of i and j
          rect.setPosition((j - 3) * cell_size, (i - 3) * cell_size);
          window.draw(rect);
        }
      }
    }

    window.display();
  }

  void wait_key_pressed()
  {
    sf::Event event;

    window.waitEvent(event);
    while (event.type != sf::Event::KeyPressed) {
      window.waitEvent(event);
    }
  }
};

#include <chrono>
#include <thread>

Display display;

int main()
{
  Board board(board_size);

  board(28, 4) = State::Alive;
  board(29, 5) = State::Alive;
  board(30, 3) = State::Alive;
  board(30, 4) = State::Alive;
  board(30, 5) = State::Alive;

  display.draw(board);
  display.wait_key_pressed();

  for (int i = 0; i != 150; ++i) {
    board = evolve(board);
    display.draw(board);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //    display.wait_key_pressed();
  }
}
