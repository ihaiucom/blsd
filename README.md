## blsd

[Blizzard shader](https://wowdev.wiki/BLS) decoder

Support and tested only on WoD/Legion and BfA beta [GLSL shaders](https://wow.tools/files/#search=%2Fgl%25%2F%2Ctype%3Abls&page=1&sort=0&desc=asc)  
In BfA+ (include Classic) OpenGL support seems dropped.  
To get code from Vanilla/BC/WotLK shaders try use [this tool](https://en.wikipedia.org/wiki/Strings_(Unix)) on files

#### Usage

```
blsd shader.bls
```

### Build

```
mkdir build & cd build
cmake ..
cmake --build .
```

### Dependencies
zlib

### License
MIT
