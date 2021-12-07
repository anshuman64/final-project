# Raytracer

Raytracing graphics renderer based on UC San Diego CS167 Fall 2021 Computer Graphics project spec by Professor Albert Chern. Features Monte Carlo path tracing, shadows, reflections, and global illumination. For MacOS.

## Examples
#### Raytracer - Scene 6 
1. Raytracing with shadows & mirror reflections
2. No shadows or mirror reflections
3. Shadows but no mirror reflections

<img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene6.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene6-no-shadows.png" width="300" /><img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene6-no-mirror.png" width="300" />

#### Raytracer - Scene 4
1. Ambient / Emission color only
2. Diffuse color only
3. Specular color only

<img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene4-ambient.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene4-diffuse.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene4-specular.png" width="300" />

#### Path Tracer - Scene 6
1. num_samples = 1, terminate_probability = 0.01
2. num_samples = 10, terminate_probability = 0.5
3. num_samples = 100, terminate_probability = 0.33

<img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene6-k1-l99.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene6-k10-l50.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/saved_output/scene6-k100-l33.png" width="300" />

## Usage
After cloning the repo, run the Raytracer executable as follows:

```./Raytracer [path_to_test_file] [no_shadows] [no_mirror] [no_gi] [num_samples] [terminate_probability]```

#### Command Line Args:
- ```path_to_test_file```: path to .test input file. See ```./testscenes``` and ```./submissionscenes``` for examples. 
- ```no_shadows```: turn off shadows
- ```no_mirror```: turn off mirror reflections (with ```no_gi``` only)
- ```no_gi```: turn off global illumination using Monte Carlo path tracing
- ```num_samples``` (ie. *K*): number of ray samples per pixel for Monte Carlo path tracing. Default = 1.
- ```terminate_probability``` (ie. *lambda*): probability each ray will NOT bounce for Monte Carlo path tracing. Default = 0.5.

**Note**: Program takes long time to run on laptop CPUs.