## blsd

[Blizzard shader](https://wowdev.wiki/BLS) decoder

Support and tested only on WoD/Legion and BfA beta [GLSL shaders](https://wow.tools/files/#search=%2Fgl%25%2F%2Ctype%3Abls&page=1&sort=0&desc=asc)  
In BfA+ (include Classic) OpenGL support seems dropped.  
To get code from Vanilla/BC/WotLK shaders try use [this tool](https://en.wikipedia.org/wiki/Strings_(Unix)) on files  
[Legion shader dump](https://www.dropbox.com/s/th98m378rx37ang/7.X.X-8.0.1_glsl150.zip)  
[WotLK (3.3.5) shader dump](https://www.dropbox.com/s/nqrnxc6f8r0ie7w/wow_335_shaders.zip)  
[Vanilla (1.12) shader dump](https://www.dropbox.com/s/we46zkuzur8e28b/wow_1.12_shaders.zip)

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
