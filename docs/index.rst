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

   Currently under active development, I have only run this on my personal machine and jetson architecture is arm so not ready there, see nvidia docs `here <https://developer.nvidia.com/embedded/learn/tutorials/jetson-container/>`_ for details. Also see, `some nice premade containers <https://github.com/dusty-nv/jetson-containers>`_

Simulation
----------

.. todo::

   We are planning to create a simulation using `simulink <https://www.mathworks.com/products/simulink.html>`_

ROS
---
This section gives an intro to the `ros <https://docs.ros.org/en/foxy/index.html>`_ environment on the jetson.

.. note::

   While it makes things easier, much of ros isn't needed for our use (for example topics are just tcp/ip socket connections). With performance, simplicity, and ease of use in mind, it could eventually be beneficial to redesign things. Look into `microros <https://micro.ros.org/>`_

Computer vision
---------------
This section goes into detail about the computer vision. We use `OpenCV <https://opencv.org/>`_ to run a `YOLO <https://pjreddie.com/darknet/yolo>`_ model to recognize images.

Sonar
-----
This section describes our use of sonar. We use bluerobotics `ping-protocol <https://github.com/bluerobotics/ping-cpp>`_

Controller
----------
This section describes our design thoughts for a frontend controller.

.. note::

   Haven't figured out the best way to work with the tether yet but hopefully we can create an interface with the `DearImGui <https://github.com/ocornut/imgui>`_ library to be able to control the drone.
