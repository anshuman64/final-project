# Raytracer

Raytracing renderer based on UCSD CS167 Fall 2021 project spec by Professor Albert Chern. Features Monte Carlo path tracing, shadows, reflections, and global illumination.

## Examples
#### Submission Scene 6 
1. Raytracing with shadows & mirror reflections
2. No shadows or mirror reflections
3. Shadows but no mirror reflections

<img src="https://github.com/anshuman64/raytracer/blob/master/output/scene6.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/output/scene6-no-shadows.png" width="300" /><img src="https://github.com/anshuman64/raytracer/blob/master/output/scene6-no-mirror.png" width="300" />

#### Submission Scene 4
1. Ambient / Emission color only
2. Diffuse color only
3. Specular color only

<img src="https://github.com/anshuman64/raytracer/blob/master/output/scene4-ambient.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/output/scene4-diffuse.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/output/scene4-specular.png" width="300" />

#### Path Tracer - Scene 6
1. num_samples = 1, bounce_probability = 0.5
2. num_samples = 10, bounce_probability = 0.5

<img src="https://github.com/anshuman64/raytracer/blob/master/output/scene6-k1-l05.png" width="300" /> <img src="https://github.com/anshuman64/raytracer/blob/master/output/scene6-k10-l05.png" width="300" />

## Usage
After cloning the repo, run the Raytracer executable as follows:

```./Raytracer [path_to_test_file] [no_shadows] [no_mirror] [no_gi] [num_samples] [bounce_probability]```

#### Command Line Args:
- ```path_to_test_file```: path to .test input file. See ```./testscenes``` and ```./submissionscenes``` for examples. 
- ```no_shadows```: turn off shadows
- ```no_mirror```: turn off mirror reflections (with ```no_gi``` only)
- ```no_gi```: turn off global illumination using Monte Carlo path tracing
- ```num_samples``` (ie. *K*): number of ray samples per pixel for Monte Carlo path tracing. Default = 1.
- ```bounce_probability``` (ie. *lambda*): probability each ray will bounce for Monte Carlo path tracing. Default = 0.5.

**Note**: Program takes long time to run on laptop CPUs.