# 2DPartInt

Geotechnical problems have been traditionally tackled as an extension of continuum mechanics theories. Such an approach, although appropriate for systems undergoing small displacements, cannot examine important problems such as landslides, rockfalls, and rainfall-induced landslides. Our implementation is a first step toward analyzing these complex behavior, and, as a starting point, studies systems constituted of geomaterials assumed as particles in a two-dimensional space. With this implementation, we plan to bring to the geotechnical community, academic and practitioners in general, a computational tool that will allow solving challenges involving multiphysics and large displacements effectively and accurately.

## Usage

Compile the project using make.

```bash
$ make
```

_(Optional)_ run the tests to ensure everything is ok.


```bash
$ make test
```

To debug some simulation step compile like this.

```bash
$ make DEBUG_STEP=true
```

Execute the program passing the `simulation_config.txt` file & the output folder as the arguments.

```bash
$ ./bin/2DPartInt simulation_config.txt out/
```

## Simulation Config File.

The behaviour of the simulation is determined by the config file.

This is the structure of the file.
_(All settings are mandatory, but they can be in any order)_

```
time=[Int] # Total time to simulate, in seconds.
dt=[Double] # Size of the time window used for each step.
x_limit=[Int] # Simulation size in Y.
y_limit=[Int] # Simulation size in X.
radius=[Int] # Radius of each particle.
kn=[Double] # Normal rigidity of the material.
ks=[Double] # Tangential rigidity of the material.
rho=[Double] # Density of the material.
thickness=[Double] # Thickness of the particles.
v0=[Double] # Initial velocity of the falling particle.
r0=[Double] # Radius of the falling particle.
```
