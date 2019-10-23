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

short Projectile::getX() {
  return x;
}

short Projectile::getY() {
  return y;
}
