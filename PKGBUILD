pkgname=image-analyze
pkgver=1.1.0
pkgrel=1
pkgdesc="Lightweight image viewer that supports measuring distance in pixels"
arch=('x86_64')
url="https://github.com/ArthurGuihaire/image-analyze"
licence='MIT'
depends=('sfml')
makedepends=('cmake' 'make' 'gcc')
source=("$url/archive/refs/tags/v$pkgver.tar.gz")
build() {
  cd "$srcdir/$pkgname"
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build --parallel
}
package() {
  cd "$srcdir/$pkgname-$pkgver/build"
  make DESTDIR="$pkgdir/" install
}