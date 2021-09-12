#! /usr/bin/python
import os
import platform
INDEX_PATH = os.path.join(".", "doxy_docs", "html", "index.html")
legit = True
separator = ""
platf = platform.system()

if platf == "Windows":
    separator = ";"
elif platf == "Linux":
    separator = ":"


if not os.path.isfile(INDEX_PATH):
    for dirname in os.environ['PATH'].split(separator):
        doxy_exe = os.path.join(dirname, "doxygen")
        if platf == "Windows":
            doxy_exe += ".exe"
        print(doxy_exe)
        if os.path.isfile(doxy_exe):
            os.system(os.path.split(doxy_exe)[1] + " dconfig")
            break
    else:
        legit = False
        print("Please install doxygen or check whether path to doxygen executable is properly appended to PATH variable or not")


if legit:
    if platf == "Windows":
        os.system("start " + INDEX_PATH )
    elif platf == "Linux":
        os.system("xdg-open " + INDEX_PATH + " &")
else:
    print("Ooh something went wrong...")