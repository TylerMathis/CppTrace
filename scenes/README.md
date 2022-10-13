# Scene configuration file (WIP)

- [Scene configuration file (WIP)](#scene-configuration-file-wip)
- [Context](#context)
- [Structure](#structure)
- [Objects and their definitions](#objects-and-their-definitions)
  - [Meta-Objects](#meta-objects)
    - [Camera](#camera)
    - [Image](#image)
  - [Materials](#materials)
    - [Lambertian](#lambertian)
    - [Metal](#metal)
    - [Dielectric](#dielectric)
  - [Physical-Objects](#physical-objects)
    - [Sphere](#sphere)

# Context
In order to provide a no-code interface for building scenes, I've written a simple configuration language -- SceneConfig (.sc) -- that allows a user to define a scene and render it. This README serves as documentation for that language.

# Structure
The primary semantic device used by SceneConfig is an object followed by a object specific definition of its parameters. To get comfortable with this idea, let's use this model to describe a car.

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

# Objects and their definitions
As mentioned before, each object requires a definition of its parameters. Here we enumerate each object and specify its required parameters.

## Meta-Objects
### Camera
The camera that "takes the picture" in the scene. There can only be one Camera per SceneConfig.
```json
Camera {
  origin      = (float, float, float) // Location
  lookAt      = (float, float, float) // Where the camera looks
  rotation    = float                 // Rotation around vector from origin to lookAt in degrees
  fov         = float                 // Field of view in degrees
  aperture    = float                 // Radius of the camera aperture
  focusDist   = auto | float          // How far to focus the camera. When "auto" is specified, the camera will focus on the lookAt point.
}
```

### Image
The image that the Camera will render to. Similarly to the Camera, there can only be one Image per SceneConfig.
```json
Image {
  name    = string  // Name of the output image
  width   = integer // Width of the output image
  height  = integer // Height of the output image
  samples = integer // Number of samples per pixel
  bounces = integer // Maximum bounces per ray
}
```

## Materials
Materials define how physical objects interact with light.

### Lambertian
Lambertian materials reflect light randomly in all directions. Each bounce attenuates the light by the material's `albedo`.
```json
Lambertian {
  albedo = (float, float, float) // Light attenutation on contact, between 0 and 1 for each component
}
```

### Metal
Metal materials reflect light perfectly about the surface normals. Each bounce attenuates the light by the material's `albedo`.
```json
Metal {
  albedo = (float, float, float) // Light attenutation on contact, between 0 and 1 for each component 
}
```

### Dielectric
Dielectric materials refract light according to approximated refraction laws. They do not attenuate light.
```json
Dielectric {
  refractiveIndex = float // Refractive index of the dielectric
}
```
## Physical-Objects
Physical objects are the only objects that light can bounce off of. They are made up of a physical definition and a material.

### Sphere
A sphere is... a sphere.
```json
Sphere {
  center    = (float, float, float) // Center point of the spher
  radius    = float                 // Radius of the sphere
  material  = Material              // Material of the sphere
}
```
*Example sphere*
```json
Sphere {
  center = (1.2, 0.0, 3.5)
  radius = 0.5
  material = Lambertian {
    albedo = (0.7, 0.4, 0.6)
  }
}
```