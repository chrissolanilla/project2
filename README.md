# Machine Problem 2 Branch Predictor

## building
`make` will build the project. The executable will be in the `bin` directory. The object files will be in the `build` directory.

You can run `make clean` to remove the `bin` and `build` directory

## running
you can run the `sim` binary in the `bin` directory. Running it without arguments will print help information.

## verifiyng results
You can run the `check_validations.sh` script to check the output of the program against the validation tests.

## printing graphs
You can run the `run_experiments.sh` script to save useful information into a csv into the `experiment_results` directory.

Then you can run the python script `plot_graphs.py` to plot the graphs.

For running the python script you need to have matplotlib installed. Try creating a virtual environment and install it pip.

The graphs will be saved in the `experiment_results` directory.
