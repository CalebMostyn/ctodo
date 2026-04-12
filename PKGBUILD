pkgname=ctodo
pkgver=1.1.0
pkgrel=1
pkgdesc="HTTP Server todo App"
arch=('x86_64')
url="https://github.com/CalebMostyn/ctodo"
license=('MIT')
depends=('glibc')
makedepends=('gcc' 'make' 'python-pip')
provides=('ctodo')
conflicts=('ctodo')
sha256sums=('SKIP')

source=("git+https://github.com/CalebMostyn/ctodo.git")

prepare() {
    cd "$srcdir/ctodo"
    git submodule update --init --recursive
    python -m venv venv
    source venv/bin/activate
    pip install -r client/python/requirements.txt
}

build() {
  cd "$srcdir/ctodo"
  source venv/bin/activate
  make clean
  make all
}

package() {
  cd "$srcdir/ctodo"
  make DESTDIR="$pkgdir" PREFIX=/usr install
}

post_install() {
  echo "Run 'ctodo init' to create default config : ~/.config/ctodo/ctodo.conf"
}
