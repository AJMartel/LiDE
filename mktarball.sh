#!/bin/bash

mkdir -p Source
cd Source

curSrcVer=$(ls -l | tr -s ' ' | cut -f9 -d " " | grep 'v[0-9]' | tail -n1)
preSrcVer=$(ls -l | tr -s ' ' | cut -f9 -d " " | grep 'v[0-9]' | tail -n2 | head -n1)
mkdir -p $curSrcVer/Patch
diff $preSrcVer/LiDE.c $curSrcVer/LiDE.c >$curSrcVer/Patch/LiDE.c.patch
diff $preSrcVer/LiDE.h $curSrcVer/LiDE.h >$curSrcVer/Patch/LiDE.h.patch
diff $preSrcVer/nwio.c $curSrcVer/nwio.c >$curSrcVer/Patch/nwio.c.patch
diff $preSrcVer/Makefile $curSrcVer/Makefile >$curSrcVer/Patch/Makefile.patch
diff $preSrcVer/README $curSrcVer/README.md >$curSrcVer/Patch/README.patch

rm -r Current
mkdir -p Current/Patch
cp $curSrcVer/* Current/ 2>/dev/null
diff Original\ Sources/hermes/hermes.c $curSrcVer/LiDE.c >Current/Patch/LiDE.c.patch
diff Original\ Sources/hermes/hermes.h $curSrcVer/LiDE.h >Current/Patch/LiDE.h.patch
diff Original\ Sources/hermes/nwio.c $curSrcVer/nwio.c >Current/Patch/nwio.c.patch
diff Original\ Sources/hermes/Makefile $curSrcVer/Makefile >Current/Patch/Makefile.patch
diff Original\ Sources/hermes/README $curSrcVer/README.md >Current/Patch/README.patch
diff Original\ Sources/hermes/mktarball.sh $curSrcVer/mktarball.sh >Current/Patch/mktarball.sh.patch

cd ..

#*****Start Configuration \ Variables*****
name="LiDE"    #Name of project.
version="`grep PROG_VERSION $name.h | cut -f2 -d \"\\"\"`"  #Replace everything between Parentheses with "`Your command to extract version of project`"
files="nwio.c $name.c $name.h Makefile ChangeLog.md README.md Source mktarball.sh" #List files\directories you want to include in your source code distribution.
#*****End Configuration \ Variables*****

echo "[$name] making tarball..."
mkdir $name-$version
tar --exclude=CVS --exclude-backup -cf temporaryplace-$version.tar $files
tar -C $name-$version -xf temporaryplace-$version.tar
rm -f $name-$version.tar.gz
tar -czf $name-$version.tar.gz --numeric-owner $name-$version
rm -rf $name-$version
rm -rf temporaryplace-$version.tar
chmod 0644 $name-$version.tar.gz
echo "[$name] done"
