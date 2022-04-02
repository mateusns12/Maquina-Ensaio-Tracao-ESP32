# Máquina de Ensaio de Tração

## Code for the Application Interface of the Tensile Testing Machine 1.0 

This program was developed using Windows Forms, in Visual Studio 2017. It does need improvements on the UI, code organization and full implementation of the proposed functions.

Its main purpose is to communicate with the microcontroler via serial, while displying a Live "Load x Deformation" Chart during the Tensile test. 

## Notes

    - Developed in: .NET Framework 4.5.2, using C#.

    - Windows Forms controls, Serial Port and .NET designer are not implemented on current .NET Core, so the code can't be fully ported to .NET core. It works on .NET Framework 4.8.

    - New version to be developed in WPF.


## Especial Thanks

Thanks to **Curious Scientist** for the basic code implementation used in this project. [Source](https://curiousscientist.tech)
# Known Issues

    - Exception launched when closing the App without selecting a serial port. This was reported in Visual Studio 2022.

    - There are some problems with the scale of values read from the serial port. It was fixed during the presentation, but wasn't inclued in this code. It needs to be reimplemented. Values are inserted in microns.

    - No desing pattern was used, so the code behind is prone to security issues.