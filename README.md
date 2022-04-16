# Máquina de Ensaio de Tração [DEPRECATED]

![Board]
![Language]
![System]
![Framework]

### *This version will no longer be updated, apart of documentation. New **link** for Version 2.0 soon.*

# Original code developed for "Máquina de Ensaio de Tração" Version 1.0

This is the first version of all the codes used for the Tensile Testing Machine, developed for matter "Materiais para Engenharia" (Enginnering Materials), in the course Mechatronics Enginnering, UFSJ, presented in December, 16, 2019.

## Tesile Testing Machine

A Tensile Testing Machine is used to determine mechanical properties of a given material, generating a real time chart with axis: Load(Newtons) x Deformation(%). The machine puts a pulling force on the material, while measuring the raw values of linear displacement of the material (in meters), and the force (in newtons), until the material is torn apart.

This machine uses a linear atuator, built using a steel rod, driven by a high torque DC Motor, generating up to 6400 Newtons of force in a linear axis.

The displacement is gathered using a digital calipher, wich gives the real time value in microns. 

The force is gathered by 4 load cells, wich gives the real time value in kilograms. 

## Contents

In the **ESP32 folder**, there is the original Arduino code for the machine, used in the presentation of the project.

In the **Application folder**, there is the code of the Console, developed in Visual Studio 2017.

In the **Website folder**, there are the files of the client side of the Webserver.

# Known Issues

It is necessary to repair and develop the following: 

    
    - Memory Leaks and acess of shared resources by concurrent threads.

    - Watchdog activated by task IDLE_0, in core 1, by the loop function, (software watchdog not used).

    - Webserver uses HTTP requests to send data to the client, wich consumes unnecessary processing power. Needs to be changed to websockets.

    - No classes defined, non objected oriented programming.

    - Organization and documentation.

# License

This project is under MIT License


[//]: # (LINKS)

[Board]: <https://img.shields.io/badge/Board-ESP32-A70000?style=for-the-badge&logo=espressif>

[Language]: <https://img.shields.io/badge/Language-C++17-00599C?style=for-the-badge&logo=cplusplus>

[Framework]: <https://img.shields.io/badge/Framework-NET 4.5.2-512BD4?style=for-the-badge&logo=dotnet>

[System]: <https://img.shields.io/badge/System-windows-A100FF?style=for-the-badge&logo=windows>