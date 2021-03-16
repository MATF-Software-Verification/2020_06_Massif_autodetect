# 2020_06_Massif_autodetect
Korišćenje izlaznih fajlova valgrind-ovog alata Massif u pronalasku neispravnog/nepotrebnog korišćenja memorije

## Pre-build koraci
* Pobtrebno je na računaru imati instaliranu biblioteku [Boost](https://www.boost.org/) >= 1.50
kao i sistem [cmake](https://www.cmake.org) >= 3.9
## Build koraci

- ``mkdir build `` 
- ``cd build`` 
- ``cmake ..``
- ``cmake --build . `` ili ``make``

ili način na koji preferirate bild sistemom cmake.

## Upotreba
    Pokretanjem alata uz opciju --help dobijaju
    se sva neophodna uputstva za upotebu. 