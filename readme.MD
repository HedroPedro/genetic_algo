# Genetic Algorithm
	A genetic algorithm to find the best parameters in pipelines for single motifs discovery

# Requiriments
- Cpp compiler
- Your favorite text editor

# How to compile
- To compile the genetic algorithm simply type the command `make`. This will generate the algorithm in a file named **alg**
- To compile the random search type `make random`. This will generate the random search in a file named **alg_random**

# How to use
This program needs the following arguments (in order and is required for both algorithms)

1.  Files to treat (if have more than one use like this "[file 1] [file 2]...[file n]")
2. A shell script containing the rest of the pipeline
3. Result file
4. Optional: Directory name for macs. Defaults to "."
5. Optional: Other parameters like --broad or -g, like files to treat if have more than one argument you must pass like "[arg 1] [arg 2] ... [arg 3]". Defaults to ""

## Fitness function
There is only one fitness function at the moment using tomtoms $e$-value

The fitness is obtained via this function:
$$
f(x) = - \log_{10}(\min e\text{-value})
$$

## Usage example
``` bash
	./alg experiment.bam pipeline.sh tomtom_out/tomtom.tsv temp "
```

## Outputs
Both algorithms generate csvs following a name convention and internal structure

### Genetic Algorithm
`generations_<generations>_<population>.csv`

|Name|Description|
|----|-----------|
|Generation|Current Generation Index|
|Fitness|Best fitness found up to this generation|
|Params|MACS3 command string for the best individual|
|Budget|Unique evalutions perfomed in this generations|
|Time|Elapsed Time in seconds|

### Random Search
`random_generations_<number of interations>_<points per iteration>`

|Name|Description|
|----|-----------|
|Iter|Current Iteration Index|
|Fitness|Best fitness found up to this iteration|
|Params|MACS3 command string for the best point|
|Time|Elapsed Time in seconds|

	P.S: Random Search doesn't have a budget because it always execute it's maximum budget

# Contributing
There's two ways that you can contribute with the project
## Code
Here we have a checklist of things that can be done to help the project
- [ ] Parallel version
- [ ] Support for other tipes of files and fitness functions
- [ ] Code optimization and refactoring
- [ ] Dynamic makefile generation based on computer configurations
- [ ] Documentation

If you change how the argument are parsed or program name **please** make sure you change the appropriating sections in this document!

## Bioinformatic
- [ ] Testing on other pipelines
- [ ] Comparation with other hyperparameter methods


# License
This project uses the MIT LICENSE, check [LICENSE](LICENSE) for more information

# Citation
One day...