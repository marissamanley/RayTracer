# Ray Tracer Report
## Marissa Manley - CAP4730 Computer Graphics Spring 2022

### Compiling Guide for Visual Studio 2019:

1. Install GLFW, GLEW, and GLM to a location on your computer. (https://www.glfw.org/download, http://glew.sourceforge.net/, https://sourceforge.net/projects/glm.mirror/)
2. Download and add RayTracer.cpp to an empty Visual Studio 2019 project.
3. Enter Project Properties -> C/C++ -> General; inside of the Additional Include Directories, add the directories at which your GLFW, GLEW, and GLM library files are located and link to the equivalent /include files. 
4. Enter Project Properties -> Linker -> General; inside of the Additional Library Directories, add the directories at which your GLFW, GLEW, and GLM library files are and link to the equivalent /lib files. 
5. Enter Project Properties -> Linker -> Input; inside of the Additional Dependencies, add the dependencies opengl32.lib, glfw3.lib, and glew32s.lib.
6. Add the glew32.dll and glfw3.dll files into the directory with your RayTracer file.
7. Within the Visual Studio 2019 Package Manager, feel free to run the following commands to include the appropriate GLFW and GLEW packages:
	Install-Package glfw
	Install-Package glew
7. You can now either run the Ray Tracer by compiling and running the code in Visual Studio 2019 using the Local Windows Debugger or by entering the command prompt, entering the Directory with your RayTracer file with the cd command, and running the commands:
	g++ -lglew -lglfw RayTracer.cpp
	./a.out

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

![Assignment1_kYDtFttlNR](https://user-images.githubusercontent.com/58527286/187339766-756d16a7-ab7a-4ff9-a199-4cccfbf6d388.png)

Perspective camera:


![Assignment1_r9QkWqJsMS](https://user-images.githubusercontent.com/58527286/187339978-edab103e-b805-4410-a186-21fe84418bdd.png)


With/without directional light:

![chrome_U0klvXdqoX](https://user-images.githubusercontent.com/58527286/187341257-aaed5af2-5136-4faa-995d-e6a47d2473f8.png)
![chrome_AQionIVAYd](https://user-images.githubusercontent.com/58527286/187340288-43fba667-c97f-4e19-a5ec-8673461a2375.png)

High/Low ambient shading:

![chrome_U0klvXdqoX](https://user-images.githubusercontent.com/58527286/187341283-737eff6f-4811-4744-9018-a4887b4c8068.png)
![chrome_EnOab9MJzj](https://user-images.githubusercontent.com/58527286/187340946-886fe7f5-052d-41ba-ae5b-eba1568656f0.png)

With/without shadows:

![chrome_U0klvXdqoX](https://user-images.githubusercontent.com/58527286/187341515-39a37b58-a666-48ae-8941-eb890aa2b689.png)
![chrome_R67cwkSh9a](https://user-images.githubusercontent.com/58527286/187341523-e443d5af-a86d-42c4-98f6-52589bd39747.png)


With/without diffuse shading:

![chrome_U0klvXdqoX](https://user-images.githubusercontent.com/58527286/187341847-e456a41b-8ec7-4241-b840-991b25c2206f.png)
![chrome_RQFaQhTtF9](https://user-images.githubusercontent.com/58527286/187341855-bf1ae7ba-830f-49a2-9a20-4f1c33435c94.png)

With/without specular shading and relfections:

![chrome_U0klvXdqoX](https://user-images.githubusercontent.com/58527286/187342110-1fb59b6f-625b-4221-aba4-40cc3dbb63ca.png)
![chrome_wnaLgX15ey](https://user-images.githubusercontent.com/58527286/187342120-f06de8fa-bd79-401b-81a1-b6220e0870af.png)


### Program Logic: 
Aside from the content given in the provided script at the beginning of this project, the code consists of various classes, a trace function, a RayTrace function, and a main function. I create a class for a Camera, Light Source, Ray, and Shape, and there are three children classes of Shape: Sphere, Tetrahedron, and Plane.  

All class objects are created in a RayTrace function, which is called in main. An array of shape objects is created and all scene objects are added to the array. Within the for loop that generates the RGB values for each pixel in the screen, a ray is generated based on the data from the camera and the current pixel. The shape array is traversed and each shape is checked to determine if the ray intersects with a shape. If there is no intersection, the background color is printed. If one or more shape is intersected, the closest shape to the camera is determined based on the calculated t value and printed to the screen after the respective diffuse shading, ambient shading, specular shading, reflection, and shadow calculations are completed (this is done in the trace function). 

As part of the shadow calculations, a ray is generated from a shape’s intersection point to the light source to check if at least one more object is intersected on the way there, in which case the lighting and color are reduced in brightness. As part of the reflection calculations, a reflection ray is created from a point of intersection to check an object is intersected. If one or more shape is intersected by the reflection ray, the closest shape to the original point of intersection is determined and the reflected object’s color and shading information is taken into account when printing the original pixel value. 

 


### Acknowledgement 
1. https://docs.microsoft.com/en-us/nuget/consume-packages/install-use-packages-powershell 
2. https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
3. https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows
4. https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
5. Fundamentals of Computer Graphics, 5th Edition (Marschner and Shirley)
6. https://www.glfw.org/
7. http://glew.sourceforge.net/
8. https://github.com/g-truc/glm 
9. Code provided by simpleTexture.cpp to output 2D pixels in a window on the screen
10. CAP4730 Spring 2022 Slack #general channel discussions with classmates
11. Class lecture slides on Ray Tracing and Lighting
