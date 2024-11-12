Raytracing Project done using C++. Renders shapes in scene with properties specified by user. Following properties can be specified: 
* Resolution
* Colour of shapes in scene
* Some options for properties of shapes (light emitting, reflective, or opaque)
* Number of shapes on screen, size, location
* Viewport location and angle
* Video capabilities

Since this is run on CPU with C++, it is not real-time. However, I made it so that users can create a scene, and define changepoints - eg. at start of video have camera facing x direction and at x location, and be facing y direction and at y location by the end of the video. Then, the program would automatically interpolate between the difference in x and y values to create a smooth video.

What I would do differently:
In this project I learned the importance of parallelization of some kind, i.e. leveraging the GPU and using API's such as Vulkan, OpenGL, or DirectX. If I were to do this again I would definitely use one of these, and this would allow the raytracer to be more true to life and real-time.
