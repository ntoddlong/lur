Welcome to Lehigh Underwater Robotics documentation
===================================================

`Main repository <https://github.com/ntoddlong/lur/>`_

Check out the :doc:`getting_started` section for further information, including
the :ref:`directory_structure` of the project.

This documentation is broken into the following sections:

.. toctree::

   getting_started
   docker
   simulation
   ros
   computer_vision
   sonar
   controller

Getting Started
---------------
This section describes the process of setting up the environment and navigating the docs

Docker
------
This section documents the docker image that allows for simple testing. Eventually we may try to house everything in the container, even on the jetson... we'll see.

.. warning::

   Currently under active development, I have only run this on my personal machine and jetson architecture is arm so not ready there
   see nvidia docs `here <https://developer.nvidia.com/embedded/learn/tutorials/jetson-container/>`_ for details

Simulation
----------
TODO

ROS
---
This section gives an intro to the `ros <https://docs.ros.org/en/foxy/index.html>`_ environment on the jetson.

.. note::

   While it makes things easier, much of ros isn't needed for our use (for example topics are just tcp/ip socket connections). With performance, simplicity, and ease of use in mind, it could eventually be beneficial to redesign things.
   Look into `microros <https://micro.ros.org/>`_

Computer vision
---------------
This section goes into detail about the computer vision. We use a `YOLO <https://pjreddie.com/darknet/>`_ model with `OpenCV <https://opencv.org/>`_ to recognize images.

Sonar
-----
TODO

Controller
----------
This section describes the frontend controller. Very early stages.
`DearImGui <https://github.com/ocornut/imgui>`_

.. note::

   This project is under active development.
