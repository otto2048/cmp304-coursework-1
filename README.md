# CMP304 Coursework 1

Project completed for CMP304 - Artificial Intelligence

## Project Overview

<p float="left">
  <img src="https://github.com/otto2048/cmp304-coursework-1/blob/main/images/12x12-2Level.png" alt="Example flow level" width="350" />
  <img src="https://github.com/otto2048/cmp304-coursework-1/blob/main/images/12x12-2Solution.png" alt="Example flow level" width="350" />
</p>

<p>
  This project involved the creation of a Genetic Algorithm that could complete a level of the game Flow, an example of of a level can be seen above.
</p>

<p>
  Playing a level of Flow requires the player to find the paths between all pairs of points given in the level, and these paths are referred to as “Flow”s. For a perfect solution, the entire grid space must be used up by these paths. The Genetic Algorithm in this project always aims for a perfect solution.
</p>

## Data Representation

Each member of the generation is represented by the Chromosome class. An instance of the Chromosome class contains all the information about a solution to a certain level. This class contains a vector of Genes, each Gene holds the information about one path (or Flow) in the solution.

## Pathfinding

Flow pathfinding is completed using a modified version of the A* algorithm.

The A* algorithm functions as normal, finding the shortest path, but the cost between points in the grid is randomized. From the outside, this makes it seem that the algorithm is making random decisions on which direction to take, but to the algorithm, it is still finding the shortest path and avoiding high-cost routes. This approach allows for efficient, but random, pathfinding.

## Algorithm Stages

### Step 0. Generating the initial population

- Generate Chromosomes to generate initial population
- Simulate the inital population (generate random path for each Gene in each Chromosome)
- Calculate the cost of each Chromosome

### Step 1. Population Ranking

- Sort the generation by each Chromosomes cost and assign each Chromosome a ranking

### Step 2. Crossover and Mutation

- Use Uniform Crossover to create a new generation, using Rank Selection to choose parents of children and, for each Gene, create a new Gene from the paths provided by the parents
- Mutate a random number of Genes in some Chromosomes by regenerating their paths

### Step 3. Path Rearrangement

- Rearrange the paths of each Gene as children created in crossover might be invalid solutions to the problem (eg. paths overlap). Path rearrangement resets overlapping paths and marks them as unsuccessful

### Step 4. Simulation

- Generate paths for each Gene in each Chromosome, where the path is marked unsucessful

### Step 5. Calculate generation member cost values

- The cost of a Chromosome is determined by path quality, the amount of space taken up by the solution, and how well any failed paths did
  - Space taken up by the solution must be considered as the best solution must use all the paths to take up all empty space
  - Path quality is determined by the number of paths that failed, if the path used zigzagging to take up more space, and if paths blocked each other
- The sigmoid function is applied to the three cost factors to ensure that each factor is evenly weighted

#### Repeat stages 1-5 until a stopping condition is met

Possible stopping conditions:
- All solutions converge on an identical solution
- All solutions converge on the best solution
- Maximum number of generations is met
