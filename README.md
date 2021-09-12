# 2020_06_Massif_autodetect
Korišćenje izlaznih fajlova valgrind-ovog alata Massif u pronalasku neispravnog/nepotrebnog korišćenja memorije

## Pre-build koraci
* Pobtrebno je na računaru imati instaliranu biblioteku [Boost](https://www.boost.org/) >= 1.50
kao i sistem [cmake](https://www.cmake.org) >= 3.9
## Build koraci
Najbolji način je kreirati poseban direktorijum u na nivou korenog dirketorijuma repozitorijuma.
Nakon što se pozicionirate u novokreirani direktorijum pokrenuti 
``cmake ..``  
nakon toga komandom ``cmake --build .`` biće kreiran izvršivi fajl pod nazivom `fixif`
### Uzorci sa testiranje  
U direktorijumu *samples* moguće je naći nekoliko jednostavnih .c programa koji možete prevesti u izvršive fajlove (preporučeno sa debug simbolima)
   
## Upotreba
    Program Options:  
    -h [ --help ]            Display help menu  
    -v [ --version ]         Display program version
    -m [ --massif ] arg      Path to massif file
    -e [ --executable ] arg  Path to executable file
    
    
## Dokumentacija 
   [Link do tehničke dokumentacije](http://alas.matf.bg.ac.rs/~mi16077/doxy_docs/html/)
   
   
## Autori
   [Ana Jakovljević](https://github.com/ana-jakovljevic)  
   [Velimir Bićanin](https://github.com/velimir97)  
   [Nikola Janković](https://github.com/gianthead97)  
