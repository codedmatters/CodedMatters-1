*Note*

This application was built using openFrameworks, v0073. All the used addons are included in the project as submodules.
When the application exports a frame, it actually splits up the frame and writes those images to disk to prevent a huge memory usage when exporting high resolution frames. To stitch these images back together into a single frame, use the script in /bin/data/scripts/extractScreenshots. This scripts needs the imagemagick 'convert' program to be installed and in your PATH.

***CODED MATTER(S)***

Visual Identity Software
Manual 

***Directory structure***

The application has the following directory structure.

_ Coded Matter(s) – Main directory
	_ Coded Matter(s).app – The application itself
	_ data – Directory containing all data
		_ exported
		_ font
		_ framedata
		_ scripts
		_ shaders
		_ \_gameCameraPosition.xml
		_ settings.xml

**exported**
This directory contains all  the data that is exported from the
application. See ‘framedata’ and ‘scripts’ for more information on how
to export data.

**font**
The font used for the logo is located in this directory

**framedata**
When you record a movie or take a screenshot from the application, the
data will first be put in this directory. The exporting is not finished
yet, because all images are cut up into 25 pieces when exporting. You
have to run a script to convert the images into a single image per frame
/ screenshot.

**scripts**
The script for converting all the exported data into frames /
screenshots is located in this directory. After exporting from the
application, double click the script in this directory. All the
resulting files will be moved to the ‘exported’ directory.
**_gameCameraPosition.xml**
This file saves the current position and orientation of the camera. This
is updated when you click the ‘save settings’ button in the application.
If you want to reset the camera to the default state the next time you
run the application, delete this file.

**settings.xml**
This file saves all the general settings in the application. It is
updated when you click the ‘save settings’ button in the application.
You can change values here instead of in the application menu if you
like. If you delete some or all values, a default value will be used.
***Settings***

I will explain every parameter you can find in the application menu. All
the same paramaters  (except for triggers) can be found in the
settings.xml file.

There is a single keyboard shortcut in the application. Use the space
bar to export a screenshot. Also, ESC closes the application.

**Save settings**– save all the settings and camera position to the XML
files

**Enable camera**– If this value is ‘on’, you can change the camera
position. Drag around with the mouse to look around you. Use the
following keys for movement:
w – forward
s – backward
a – left
d – right
e – up
c – down
q – roll left
r – roll right

**Base color** **rotation** determines the rotation angle for the base
color gradient.

**Base color** and **base color 2**determine the overall color of the
shape. The color is interpolated from left to right.

**Background color** **rotation** determines the rotation angle for the
background color gradient.

**Background color top ** and **background color bottom** determine the
color of the background gradient.

**Export width** and **export height** set the resolution of the
exported frames. You can select any of the three presets **A2 320dpi**,
**A5 350dpi** or **HD video**. You can also choose a custom resolution.
**Recording**
**Enable recording** - when this value is set to ‘on’, every frame is
exported to disk. When it is turned off again, recording stops. Note:
the framerate will be much lower when recording, especially using high
export resolutions.
**Cam speed** – The X, Y and Z cam speed values determine the movement
of the camera without using the keyboard and mouse.

**Wave in duration** and **wave out duration** determine how long the
‘wave in’ and ‘wave out’ animations will take. This value is a number of
frames.

**Wave in** and **wave out** set off the wave animations. Only one
animation can be in progress at the same time.

**Particles**
**Num rows** and **num cols** determine the amount of particles.
Whenever one of these values is changed, any offset on the x axis is
reset.

**Row spacing** and **column spacing** set the spacing between rows and
columns.

**Direction amount** determines the influence of the direction of the
waves on the rotation of the particles.

**Use cone** – draw a cone instead of a small block

**Use second cone**– draw a second cone opposed to the first one

**Use sphere in cone** – draw a sphere in the base of every cone. Will
decrease performance

**Draw direction** – draw a thin line in the direction of the particle

**Particle length**, **particle height** and **particle width**
determine the size of every particle.

**Particle speed** – The amount of movement over the x axis by every
particle. Set this to a negative value to move to the left.

**Particle rotation** – Rotate the particles with a certain speed.

**Noise**
**Noise amount** determines how much influence the noise field has on
the height of the particles.

**Noise X**, **noise Y**and **noise Z**determine the coordinates of the
noise. Higher value means bigger waves.

**Noise speed X**, **noise speed Y**and **noise speed Z**determine the
amount of movement in the noise.

The parameters for **Scale noise** and **Color noise** work the same.
**Scale noise** influences the scale of particles and **Color noise**
creates color highlights in the particle field.

**SSAO**
**Enable SSAO**– Enables the use of Ambient Occlusion. This effect is
not rendered in the preview window for performance reasons. It adds some
shadowing to the shape.

**SSAO radius –**The radius of the shadows generated by SSAO.
**SSAO min threshold** and **SSAO max threshold** determine what
darkness a shadow should have before it is shown.

**SSAO exponent** – The shadow falloff exponent. 

**SSAO num samples** – The number of samples used in the shadowing
process.

**Shading**
**Enable shading**– Enables the use of a phong shader. This reflects a
virtual light from the particles.

**Ambient amount**determines the amount of ambient light used in the
phong shading.

**Diffuse amount** determines the amount of diffuse light used in the
phong shading.

**Light X**, **light Y** and **light Z** determine the position of the
virtual light.

**Filter**
**Enable filter**– Enables the use of a vignette filter.

**Vignette size**determines the size of the vignette.

**Vignette amount**determines how much effect the vignette has. A
negative value means that the vignette darkens, a positive value
lightens.

**Export settings**
**Enable AA (export only)**– Enables antialiasing. This effect is not
shown in the preview window for performance reasons.

**Enable depth map (export only)**– Enables the exporting of a depth
map. This depth map can be used for depth of field effects in post
processing.

**Show depth map** – Show the depth map in the preview window.

**Depth near value** and **depth far value** determine the scaling of
the depth map.

**Logo**
**Show logo**– Enables the rendering of the logo on top of the shape.

**Font size** determines the font size used for the logo.

**Logo color** determines the color of the logo.

**Logo position X**, **logo position Y** and **logo position Z**
determine the position of the logo. The logo is automatically centered
after changing the font size.
