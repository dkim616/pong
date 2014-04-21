#include "ball.h"

#include <algorithm>

#include "game.h"
#include "graphics.h"
#include "sprite.h"
#include "paddle.h"

namespace {
  const std::string kSpriteFilePath("BallPaddle");

  const int kBallLeft = 0;
  const int kBallTop = 0;
  const int kBallWidth = 32;
  const int kBallHeight = 32;

  const int kMinVelocity = 250;
}

Ball::Ball(Graphics& graphics, float x, float y)
    : alive_(true),
      x_(x),
      y_(y) {
  initializeSprite(graphics);
  setRandomVelocity();
}

bool Ball::update(int elapsed_time) {
  x_ += velocity_x_ * elapsed_time;
  if (x_ < -100 || x_ > Game::kScreenWidth + 100 - 32) {
    x_ -= velocity_x_ * elapsed_time;
    alive_ = false;
  }

  y_ += velocity_y_ * elapsed_time;
  if (y_ < 0 || y_ > Game::kScreenHeight - 32) {
    y_ -= velocity_y_ * elapsed_time;
    velocity_y_ = -velocity_y_;
  }

  return alive_;
}

void Ball::draw(Graphics& graphics) {
  sprite_->draw(graphics, (int)x_, (int)y_);
}

void Ball::bounce(Paddle& paddle) {
  if (velocity_x_ < 0) {
    x_ = paddle.collisionRectangle().left() + paddle.collisionRectangle().width();
  } else if (velocity_x_ > 0) {
    x_ = paddle.collisionRectangle().right() - paddle.collisionRectangle().width() - (float)kBallWidth;
  }
  velocity_x_ = -velocity_x_;
}

Rectangle Ball::collisionRectangle() const {
  return Rectangle(x_, y_, (float)kBallWidth, (float)kBallHeight);
}

void Ball::initializeSprite(Graphics& graphics) {
  sprite_ = boost::shared_ptr<Sprite>(new Sprite(
      graphics,
      kSpriteFilePath,
      kBallLeft,
      kBallTop,
      kBallWidth,
      kBallHeight));
}

void Ball::setRandomVelocity() {
  velocity_x_ = (std::max(rand() % 1000, kMinVelocity) - 500.0f) / 1000.0f;
  velocity_y_ = (std::max(rand() % 1000, kMinVelocity) - 500.0f) / 1000.0f;
}
