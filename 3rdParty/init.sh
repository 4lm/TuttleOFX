#!/bin/bash
BOOST_FILENAME=boost_1_41_0
BOOST_ARCHIVE=$BOOST_FILENAME.tar.bz2
BOOST_URL=http://sourceforge.net/projects/boost/files/boost/1.41.0/$BOOST_ARCHIVE/download

EXPAT_URL=http://sourceforge.net/projects/expat/files/expat/2.0.1/expat-2.0.1.tar.gz/download

getAndUncompress() {
	echo downloading $1
	wget -nv $1&&\
	uncompress $2&&\
	echo erasing file $2&&\
	rm $2
	echo finished
}

uncompress(){
	EXTENSION=`echo $1|awk -F . '{print $NF}'`
	echo uncompression file $1
	[ "$EXTENSION" == "bz2" ]&&tar xfj $1&&return 0
	[ "$EXTENSION" == "gz" ]&&tar xfz $1&&return 0
	return 1
}

echo ">>> GETTING Boost"
getAndUncompress $BOOST_URL $BOOST_ARCHIVE
ln -s $BOOST_FILENAME boost

echo ">>> BUILDING Boost JAM"
CURRENT_DIR=`pwd`
cd $CURRENT_DIR/boost
source `pwd`/bootstrap.sh
cd $CURRENT_DIR

echo ">>> PREPARING bjam.sh script"
BJAM=../bjam.sh
rm -f $BJAM
echo "export BOOST_BUILD_PATH=`pwd`/boost/tools/build/v2">>$BJAM
echo "`pwd`/boost/bjam \$*">>$BJAM
chmod u+x $BJAM
