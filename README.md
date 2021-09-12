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
   Dokumentaciju je moguće generisati pokretanjem neke od skripti `gen_and_show_docs.py` ili `gen_and_show_docs.sh`, takođe neophodno je da instalirate alat [doxygen](https://www.doxygen.nl/index.html)
   Takođe ako ne želite da instalirate dodatni alat dokumentacija je dostupna na [adresi](http://alas.matf.bg.ac.rs/~mi16077/doxy_docs/html/).
   
   
## Autori
   [Ana Jakovljević](https://github.com/ana-jakovljevic)  
   [Velimir Bićanin](https://github.com/velimir97)  
   [Nikola Janković](https://github.com/gianthead97)  
