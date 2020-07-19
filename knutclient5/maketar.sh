#!/bin/sh

# use ./maketar.sh knitclient_x.x.x'

echo $1

STARTPOINT="/home/dan"


mkdir ${STARTPOINT}/$1



mkdir ${STARTPOINT}/${1}/build

cp -a ./src ${STARTPOINT}/${1}
cp -a ./doc ${STARTPOINT}/${1}
cp -a ./po ${STARTPOINT}/${1}
cp -a ./icons ${STARTPOINT}/${1}
cp -a ./pics ${STARTPOINT}/${1}

#rm -r -f ${STARTPOINT}/${1}/src/.svn
#rm -r -f ${STARTPOINT}/${1}/doc/.svn
#rm -r -f ${STARTPOINT}/${1}/doc/en/.svn
#rm -r -f ${STARTPOINT}/${1}/doc/cs/.svn
#rm -r -f ${STARTPOINT}/${1}/po/.svn
#rm -r -f ${STARTPOINT}/${1}/icons/.svn
#rm -r -f ${STARTPOINT}/${1}/pics/.svn


cp  ./AUTHORS ${STARTPOINT}/${1}
cp  ./COPYING ${STARTPOINT}/${1}
cp  ./INSTALL ${STARTPOINT}/${1}
cp  ./README ${STARTPOINT}/${1}
cp  ./TODO ${STARTPOINT}/${1}
cp  ./CMakeLists.txt ${STARTPOINT}/${1}
cp  ./ChangeLog ${STARTPOINT}/${1}
cp  ./Messages.sh ${STARTPOINT}/${1}
cp  ./cmake_release ${STARTPOINT}/${1}
cp  ./knutclient.lsm ${STARTPOINT}/${1}
cp  ./knutclient.notifyrc ${STARTPOINT}/${1}
cp  ./knutclient.desktop ${STARTPOINT}/${1}


OLDDIR=`pwd`
cd ${STARTPOINT}


echo "Generuji tar.gz soubor"
tar cvf ${1}.tar $1
gzip ${1}.tar

cd $OLDDIR
