# Automatic Batch Code Printer

This repository contains the design and implementation details for the Automatic Batch Code Printer, developed as part of the EN2160 - Electronic Design Realization course at the University of Moratuwa. The project leverages ATmega microcontrollers, shift registers, and other electronic components to achieve precise and reliable printing on various surfaces.

## Table of Contents
- [Introduction](#introduction)
- [Schematics](#schematics)
- [PCB Design and Assembly](#pcb-design-and-assembly)
- [Enclosure Design](#enclosure-design)
- [Detailed Programming Information](#detailed-programming-information)
- [Daily Log Entries](#daily-log-entries)
- [Getting Started](#getting-started)
- [Contributors](#contributors)
- [License](#license)

## Introduction

The Automatic Batch Code Printer is designed to generate and print batch codes on multiple surfaces with high precision. It integrates several ATmega microcontrollers that handle tasks such as bit pattern generation, signal processing, pressure control, and surface detection, ensuring high-quality and efficient printing.

### Key Features
- **High Precision Printing**: Utilizes piezoelectric technology and PWM signals for fine control over ink droplet formation.
- **Versatility**: Capable of printing on various surfaces with real-time adjustments for optimal print conditions.
- **Robust and Reliable**: Features fail-safe mechanisms for pressure control and surface detection to prevent printing errors.
- **Efficient and Environmentally Friendly**: Lower energy consumption and minimal ink wastage compared to traditional printing methods.

## Schematics

Detailed schematics for the entire system, including the main and secondary microcontrollers, power supply, shift registers, and pressure control circuits, are provided. These schematics are crucial for understanding the layout and connections between components.

## PCB Design and Assembly

### PCB Design
The PCB design for this project was meticulously crafted to ensure signal integrity, power management, and minimal interference. The design process included optimal component placement, routing, and adherence to industry standards.

### PCB Assembly
Photographs of both the bare and soldered PCBs are included, along with evidence of PCB testing to ensure functionality before final assembly.

## Enclosure Design

The project also includes a custom-designed enclosure for the batch code printer. The enclosure is designed to house all the critical components securely while providing easy access for maintenance. Detailed design drawings and photographs of the assembled enclosure are available.

## Detailed Programming Information

The programming section includes the full source code used to control the printer, handle user inputs, and manage the printing process. This code is essential for replicating the functionality of the system and includes:
- ASCII to bitmap conversion for dot matrix printing.
- PWM signal generation for controlling the print head.
- Proximity sensor integration for accurate surface detection.
- Pressure monitoring and control algorithms.

## Daily Log Entries

A detailed log of the project development process is provided, documenting each stage from initial concept to final implementation. This includes design reviews, component selection, circuit design, programming, PCB layout, and enclosure fabrication.

## Getting Started

### Prerequisites
- **Arduino IDE**: Required for compiling and uploading the provided code to the microcontrollers.
- **Microchip Studio**: Used for programming the ATmega microcontrollers.

### Instructions
1. **Hardware Setup**: Assemble the circuit and connect all components as per the provided schematics and PCB layout.
2. **Software Upload**: Use the Arduino IDE to upload the code to the microcontrollers.
3. **Operation**: Power up the system, input the desired batch code using the provided keyboard interface, and initiate the printing process.

## Contributors

- **Anjula M.K** (210368V)
- **Meemanage N.A** (210385U)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
