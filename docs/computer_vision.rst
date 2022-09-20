Computer Vision
===============

.. _OpenCV:
.. _YOLO:

OpenCV
------
`OpenCV <https://opencv.org/>`_

Open a VideoCapture object for each camera and read the frame data into a Mat object

.. note::

   It is necessary to run the image processing work on the GPU of the jetson. OpenCV was compiled from source with special flags to enable this. See our OpenCV compile script `here <https://github.com/ntoddlong/lur/blob/main/opencv_build.sh>`_

YOLO
----
`YOLO <https://pjreddie.com/darknet/yolo>`_
