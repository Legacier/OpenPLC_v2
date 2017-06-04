# OpenPLC v2

El propósito de este programa es emular un PLC en una máquina Linux. Este PLC virtual usa el conjunto de aplicaciones de OpenPLC para ejecutar programas IEC 61131-3 y responder a solicitudes MODBUS/TCP. Los programas pueden ser creados usando OpenPLC editor y posteriormente subidos a éste PLC virtual.

OpenPLC soporta dispositivos físicos gracias a las diferentes capas de hardware que posee. Es posible añadir capas de hardware adicionales al proyecto. Por ejemplo, existe una capa específica para RaspberryPi, que le permite controlar sus pines de E/S.

Incluye una aplicación NodeJS que funciona como servidor http, para que el usuario suba sus programas.

Es necesario tener instalados previamente NodeJS y WiringPi (en caso de emplear una Raspberry Pi) para poder usar este programa.

Uso:

1) ./build.sh

2) sudo nodejs server.js

Esto creará un servidor en el puerto 8080. Sólo es necesario abrir el navegador e ir a la dirección localhost:8080. Una vez la aplicación está siendo ejecutada, es posible conectarse al PLC virtual usando cualquier software MODBUS/TCP HMI.

Nota: Esta versión de OpenPLC incluye la capa de hardware necesaria para soportar la plataforma OpenPiController.

--------------------------------------------------------------------------------------------------------------------------------

# OpenPLC v2

This program is intended to emulate a PLC on a Linux machine. This virtual PLC uses the OpenPLC Software Stack to execute IEC 61131-3 programs and reply to MODBUS/TCP requests. Programs can be created using the PLCopen editor and then uploaded to this virtual PLC.

The OpenPLC has different hardware layers to support physical devices. More hardware layers can be added to the project. For instance, there is a hardware layer for the RaspberryPi, which makes the OpenPLC controls its IO pins. 

There is a NodeJS application that works as a http server for the user to upload new programs.

You must have NodeJS and WiringPi (in case you are using Raspberry Pi) installed to use this program. Usage:

1) ./build.sh

2) sudo nodejs server.js

A server will be created on port 8080. Just open your browser and navigate to localhost:8080. After the application is running, you can connect to the virtual PLC using any MODBUS/TCP HMI software.

