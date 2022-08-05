Getting Started
===============

This documentation is broken into the following sections:

.. toctree::

   docker
   simulation
   ros
   computer vision
   sonar

Docker
------
This section documents the docker image that allows for simple testing. Eventually we may try to house everything in the container, even on the jetson... we'll see.
.. warning::
   Currently under active development, I have only run this on my personal machine and jetson architecture is arm so not ready there
   see nvidia docs `here <https://developer.nvidia.com/embedded/learn/tutorials/jetson-container/>` for details

Simulation
----------
TODO

Ros
---
This section gives an intro to the `ros <https://docs.ros.org/en/foxy/index.html>` environment on the jetson
.. note::
   While it makes things easier, much of ros isn't needed for our use (for example topics are just tcp/ip socket connections). With performance, simplicity, and ease of use in mind, it could eventually be beneficial to redesign things.

Computer vision
---------------
This section goes into detail about the computer vision. We use a `YOLO <https://pjreddie.com/darknet/>` model with `OpenCV <https://opencv.org/>` to recognize images.

Sonar
-----
TODO
