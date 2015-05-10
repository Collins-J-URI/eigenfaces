This project demonstrates the use of RasterImage_binary objects, and
in particular the access to their raster.

The application's window has 4 quadrant displaying the following information:

|	o upper-left quadrant: original (gray-level) image                      |
|   o upper-right quadrant: a binary copy of that image (QuickDraw set the  |
|     threshold at 127-128)                                                 |
|   o lower-left quadrant: binary copy of an attenuated version of the      |
|       input image (intensity * 0.8)                                       |
|   o lower-right quadrant: modified version of the U-R binary image (added |
|       1 diagonal line)                                                    |
