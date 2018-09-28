#!/bin/bash
#  @author: Dovydas Girdvainis 
#  @date  : 2018-09-10 

## Paths
PACKER_DIR=$(pwd) 
PACKAGE_DIR=${PACKER_DIR}/gateway-installer-package
BINARIES_ROOT_DIR=../../../../asneg/ 
GATEWAY_INSTALLER_DIR=${PACKER_DIR}/gateway-installer/

## Private variable declaration 
PACKAGE_TYPE="" 
BINARIES_DIR="" 
BOOST_VER=""
ARCH=""
PACKAGE_VERSION=""
PROJECTS=()

## Color codes
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
PURPLE='\033[1;35m'
NC='\033[0m'

usage () {
	echo "-t | --type	- specify the package type ( DEBUG | RELEASE )"
	echo "-a | --arch    	- specify the packaing architechture (x86 | arm)"
	echo "-v | --ver     	- specify the package label version"
	echo "-b | --boost	- OPTIONAL specify boost version ( 1_54_0 | 1_67_0 )"
	echo "-p | --project	- OPTIONAL specify project specific xml descriptors (for example Ameli or Parsifal), can spcify more than one"
	echo "-h | --help	- prints this message"
	echo "builder.sh --type DEBUG --arch arm --ver 1.2 --boost 1_54_0 --project Ameli --project Parsifal"
}


doPackaging () {

## Package opcua server
${PACKER_DIR}/../opcua_packager/packageOPCUA.sh -t $PACKAGE_TYPE -a $ARCH -v $PACKAGE_VERSION -b $BOOST_VER
ret_code=$? 

if [ $ret_code -ne 0 ]; 
then 
	echo -e "$RED Failed executind OPC UA packager. $NC"
	exit 1
fi

if [ ! -d "$PACKAGE_DIR" ]; 
then 
	## IF NOT create the directory
	mkdir $PACKAGE_DIR
	cp -R ../gateway-installer/ ./

else 
	## IF YES delete the old one and create a new 
	rm -rf $PACKAGE_DIR $GATEWAY_INSTALLER_DIR
	mkdir $PACKAGE_DIR
	cp -R ../gateway-installer ./

fi

## Copy the gateway installer files to packaging directory 
cp -rf ${GATEWAY_INSTALLER_DIR}* $PACKAGE_DIR

## Move the tar file to data directory
mv ${PACKER_DIR}/*.tar ${PACKAGE_DIR}/data

## Edit the installer file 
sed -i "8s/1.4.0/${PACKAGE_VERSION}/" "${GATEWAY_INSTALLER_DIR}/gateway-installer.sh"

cd $PACKAGE_DIR
tar -cvzf ../gateway-installer-v${PACKAGE_VERSION}_${RELEASE_TYPE}.tar.gz *

## Remove the package directory
rm -rf $PACKAGE_DIR

}

while [ -n "$1" ];
do
	case $1 in 
		-t | --type)
			shift
			if [ -n "$1" ]; 
			then 
				PACKAGE_TYPE=$1
			else 
				usage
			fi
			;;
		-a | --arch) 
			shift
			if [ -n "$1" ]; 
			then 
				ARCH=$1
			else 
				usage
			fi
			;;
		-v | --ver) 
			shift
			if [ -n "$1" ]; 
			then 
				PACKAGE_VERSION=$1
			else 
				usage
			fi
			;;
		-b | --boost)
			shift
			if [ -n "$1" ]; 
			then 
				BOOST_VER=$1
			else 
				usage
			fi
			;;
		-p | --project)
			shift 
			if [ -n "$1" ]; 
			then 
				PROJECTS+=$1
			else 
				usage
			fi
			;;
		-h | --help)
			usage
			exit 0 
			;;			
		*)
			usage
			exit 1
			;;
	esac
	shift
done

doPackaging

