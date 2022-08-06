Welcome to Lehigh Underwater Robotics documentation
===================================================

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> bf1caec (Updating docs)
=======
>>>>>>> afb988ea70297b9dbbf429ee3d4f71d226add26f
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

Getting Started
---------------
This section describes the process of setting up the environment and navigating the docs

Docker
------
This section documents the docker image that allows for simple testing. Eventually we may try to house everything in the container, even on the jetson... we'll see.

.. warning::

   Currently under active development, I have only run this on my personal machine and jetson architecture is arm so not ready there
   see nvidia docs `here <https://developer.nvidia.com/embedded/learn/tutorials/jetson-container/>`_ for details
<<<<<<< HEAD
<<<<<<< HEAD

Simulation
----------
TODO

ROS
---
This section gives an intro to the `ros <https://docs.ros.org/en/foxy/index.html>`_ environment on the jetson

.. note::

   While it makes things easier, much of ros isn't needed for our use (for example topics are just tcp/ip socket connections). With performance, simplicity, and ease of use in mind, it could eventually be beneficial to redesign things.

Computer vision
---------------
This section goes into detail about the computer vision. We use a `YOLO <https://pjreddie.com/darknet/>`_ model with `OpenCV <https://opencv.org/>`_ to recognize images.

Sonar
-----
TODO
=======
=======
>>>>>>> 41121e6 (Readme for docs)
`Main repository <https://github.com/ntoddlong/robosub22/>`_

<<<<<<< HEAD
Check out the :doc:`getting started` section for further information, including
how to :ref:`installation` the project.
<<<<<<< HEAD
>>>>>>> ec1c01a (Moving index.rst)
=======
>>>>>>> 41121e6 (Readme for docs)
=======
Check out the :doc:`getting_started` section for further information, including
the :ref:`directory_structure` of the project.
>>>>>>> 4035fdd (Updating docs)

This documentation is broken into the following sections:

.. toctree::

   getting_started
   docker
   simulation
   ros
   computer_vision
   sonar

Getting Started
---------------
This section describes the process of setting up the environment and navigating the docs

Docker
------
This section documents the docker image that allows for simple testing. Eventually we may try to house everything in the container, even on the jetson... we'll see.

.. warning::

   Currently under active development, I have only run this on my personal machine and jetson architecture is arm so not ready there
   see nvidia docs `here <https://developer.nvidia.com/embedded/learn/tutorials/jetson-container/>` for details
=======
>>>>>>> bf1caec (Updating docs)
=======
>>>>>>> afb988ea70297b9dbbf429ee3d4f71d226add26f

Simulation
----------
TODO

ROS
---
This section gives an intro to the `ros <https://docs.ros.org/en/foxy/index.html>`_ environment on the jetson

.. note::

   While it makes things easier, much of ros isn't needed for our use (for example topics are just tcp/ip socket connections). With performance, simplicity, and ease of use in mind, it could eventually be beneficial to redesign things.

Computer vision
---------------
This section goes into detail about the computer vision. We use a `YOLO <https://pjreddie.com/darknet/>`_ model with `OpenCV <https://opencv.org/>`_ to recognize images.

Sonar
-----
TODO

.. note::

   This project is under active development.
