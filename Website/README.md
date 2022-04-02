# Máquina de Ensaio de Tração

## Client side files for the ESP32 Webserver

These are the files created for the webserver. It is a basic HTML website, displaying a "Load x Deformation" Chart, the same of the Windows Application. The Chart is powered by *Highcharts.js*.


# Known Issue
The webserver has a major design **flawn**, wich is that it uses HTTP requests to get data from the webserver. The client asks for data each 200ms, same time used to get the reading on the sensors. It leads to blocking during the reads, even though the tasks are run asyncronously, the data is locked by mutexes. 
So the time could go up to 500ms bettwen reads.

It has to be changed to websockets, auto updating the data, with sockets being sent only by the server.



