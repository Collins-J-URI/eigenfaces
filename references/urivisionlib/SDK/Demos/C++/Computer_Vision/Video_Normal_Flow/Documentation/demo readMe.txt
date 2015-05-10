This demo shows how to use a NormalFlowCalculator class (using a QuickTime
movie as video source)

Note that the code in this demo is not the most efficient possible.  For example,
the movide file is read into a color image first, then copied into the gray-level
image that is processed.  We only do this because we want to display the original
image in a subwindow.  If we wanted to optimize somewhat,, we would read
directly into the gray-level image.