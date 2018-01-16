```
cmake -H. -B_builds -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=_install
cmake --build _builds
cmake --build _builds --target test -- ARGS=--verbose
cmake --build _builds --target install
_builds/example
```
