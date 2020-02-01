mkdir build_deb -p

(
  cd build_deb

  cmake -DCMAKE_BUILD_TYPE=Release -DDEB_PREFIX=deb ..
  make install
)

sed -i "s/arch/$(dpkg --print-architecture)/; s/size/$(du -s deb | awk '{print $1}')/" deb/DEBIAN/control

fakeroot dpkg-deb --build deb

mv deb.deb desnapper-btrfs-"$(dpkg --print-architecture)".deb
