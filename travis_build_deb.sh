mkdir build_deb

(
  cd build_deb

  cmake ..
  make install
)

sed -i "s/arch/$(dpkg --print-architecture)/; s/size/$(du -s deb | awk '{print $1}')/" deb/DEBIAN/control

fakeroot dpkg-deb --build deb
