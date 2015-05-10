This project demonstrates the use of the scale rendering property of the RasterImage
class (implemented using the glZoomPixel function), as opposed to the raster
scaling property implemented in other classes such as the RasterPyramid class.

As in the simple IO demo, the user can cycle through images by pressing the '[' and 
']' keys.  In addition, hitting the ',' key ('<' minus the shift) will decrease the
scale at which the image is rendered (down to 1/10th of original size) while hitting
the '.' key ('>' minus the shift) will increase the renedering scale up to twice
the original size. 