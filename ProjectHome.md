This wiki was created as a companion page for the actual Master Thesis that is written at IHA (http://www.iha.dk) running from the beginning of the 2Q 2010, until the end of the 4Q 2010. As the title suggests, this thesis will be written to gain knowledge concerning wireless communication with sensors with energy efficiency as the main angle.
The thesis is written by Claus Andersen and Esben Sune Rasmussen.

# Introduction #

The extremely low cost of embedded microcontrollers and RF transceivers has opened up a new world of possibilities in pervasive healthcare. It is now possible to place several sensors on a patient without bothering him/her with wires to connect these. This again leads to new ideas in sensors and things that can be measured in real time while the patient is at home.
However, existing solutions for connecting wireless sensors often have a sever overhead either power wise or in the development cycle excluding it from a number of uses.

## Purpose ##

This project has a theoretical and a practical part:
  * List and evaluate existing techniques for reducing power consumption in embedded network systems.
  * Build a prototype framework that employs selected low power techniques and that enables easy sensor development.

## Problem description ##

Is it possible to lower power consumption in the communication infrastructure compared to the existing solutions at IHA's SIH while still maintaining a flexible and easy to use platform?

# Background #

IHA has focused on healthcare projects since 2006 ([SIH](http://www.iha.dk/Sundhedsteknologier-i-Hjemmet-9259.aspx)) and has come up with several interesting projects, such as:
  * Monitoring heart rate variability
  * Monitoring fluid balance
  * Epileptic seizure detector
  * and several more...

Several different sensors may be connected to one person at a time, each sensor will monitor this person for various data at various rates. Some sensors may require real-time properties and others may be more relaxed and relay data through other sensors.

Since the development in battery capacity has stagnated and since there are limitations to the physical size of the sensor platform (battery, sensor, processor, communication device and casing) it is necessary to optimize the sensor platform to use less power.

# Method - Approaching the problem #

  * Gather knowledge of previous work on both wireless sensor networks and low power embedded design (at IHA and in the world).
  * Test and weigh different solutions for use in this particular context (healthcare/SIH).
  * Develop a prototype solution and produce a report describing the gathered knowledge and the results achieved.

# Expected results #

A report will be produced that highlight techniques used in different WSN (Wireless Sensor Network) projects. The results of an analysis of each technique will be presented in the report.

A two part software framework will be produced that enables new sensors to be developed:
  * A C framework that provides the operating system, network stack and sensor sampler functionality needed to create a sensor.
  * A C# class library that manages a sensor network.
A demonstration application using the two parts of the framework to produce a basic sensor network will be implemented.

# Resources #

Needed hardware:
  * Smartphone (ARM, Android)
  * PC (x86, Windows)
  * Sensor platform (ATmega128RFA1)

# Project Schedule #

Due date - Description
  * **Apr 2010** <br> Master thesis description.<br>
<ul><li><b>Jul 2010</b> <br> Initial project activities. Research related articles, and order required hardware/software for implementation/testing.<br>
</li><li><b>Sep 2010</b> <br> Weigh and research results from initial research. Should result in one or more solutions for the implementation.<br>
</li><li><b>Oct 2010</b> <br> Implement and benchmark the selected solution.<br>
</li><li><b>Nov 2010</b> <br> Review results from the solution and conclude documentation.<br>
</li><li><b>Dec 2010</b> <br> Project delivery.</li></ul>

