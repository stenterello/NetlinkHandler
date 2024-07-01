#!/bin/bash

if [ $(docker images | grep netlink_handler_docs | wc -l) -eq 0 ]; then
  docker build -t netlink_handler_docs:1 doxygen/
fi

if [ ! -d doxygen/docs ]; then
  mkdir doxygen/docs
fi

if [ ! -f doxygen/Doxyfile ]; then
  docker run --user $UID:`id -g` --rm -v "`pwd`:/app" netlink_handler_docs:1 doxygen -u -g doxygen/Doxyfile
  sed -i '/^PROJECT_NAME/c\PROJECT_NAME\t\t= "NetlinkHandler"' doxygen/Doxyfile
  sed -i '/^OUTPUT_DIRECTORY/c\OUTPUT_DIRECTORY\t\t= "./doxygen/docs"' doxygen/Doxyfile
  sed -i '/^EXTRACT_ALL/c\EXTRACT_ALL\t\t= YES' doxygen/Doxyfile
  sed -i '/^EXTRACT_PRIVATE/c\EXTRACT_PRIVATE\t\t= YES' doxygen/Doxyfile
  sed -i '/^EXTRACT_PRIV_VIRTUAL/c\EXTRACT_PRIV_VIRTUAL\t\t= YES' doxygen/Doxyfile
  sed -i '/^USE_MDFILE_AS_MAINPAGE/c\USE_MDFILE_AS_MAINPAGE\t= README.md' doxygen/Doxyfile
#  sed -i '/^INPUT /c\INPUT\t\t\t= "../."' doxygen/Doxyfile
  sed -i '/^RECURSIVE/c\RECURSIVE\t\t\t= YES' doxygen/Doxyfile
fi

docker run --user $UID:`id -g` --rm -v "`pwd`/:/app" netlink_handler_docs:1 doxygen doxygen/Doxyfile