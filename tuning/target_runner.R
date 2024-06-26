#!/usr/bin/env Rscript
args <- commandArgs(trailingOnly = TRUE)

dataset <- as.character(args[5]) 
salesmen <- as.integer(args[7])
beta <- as.numeric(args[9])
rho <- as.numeric(args[11])
pop <- as.numeric(args[13])
pop_size <- as.integer(args[15])
ratio <- as.numeric(args[17])

# Construct the command to run your C++ executable with the dataset
command <- sprintf("./../a.out --dataset=%s --salesmen=%d --beta=%f --rho=%f --pop=%f --pop_size=%d ratio=%f --thread=32 --run=20", dataset, salesmen, beta, rho, pop, pop_size, ratio)

# Run the command and capture the output
result <- system(command, intern = TRUE)

# Extract the performance measure from the output (assuming it prints the result on the last line)
performance <- as.numeric(tail(result, n = 1))

# Return the performance measure to irace
cat(performance, "\n")
