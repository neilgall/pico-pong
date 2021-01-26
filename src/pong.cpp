#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <string>

#include "pico_display.hpp"

using namespace pimoroni;

const uint32_t BAT_WIDTH = 4;
const uint32_t BAT_HEIGHT = 40;
const uint32_t BALL_RADIUS = 5;

uint16_t buffer[PicoDisplay::WIDTH * PicoDisplay::HEIGHT];
PicoDisplay pico_display(buffer);

struct Player {
  uint32_t index;
  uint32_t score;
  uint32_t y;

  Player(int index)
    : index(index)
    , y(PicoDisplay::HEIGHT / 2)
    , score(0)
  {}

  void up() {
    if (y > BAT_HEIGHT / 2)
      y--;
  }

  void down() {
    if (y < PicoDisplay::HEIGHT - BAT_HEIGHT / 2 - 1)
      y++;
  }

  void draw() {
    uint32_t x = (index == 1) ? 0 : PicoDisplay::WIDTH - 1 - BAT_WIDTH;
    pico_display.rectangle(Rect(x, y - BAT_HEIGHT / 2, BAT_WIDTH, BAT_HEIGHT));

    x = PicoDisplay::WIDTH / 2 + (index == 1 ? -30 : 30);
    pico_display.text(std::to_string(score), Point(x, 0), false, 2);
  }
};

struct Ball {
  Point start;
  Point pos;
  int32_t xd, yd;

  Ball(Point pos)
    : start(pos)
    , pos(pos)
    , xd(1)
    , yd(1)
  {}

  void move(Player &player1, Player &player2) {
    pos.x += xd;
    pos.y += yd;

    if (pos.y < BALL_RADIUS / 2 || pos.y > PicoDisplay::HEIGHT - BALL_RADIUS / 2)
      yd = -yd;

    if (pos.x < BAT_WIDTH + BALL_RADIUS / 2) {
      if (abs((int32_t)pos.y - (int32_t)player1.y) <= BAT_HEIGHT)
        xd = -xd;
      else {
        player2.score++;
        pos = start;
      }
    }

    if (pos.x > PicoDisplay::WIDTH - 1 - BAT_WIDTH - BALL_RADIUS / 2) {
      if (abs((int32_t)pos.y - (int32_t)player2.y) <= BAT_HEIGHT)
        xd = -xd;
      else {
        player1.score++;
        pos = start;
      }
    }
  }

  void draw() {
    pico_display.circle(pos, BALL_RADIUS);
  }
};

struct Game {
  Player player1;
  Player player2;
  Ball ball;

  Game()
    : player1(1)
    , player2(2)
    , ball(Point(PicoDisplay::WIDTH / 2, PicoDisplay::HEIGHT / 2))
  {}

  void draw();
  void update();
};

void Game::draw() {
  pico_display.set_pen(pico_display.create_pen(0, 0, 0));
  pico_display.clear();
  pico_display.set_pen(pico_display.create_pen(255, 255, 255));
  player1.draw();
  player2.draw();
  ball.draw();
}

void Game::update() {
  if (pico_display.is_pressed(pico_display.A)) {
    player1.up();
  } else if (pico_display.is_pressed(pico_display.B)) {
    player1.down();
  }

  if (pico_display.is_pressed(pico_display.X)) {
    player2.up();
  } else if (pico_display.is_pressed(pico_display.Y)) {
    player2.down();
  }

  ball.move(player1, player2);
}

int main() {
  Game game;

  pico_display.init();
  pico_display.set_backlight(100);

  while (true) {
    game.draw();
    game.update();

    pico_display.update();
  }

  return 0;
}
