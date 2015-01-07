# LZO Compression Library

## Overview

Name

**LZO Compression Library**

Description

This library allows you to use LZO compression as compared to the inflate/deflate compression used by default by dynaTrace. LZO requires less CPU but the resulting amount of data is higher.

dynaTrace Version

3.x, 4.x, 5.x

dynaTrace Components

dynaTrace Server, dynaTrace Client, dynaTrace Collector

Version

3.0.1

Support

[Supported](https://community/display/DL/Support+Levels#SupportLevels-Supported)

Download

Choose the Library to download according to your OS from the list below -  
or [Download the Source Code for the dynaTrace LZO Library Wrapper](attachments_51183654_1_dtLZOWrapper.zip)

AIX ppc32:

[aix-ppc32.tar](attachments_11796484_2_aix-ppc32.tar)

AIX ppc64:

[aix-ppc64.tar](attachments_11796485_2_aix-ppc64.tar)

HP-UX IA64 32:

[hpux-32.ia64.tar](attachments_11796486_1_hpux-32.ia64.tar)

HP-UX IA64 64:

[hpux-64.ia64.tar](attachments_11796488_1_hpux-64.ia64.tar)

HP-UX PA-RISC 2.0 32:

[hpux-32.pa-risc.tar](attachments_11796487_1_hpux-32.pa-risc.tar)

HP-UX PA-RISC 2.0 64:

[hpux-32.pa-risc.tar](attachments_11796487_1_hpux-32.pa-risc.tar)

Linux IA64:

[linux.ia64.tar](attachments_11796490_1_linux.ia64.tar)

Linux x64:

[linux.x64.tar](attachments_11796491_1_linux.x64.tar)

Linux x86:

[linux.x86.tar](attachments_11796492_1_linux.x86.tar)

Solaris SPARC-32:

[solaris.sparc32.tar](attachments_11796493_1_solaris.sparc32.tar)

Solaris SPARC-64:

[solaris.sparc64.tar](attachments_11796494_1_solaris.sparc64.tar)

Solaris x64:

[solaris.x64.tar](attachments_11796495_1_solaris.x64.tar)

Solaris x86:

[solaris.x86.tar](attachments_11796496_1_solaris.x86.tar)

Windows x86:

[windowsx86.zip](attachments_11796503_1_windowsx86.zip)

Windows x64:

[windowsx64.zip](attachments_11796497_1_windowsx64.zip)

zLinux s390:

[zlinux-s390.tar](attachments_11796499_1_zlinux-s390.tar)

zLinux s390x:

[zlinux-s390x.tar](attachments_11796500_1_zlinux-s390x.tar)

z/OS s390:

[zOS-s390.tar](attachments_11796501_1_zOS-s390.tar)

z/OS s390x:

[zOS-s390.tar](attachments_11796501_1_zOS-s390.tar)

## Description

LZO compression is a tradeoff between default inflate/deflate and uncompressed communication regarding CPU utilization and compression-rate.

The core LZO compression algorithm has been developed by Markus Franz Xaver Johannes Oberhumer, see <http://www.oberhumer.com/opensource/lzo/>.

LZO and miniLZO are Copyright (C) 1996-2008 Markus Franz Xaver Johannes Oberhumer.

## Installation

  1. Choose and download the package from the list above according to your OS. 

    * The architecture (64bit/32bit) must match the VM of the installed dynaTrace component! 

  2. Copy the shared library to the `<dynaTrace_home>/tools/lib` directory. 

Please note that both sides of communication need the corresponding library to use LZO compression. If the LZO library is only installed on one peer, the default inflate/deflate compression facilities
will be used. To check if LZO compression is enabled, watch out for log lines similar to "INFO
[PassiveConnector](https://community/pages/createpage.action?spaceKey=DL&title=PassiveConnector&linkCreation=true&fromPageId=11567110) connection to HOSTNAME:3466:Inbound:Plain:LZO has been accepted"
in the corresponding logs.

## Configuration

### How to configure LZO compression between dynaTrace Server and dynaTrace Client

Shutdown the Server and Client and copy the shared library to the `<dynaTrace_home>\tools\lib` directory on server- as well as on client-side. Start the Server and connect the Client. Beginning with
dynaTrace 3.5.1 compression is enabled automatically. For older versions you have to enable compression manually. To do so, open the `"Server Preferences"` dialog in the Client and choose the menu
item `"Connectivity"`. Choose the checkbox `"enable data compression"` to enable compression for communication between Server and Client.

![images_community/download/attachments/11567110/EnableCompression.png](images_community/download/attachments/11567110/EnableCompression.png)  
Figure: Enable data compression between Server and Client

Disconnect and reconnect the Client to finish the LZO compression installation.

### How to configure LZO compression between dynaTrace Server and dynaTrace Collector

Shutdown the Server and Collector and copy the shared library to the `<dynaTrace_home>\tools\lib` directory on server- as well as on collector-side. The dynaTrace Collector's setup is stored in
_collector.config.xml_ located in the _./conf/_ directory of the

dynaTrace Collector

installation.

In case you are using Collector instances the _collector.config.xml_ can be found in `<dynaTrace_home>\collector\instances\yourInstance\conf`.

The default _collector.config.xml_ looks as follows having the _compress_ attribute set to _true_:

    
    
    <?xml version="1.0" encoding="UTF-8"?>
    <dynatrace version="3.0.1.1598">
      <collectorconfig memento.version="3.0.1.1598" proxypassword="" agentport="9998" useproxy="false" authstring="" serverport="6698" proxyport="8080" embedded="false" compress="true" usessl="false"
                       proxyhost="" useproxyauthentication="false" proxyusername="" name="dynaTrace Collector" watchdogtimeout="10" agentaddress="" serveraddress="localhost">
        <loggingconfig append="true" console="INFO" level="INFO" maxfiles="5" path="../log/collector/dynaTrace Collector" html="false" maxbytes="10485760" />
        <buffers serverbuffercount="10" agentbuffersize="32768" serverbuffersize="524288" />
        <plugins>
          <loggingconfig append="true" console="WARNING" level="INFO" maxfiles="5" path="../log/collector/dynaTrace Collector" html="false" maxbytes="10485760" />
        </plugins>
      </collectorconfig>
    </dynatrace>

The setup of LZO data compression between the Server and the Collector is finished.

