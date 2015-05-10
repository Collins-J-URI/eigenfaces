   This simple application demonstrates how to get and plot the histogram  
   of a RasterImage_gray object.                                           
   The input image is displayed in the left subwindow                      
   Initially the histogram of that image is displayed in the right half.   
   A right-click drop menu in the right half allows the user to select     
       - the render mode (LIN_LIN or LIN_LOG) of the histogram             
       - whether or not to display the axes                                
       - whether or not to display a title for the histogram.              
           Actually, this is not implemented yet because I am having 2nd   
           thoughts about the pertinence of this functionality which       
           forces me to use glut within the library (something that I      
           have been trying to avoid)                                      
       - the color of the histogram                                        
       - the color of the histogram's background                           
       - the color of the histogram' axes                                  
       - the color of the histogram' title                                 
