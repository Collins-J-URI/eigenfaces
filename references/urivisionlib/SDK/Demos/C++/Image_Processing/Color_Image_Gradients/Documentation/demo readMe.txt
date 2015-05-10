This project demonstrates the use of the ColorImageGradient classes
    o ColorImageGradient_Sobel
    o ColorImageGradient_Gaussian
A ColorImageGradient operates on a RasterImage_RGBa in one of the
following two modes:

    - GRAD_AVE_OF_COLOR_COMPONENTS
        In this mode, separate gradients are calculated at each pixel for 
        the red, green, and blue components, and the gradient at the pixel
        is the average of these three gradients
        
    - GRAD_LARGEST_COLOR_COMPONENT        
        In this mode, separate gradients are calculated at each pixel for 
        the red, green, and blue components, and the gradient at the pixel
        is the one of these three component gradients with the largest
        magnitude
