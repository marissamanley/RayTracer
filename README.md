# Ray Tracer Report
## Marissa Manley - CAP4730 Computer Graphics Spring 2022

### Compiling Guide:
See README.txt

### Operation Guide:
Press Z to switch from Orthographic camera to Perspective Camera


**Note:** All functionalities required in the project were implemented and work correctly, so this report does not include subsections that describe required functionalities that did not work or were not implemented.


### Objects and Light Sources in Scene:
**Orthographic Camera** - The center of the orthographic camera is at (0, -120, 0) and has a LookAt vector pointing in the direction (0.3f, 0.4f, 1), which is a tilted down and right-facing direction from above the objects in the scene. The camera canvas size is 256 x 256, while the resolution of the camera is 512 x 512. 

**Perspective Camera** - The center of the perspective camera is at (0, -180, -50) and has a LookAt vector pointing in the direction (0.4f,0.1f,1). The focal length is 590. The camera canvas size is 256x256, while the resolution of the camera is 512 x 512. 

**Ambient Light** - By default, the ambient light in the scene has an intensity of 0.4 and impacts every object in the scene. It is used to create the effects of the ambient shading.

**Directional Light** - By default, a directional light with direction (1.1f, 0.8f, 1.2f) is established within the scene. It impacts every object in the scene and is used to create the effects of the diffuse shading and specular shading, along with the shadows, of the scene objects. 

**Plane** - A flat, grey plane made up of three points spans across the scene on the y-axis value -100. The plane has the default ambient and diffuse shading values and is made to be reflective by the specular shading generation. The number of recursive ray generations is only one.

**Red Sphere** - A non-reflective red sphere is located at (160, -140, 350) with a radius of 40. It takes the default diffuse and ambient shading values.

![Assignment1_EYn0csdFrC](https://user-images.githubusercontent.com/58527286/187338641-61738d21-8c27-4ae4-8511-748f91ad28b0.png)

**Green Sphere** - A non-reflective green sphere is located at (40,-130,220) with a radius of 30. It takes the default diffuse and ambient shading values.

![Assignment1_DRlDMLyseS](https://user-images.githubusercontent.com/58527286/187338938-c762daf2-734d-4f0d-ae92-8ff6eccaecf1.png)

**Tetrahedron** - A non-reflective blue tetrahedron consisting of four triangles is located at points (135, -100, 251), (105, -100, 261), (145, -100, 266), and (125, -140, 264). It takes the default diffuse and ambient shading values.

![Assignment1_0qMtuuKD18](https://user-images.githubusercontent.com/58527286/187339139-09157014-05b4-454d-9d9c-2b73a152736e.png)








### Functionality Compare:
	
Orthographic camera:

Perspective camera:


With/without directional light:






High/Low ambient shading:





With/without shadows:





With/without diffuse shading:






With/without specular shading and relfections:





Program Logic: 
Aside from the content given in the provided script at the beginning of this project, the code consists of various classes, a trace function, a RayTrace function, and a main function. I create a class for a Camera, Light Source, Ray, and Shape, and there are three children classes of Shape: Sphere, Tetrahedron, and Plane.  

All class objects are created in a RayTrace function, which is called in main. An array of shape objects is created and all scene objects are added to the array. Within the for loop that generates the RGB values for each pixel in the screen, a ray is generated based on the data from the camera and the current pixel. The shape array is traversed and each shape is checked to determine if the ray intersects with a shape. If there is no intersection, the background color is printed. If one or more shape is intersected, the closest shape to the camera is determined based on the calculated t value and printed to the screen after the respective diffuse shading, ambient shading, specular shading, reflection, and shadow calculations are completed (this is done in the trace function). 

As part of the shadow calculations, a ray is generated from a shape’s intersection point to the light source to check if at least one more object is intersected on the way there, in which case the lighting and color are reduced in brightness. As part of the reflection calculations, a reflection ray is created from a point of intersection to check an object is intersected. If one or more shape is intersected by the reflection ray, the closest shape to the original point of intersection is determined and the reflected object’s color and shading information is taken into account when printing the original pixel value. 



















Requirement Check List
Parallel rays are generated and solve ray-object intersections with the objects in the scene and determine the color of each screen pixel. 
Ambient, diffuse, and specular shading are calculated when the pixel color is calculated. 
A plane is placed underneath the objects in the scene and is reflective/glazed. 
A short animation displays the rendered image after the LookAt vector is changed over a function of time.
By pressing Z, the camera can be switched from orthographic to perspective and vice versa. 
Shadows are generated by the ray-caster as well.
A report was created. 


Acknowledgement 
https://docs.microsoft.com/en-us/nuget/consume-packages/install-use-packages-powershell 
https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows
https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
Fundamentals of Computer Graphics, 5th Edition (Marschner and Shirley)
https://www.glfw.org/
http://glew.sourceforge.net/
https://github.com/g-truc/glm 
Code provided by simpleTexture.cpp to output 2D pixels in a window on the screen
CAP4730 Spring 2022 Slack #general channel discussions with classmates
Class lecture slides on Ray Tracing and Lighting
