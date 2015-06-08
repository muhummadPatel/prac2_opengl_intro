##Practical 2 - _OpenGL: Basic Rendering and Transformations_

**Author** : Muhummad Yunus Patel  
**Student#** : PTLMUH006  
**Date** : 27-April-2015  

**NOTE:** Should you require any clarification, please feel free to contact me 
 at muhummad.patel@gmail.com. Thank you. :)

###Description:
This practical required us to use Qt and its OpenGL bindings to write a simple
program to load and render binary stl files. A sample skeleton project was 
provided. The skeleton code handled the basic window setup, etc. A seperate
stlModel class was written to read in a binary stl file and store the vertices
as a float array. the stlmodel class was used in the GLWidget class provided in
the skeleton to read in the provided stl file \(bunny.stl\). The options required
for the practical \(translate, rotate, scale, colour change, etc\) were also 
implemented in the glWidget class. The GLM library was used for matrix  and 
vector operations.

###Compiling and running the solution:

* Load the project into QtCreator.

* Build the project in QtCreator.

* Run the project in QtCreator. \(**NOTE**: by default, there will be no model 
 rendered. You must choose an stl file to load. see below.\)  
 \(**ALSO NOTE**: please ensure the the glm folder is in the working directory\(by
 default this is the debug folder\)\)

* Loading a model:
    * when the program starts, there will be no model.
    * open the File menu in the menu bar on the top of the screen.
    * click on Open.
    * Select the stl file to be loaded in the fileDialog that pops up.
    * The model should then be displayed in the window.  

* Translating the model:
    * press t to enter translate mode.
    * scroll up or down with the mousewheel to translate the model in the current
     active axis.
    * press t again to toggle between the axes.  

* Rotating the model:
    * press r to enter rotate mode.
    * scroll up or down with the mousewheel to rotate the model in the current
     active axis.
    * press r again to toggle between the axes.  

* Scaling the model:
    * press s to enter scale mode.
    * scroll up or down with the mousewheel to scale the model in the current
     active axis.
    * press s again to toggle between the axes.  

* Changing the colour of the model:
    * press either 1, 2, 3, 4, or 5.
    * this will switch between 5 preset colours.
    * the model will then be rendered in the colour preset that you selected.  

* Resetting the Scene:
    * open the File menu from the menu bar at the top of the screen.
    * click on new/reset.
    * this will reload the current model and reset the scene (undoes any changes 
     you made to the scene).