# Scene configuration file (WIP)

- [Scene configuration file (WIP)](#scene-configuration-file-wip)
  - [Context](#context)
  - [Structure](#structure)
  - [Objects](#objects)
  - [Definitions](#definitions)
    - [Camera](#camera)
    - [Image](#image)

## Context
In order to provide a no-code interface for building scenes, I've written a simple configuration language -- SceneConfig (.sc) -- that allows a user to define a scene and render it. This README serves as documentation for that language.

## Structure
The primary semantic device used by SceneConfig is an Object followed by a Object specific definition of its parameters. To get comfortable with this idea, let's use this model to describe a car.

```json
Car {
  make: Chevy
  model: Cruze
  year: 2014
  color: Grey
  doors: 4
  wheels: 4
  // so on an so forth
}
```

Although simple, this model is powerful enough for a user to define a scene easily.

## Objects
SceneConfig supports the following Objects:

* Camera
* Hittables
  * Sphere
* Materials
  * Lambertian (Diffuse)
  * Metal (Reflective)
  * Dielectric (Refractive)

## Definitions
As mentioned before, each Object requires a definition of it's parameters. Here we specify the required parameters for each Object.

### Camera
The camera that "takes the picture" in the scene. There can only be one Camera per SceneConfig.
```json
Camera {
  origin = (x, y, z)        // Location
  lookAt = (x, y, z)        // Where the camera looks
  rotation = angle          // Rotation around vector from origin to lookAt in degrees
  fov = angle               // Field of view in degrees
  aspectRatio = w:h         // Ratio of width to height
  aperture = float          // Radius of the camera aperture
  focusDist = auto | float  // Where to focus the camera. When "auto" is specified, the camera will focus on the lookAt point.
}
```

### Image
The image that the Camera will render to. Similarly to the camera, there can only be one Image per SceneConfig.
```json
Image {
  name = string     // Name of the output image
  width = integer   // Width of the output image
  height = integer  // Height of the output image
  samples = integer // Number of samples per pixel
  bounces = integer // Maximum bounces per ray
}
```