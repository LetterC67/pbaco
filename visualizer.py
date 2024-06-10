import matplotlib.pyplot as plt
import numpy as np

# Function to generate random data points with a decreasing trend
def generate_decreasing_data(x_values, start_y, end_y):
    y_values = []
    current_y = start_y
    step = (start_y - end_y) / len(x_values)
    
    for x in x_values:
        current_y -= np.random.uniform(0, step)
        y_values.append(current_y)
    
    return y_values

# Define the x values
x_values = list(range(10, 1010, 10))

# Define start and end y values for multiple datasets
def read(file_path):
    numbers = []
    print(file_path)
    with open(file_path, 'r') as file:
        # Read each line in the file
        for line in file:
            # Strip any leading/trailing whitespace (including newlines)
            line = line.strip().split()[1]
            numbers.append(float(line))

    print(numbers)
    return numbers


# Create the plot
plt.figure(figsize=(10, 6))
datasets = [ 'convergence/pcb1173_10_original', 'convergence/pcb1173_10_']

# Generate and plot each dataset
for dataset in datasets:
    y_values = read(dataset)
    x_values = []
    for i in range(len(y_values)):
        x_values.append(i)
    plt.plot(x_values, y_values, marker=',', linestyle='-', label=dataset)

# Add title and labels
plt.title('Random Decreasing Data Points for Multiple Datasets')
plt.xlabel('X values')
plt.ylabel('Y values')

# Add legend
plt.legend()

# Show grid
plt.grid(True)

# Show the plot
plt.show()