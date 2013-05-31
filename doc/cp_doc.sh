#!/bin/sh

EAGLE_DIR="~/Documents/eagle/proj/nerd3"
EAGLE_DIR=`eval "echo $EAGLE_DIR"`     # expand tilda

SCH="nerd300.sch"
if ! [ -e ./$SCH ] || [ $EAGLE_DIR/$SCH -nt ./$SCH ]; then
    cp -p $EAGLE_DIR/$SCH ./$SCH
    echo "copied: sch"
fi

PDF_SRC="nerd300.pdf"
PDF_DEST=`basename -s .pdf $PDF_SRC`_sch.pdf
if ! [ -e ./$PDF_DEST ] || [ $EAGLE_DIR/$PDF_SRC -nt ./$PDF_DEST ]; then
    cp -p $EAGLE_DIR/$PDF_SRC ./$PDF_DEST
    echo "copied: pdf"
fi


