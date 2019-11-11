#include "Projectile.h"

Projectile::Projectile(short inx, short iny, double inangle) {
  x = inx;
  y = iny;
  angle = inangle;
  lastPopped = nullptr;
}

Projectile::~Projectile() {
  //??
}

float Projectile::getX() const {
  return x;
}

float Projectile::getY() const {
  return y;
}
