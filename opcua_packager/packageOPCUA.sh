#!/bin/bash 
#  @author: Dovydas Girdvainis 
#  @date  : 2018-09-10

## Paths
PACKER_DIR=$(pwd) 
BINARIES_ROOT_DIR=../../../../asneg/ 
PACKAGE_DIR=$PACKER_DIR/opcua/
DEPENDENCY_BASE_DIR=../../../../../../

## Private variable declarations
ARG_COUNT=$#
MIN_ARG_COUNT=2
PACKAGE_TYPE="" 
BINARIES_DIR="" 
BOOST_VER=""
ARCH=""
PACKAGE_VERSION=""
PROJECTS=()
GIT_USER=""
GIT_PWD=""

SUPPORTED_PACKAGE_TYPES=("DEBUG","RELEASE")
SUPPORTED_ARCHS=("x86-amd64","arm")
SUPPORTED_BOOST_VERSIONS=("1_54_0","1_67_0")
SUPPORTED_PROJECTS=("PARSIFAL","AMELI")

## Color codes
RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
PURPLE='\033[1;35m'
NC='\033[0m'

## Internal functions 

usage () {
	echo "-t | --type	- specify the package type ( DEBUG | RELEASE )"
	echo "-a | --arch    	- specify the packaing architechture (x86 | arm)"
	echo "-v | --ver     	- specify the package label version"
	echo "-b | --boost	- OPTIONAL specify boost version ( 1_54_0 | 1_67_0 )"
	echo "-p | --project	- OPTIONAL specify project specific xml descriptors (for example Ameli or Parsifal), can spcify more than one"
	echo "-h | --help	- prints this message"
	echo "packageOPCUA.sh --type DEBUG --arch arm --ver 1.2 --boost 1_54_0 --project Ameli --project Parsifal"
}


