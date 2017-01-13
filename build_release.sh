#!/bin/sh
PREFIX=$DATAHOME/local

echo "Compiling Suprem4.GS and strread"
make clean && make COPT="-O2" FOPT="-O2 -ffixed-form" depend install
cd ./strread && make && cd ..

echo ""
echo "Installing to $(PREFIX)"
cp -rvf ./bin/suprem $PREFIX/bin/suprem
cp -rvf ./strread/dist/strread $PREFIX/bin/strread
cp -rvf ./data $PREFIX/data
cp -rvf ./help $PREFIX/help
cp -rvf ./doc  $PREFIX/doc

echo "Installation finished!!"
