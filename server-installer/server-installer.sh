#!/bin/bash 
#  @author: Dovydas Girdvainis 
#  @date  : 2018-10-26 

## Private variables

rootDir=$(pwd)
dataDir=data
permisions=755
os_type="$(cat /etc/os-release | grep ^ID=)"
suported_os=("ID=debian","ID=ubuntu")

## Colours codes for the installer

RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
PURPLE='\033[1;35m'
NC='\033[0m'

check_os () {

if [[ ! "${suported_os[*]}" =~ "$os_type" ]];
then 
	echo -e "$os_type is unsuported!"
	exit 1
fi

}

do_build () {

	## Get the time from NTP server
	apt-get update

	## Sync the system time with NTP server

	hwclock --systohc
	mkdir gateway-install
	cd gateway-install

	## Install basic packages

	echo -e "$BLUE Step 1 out of 9. Installing the necessary packages. $NC"

	apt-get install -y xml-twig-tools binutils build-essential autoconf automake git unixodbc-dev mysql-server mysql-client
	
	if [ $os_type = "ID=ubuntu" ]; 
	then
		mkdir -p /usr/lib/x86_64_linux-gnu/odbc
		wget https://dev.mysql.com/get/Downloads/Connector-ODBC/8.0/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz
		tar -xvfz mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz -C /usr/lib/x86_64_linux-gnu/odbc
	else
		apt-get install -y libmyodbc
	fi
	
	echo -e "$GREEN Step 1 was succesfully completed. $NC"

	## Configure ODBC

	echo -e "$BLUE Step 2 out of 9. Configuring the ODBC service. $NC"

	mv $rootDir/$dataDir/odbc.ini /etc/
	mv $rootDir/$dataDir/odbcinst.ini /etc/

	echo -e "$GREEN Step 2 was succesfully completed. $NC"

	echo -e "$BLUE Step 3 out of . Installing Leshan demo server. (Legacy demo server) $NC"

	## Download and install Leshan
	apt-get install -y default-jre default-jdk maven
        wget https://hudson.eclipse.org/leshan/job/leshan/lastSuccessfulBuild/artifact/leshan-server-demo.jar
	mkdir -p /opt/leshan
        cp leshan-server-demo.jar leshan-bsserver-demo.jar /opt/leshan
	
	## Configure Leshan Server
	mkdir -p /etc/leshan
        cp $rootDir/$dataDir/leshan /etc/init.d/leshan
        cp $rootDir/$dataDir/leshan-bootstrap /etc/init.d/leshan-bootstrap
        cp $rootDir/$dataDir/bootstrap.json /etc/leshan/
        chmod $permisions /etc/init.d/leshan
        chmod $permisions /etc/init.d/leshan-bootstrap

	echo -e "$GREEN Setp 3 succesfully completed $NC"

	echo -e "$BLUE Step 8 out of 9. Installing OPC UA server. $NC"

	 while [ ! -e /opt/opcua/opcua-run.sh ] || [ ! -s /opt/opcua/opcua-run.sh ] && [ $counter -gt 0 ]
        do
                tar -pxvf $rootDir/$dataDir/opcua-server*.tar -C /opt/
                counter=$(expr $counter - 1)
        done

        if [ $counter -eq 0 ]
        then
                echo -e "$RED Step 8 failed. Extraction of OPC UA server files has failed."
        fi

        chown root:root /opt/opcua
        chmod -R $permisions /opt/opcua

        ## Install the OPC UA server startup files and crontab
        cp $rootDir/$dataDir/asneg /etc/init.d/
        chown root:root /etc/init.d/asneg
        chmod $permisions /etc/init.d/asneg
        cp $rootDir/$dataDir/crontab /etc/crontab
        chown root:root /etc/crontab
        chmod $permisions /etc/crontab
 
        echo -e "$GREEN Step 8 succesfully completed. $NC"

	if [ -e /opt/opcua/cfg/etc/OpcUaStack/Nodes/dbConfig.xml ]
        then
                echo -e "$BLUE Step 9 out of 9. Setting up the SQL database. $NC"
                sqlUserName="$(xml_grep 'UserName' /opt/opcua/cfg/etc/OpcUaStack/Nodes/dbConfig.xml --text_only)"
                sqlPassword="$(xml_grep 'Password' /opt/opcua/cfg/etc/OpcUaStack/Nodes/dbConfig.xml --text_only)"

                while : ; do
                        echo -e "$PURPLE Please enter the SQL root pasword: $NC"
                        read -s rootPswd
                        mysql --user=root --password=$rootPswd -e "CREATE USER '$sqlUserName'@'localhost' IDENTIFIED BY '$sqlPassword';"

                        if [ "$?" -eq 0 ]
                        then
                                break;
                        else
                                echo -e "$RED Step 9 failled. Failed creating the user. Wrong mySQL root password. $NC"
                        fi
                done

                mysql --user=root --password=$rootPswd -e "GRANT ALL PRIVILEGES ON * . * TO '$sqlUserName'@'localhost';"
                mysql --user=root --password=$rootPswd -e "FLUSH PRIVILEGES;"

                echo -e "$GREEN Step 9 succesfully completed. $NC"
        else
                echo -e "$Red Step 9 failled. Server configuration files do not exist."
        fi

}

check_os
do_build