packageArmBinaries () {

## Check if openssl libs for arm exist
if [ -d "${DEPENDENCY_BASE_DIR}/openssl/lib" ]; 
then 
	cp -rf ${DEPENDENCY_BASE_DIR}/openssl/lib/* $PACKAGE_DIR/bin
else 
	echo -e "$RED NO OPENSSL for arm has been found! Try running the Dependency_Installer! $NC"
	exit 1
fi 

## Check if odbc-arm libs exist
if [ -d "${DEPENDENCY_BASE_DIR}/odbc-$ARCH/lib" ]; 
then 
	cp ${DEPENDENCY_BASE_DIR}/odbc-$ARCH/lib/libodbc.* $PACKAGE_DIR/bin
else 
	echo -e "$RED NO ODBC for arm has been found! Try running the Dependency_Installer! $NC"
	exit 1
fi

## Check if debug version 
if [ "$ARCH,," == "debug" ];
then
## Add remote debuging startup 
touch ${PACKAGE_DIR}/startGDBServer.sh
cat << EOF > ${PACKAGE_DIR}/startGDBServer.sh 
#!/bin/bash
#  @author      : Dovydas Girdvainis 
#  @date        : 2018-09-10 
#  @description : Start a remote debuging server on the host

cd ./bin 
gdbserver :9091 OpcUaServer4 etc/OpcUaStack/OpcUaServer.xml
        
EOF
fi

}

handleProjectSpecifics () {

# Break up array into sets seperated by whitespace
set ' '
PROJECTS=$PROJECTS

for PROJECT in ${PROJECTS[@]}
do

	## Check if Project is supported
	if [[ "${SUPPORTED_PROJECTS[*]}" =~ "${PROJECT^^}" ]] && [[ "${PROJECT^^}" != " " ]]; 
	then 

		echo -e "$GREEN Getting $PROJECT files from git! $NC" 		

		## Copy the project specific xmls 
		mkdir ${PACKAGE_DIR}${PROJECT}-XMLS 
		cp -R ${PACKAGE_DIR}cfg ${PACKAGE_DIR}${PROJECT}-XMLS/		

		## Checkout the correct xml files
		cd ${PACKAGE_DIR}/${PROJECT}-XMLS/cfg/etc/OpcUaStack/Nodes
		rm -rf ./sensor_xml_descriptions
		echo -e "$BLUE Please provide git credentials: $NC"
		git clone https://redmine.hahn-schickard.de/opc-ua-gateway/sensor_xml_descriptions.git 
		cd ./sensor_xml_descriptions
		git checkout ${PROJECT,,}

		## Remove the git links
		rm -rf .git*

		## Remove tests
		rm -rf ./tests 

		## Return to working directory
		cd ${PACKAGE_DIR}

		## Create project specific launchers 
		cp ${PACKAGE_DIR}opcua-run.sh ${PACKAGE_DIR}opcua-run_${PROJECT}.sh

		## Edit project specific launcher 
		sed -i "7s/cfg/${PROJECT}-XMLS\/cfg/" "${PACKAGE_DIR}opcua-run_${PROJECT}.sh"
	else 
		echo -e "$RED ${PROJECT} xml description directory not supported. Supported projects: ${SUPPORTED_PROJECTS} $NC"
	fi
done

}

packageBinaries () {
	BINARIES_DIR=$(pwd)
	echo -e "$GREEN Packaging the OPC UA server binaries from $PURPLE $BINARIES_DIR $GREEN for $PACKAGE_TYPE... $NC"

	cd $PACKER_DIR
 
	## Check if directory exists
	if [ ! -d "$PACKAGE_DIR" ]; 
	then 
		## IF NOT create the directory
		mkdir $PACKAGE_DIR
	else 
		## IF YES delete the old one and create a new 
		rm -rf $PACKAGE_DIR 
		mkdir $PACKAGE_DIR
	fi
	
	## Copy the binary files
	cp -R ${BINARIES_DIR}* $PACKAGE_DIR
	
	## Rename the directory
	mv $PACKAGE_DIR/build-* $PACKAGE_DIR/bin

	## Remove Make files and build config
	rm -rf $PACKAGE_DIR/bin/C* $PACKAGE_DIR/bin/c* $PACKAGE_DIR/bin/BuildConfig.h $PACKAGE_DIR/bin/Makefile $PACKAGE_DIR/bin/OpcUaProjectBuilderConfig.h

	## Add the boost dependencies 
	if [ -d "${DEPENDENCY_BASE_DIR}boost-${ARCH}_${BOOST_VER}/lib" ] 
	then 
		cp ${DEPENDENCY_BASE_DIR}boost-${ARCH}_${BOOST_VER}/lib/* $PACKAGE_DIR/bin
	else 
		echo -e "$RED Boost libraries not found at ${DEPENDENCY_BASE_DIR}boost-${ARCH}_${BOOST_VER}/lib! Try the Dependency_Installer! $NC"
		exit 1
	fi
	
	if [ "$ARCH" = "arm" ];
	then
		packageArmBinaries
	fi

	## Copy the startup script
	cp ${BINARIES_ROOT_DIR}../opcua-run-${ARCH}-${PACKAGE_TYPE,,}.sh $PACKAGE_DIR/

	## Edit the startup script
	sed -i "4s/asneg\/build-${ARCH,,}-${PACKAGE_TYPE,,}/.\/bin/" "${PACKAGE_DIR}opcua-run-${ARCH,,}-${PACKAGE_TYPE,,}.sh"
	sed -i "7s/asneg\/build-${ARCH,,}-${PACKAGE_TYPE,,}/.\/bin/" "${PACKAGE_DIR}opcua-run-${ARCH,,}-${PACKAGE_TYPE,,}.sh"
	sed -i "10s/asneg\/build-${ARCH,,}-${PACKAGE_TYPE,,}/.\/bin/" "${PACKAGE_DIR}opcua-run-${ARCH,,}-${PACKAGE_TYPE,,}.sh"

	## Rename the startup script
	mv ${PACKAGE_DIR}opcua-run-${ARCH,,}-${PACKAGE_TYPE,,}.sh ${PACKAGE_DIR}opcua-run.sh

	## Copy the standard xml files 
	cp -R ${BINARIES_ROOT_DIR}../cfg ${PACKAGE_DIR}

	## Handle other projects
	if [ -n "$PROJECTS" ]; 
	then 
		handleProjectSpecifics
	fi

	cd ../

	## Remove git linking 
	rm -f ${PACKAGE_DIR}/cfg/etc/OpcUaStack/Nodes/sensor_xml_descriptions/.git* 

	## Tar the files 
	tar -cvf opcua-server_v${PACKAGE_VERSION}_${ARCH}_${PACKAGE_TYPE}.tar opcua

	## Remove the package directory 
	rm -rf ${PACKAGE_DIR}

	echo -e "$GREEN Packaging complete! $NC"
	exit 0
}

doPackaging () {
	if [ -d "${BINARIES_ROOT_DIR}build-${ARCH}-${PACKAGE_TYPE,,}" ]; 
	then
		cd ${BINARIES_ROOT_DIR}build-${ARCH}-"${PACKAGE_TYPE,,}"
		packageBinaries
	else 
		echo -e "$RED Directory build-${ARCH}-${PACKAGE_TYPE,,} in $BINARIES_ROOT_DIR does not exist! $NC"
	fi
}

checkMandatoryArgs () {

## Check packaging type
if [[ ! "${SUPPORTED_PACKAGE_TYPES[*]}" =~ "${PACKAGE_TYPE^^}" ]];

then 
	echo -e "$RED Unsupported package type : $PACKAGE_TYPE ! $NC"
	echo -e "$PURPLE Supported package types are: $SUPPORTED_PACKAGE_TYPES $NC"
	exit 1
fi

## Check packaging architechure 
if [[ ! "${SUPPORTED_ARCHS[*]}" =~ "$ARCH" ]];
then 
	echo -e "$RED Unsupported architecture type : $ARCH ! $NC"
	echo -e "$PURPLE Supported architecture types are: $SUPPORTED_ARCHS $NC"
	echo -e "$PURPLE Architecture argument is case sensitive! $NC"
	exit 1
fi

## Check packaging label 
if [ -z "$PACKAGE_VERSION" ];
then 
	echo -e "$RED No versioning label has been provided! $NC" 
	exit 1
fi

## Check boost argument 
if [ -z "$BOOST_VER" ];
then 
	echo -e "$PURPLE Boost version not set, setting to default 1_67_0 $NC"
	BOOST_VER="1_67_0"
else 
	if [[ ! "${SUPPORTED_BOOST_VERSIONS[*]}" =~ "$BOOST_VER" ]];
	then 
		echo -e "$RED Unssuported boost verseion $BOOST_VER ! $NC"
		echo -e "$PURPLE Supported boost versions: $SUPPORTED_BOOST_VERSIONS $NC"
		exit 1
	fi
fi

}

if [ $ARG_COUNT -lt $MIN_ARG_COUNT ]; 
then 
	usage
	exit 1
fi

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
				PROJECTS+=" " 
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

checkMandatoryArgs 
doPackaging
