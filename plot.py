import matplotlib.pyplot as plt
import sys
sys.stdin = open("temp", "r")

def read_city_coordinates(filename):
    """
    Read city coordinates from a file.

    Args:
    - filename: Name of the file containing city coordinates

    Returns:
    - A dictionary where keys are city names and values are tuples (x, y) representing coordinates
    """
    city_coordinates = {}

    with open(filename, 'r') as file:
        for line in file:
            data = line.strip().split()
            if not data[0].isnumeric():
                continue
            if(int(data[0]) < 10):
                print(data[0])
            city_name = int(data[0]) - 1
            x = float(data[1])
            y = float(data[2])
            city_coordinates[city_name] = (x, y)
    print(city_coordinates[0][0])
    return city_coordinates

def plot_salesmen_tours(city_coordinates, salesmen_tours):
    """
    Plot tours of multiple salesmen using different colors.

    Args:
    - city_coordinates: A dictionary containing city coordinates
    - salesmen_tours: A list of lists where each inner list represents the tour of a salesman
    """
    colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k']  # List of colors for different salesmen

    for i, tour in enumerate(salesmen_tours):
        x = [city_coordinates[int(city)][0] for city in tour]
        y = [city_coordinates[int(city)][1] for city in tour]
        plt.plot(x + [x[0]], y + [y[0]], linestyle='-', marker='o', color=colors[i % len(colors)], label=f'Salesman {i+1}')
        print(len(x))
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('MTSP: Tours of Salesmen')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    # Read city coordinates from file
    filename = input("Enter the filename containing city coordinates: ")
    city_coordinates = read_city_coordinates(filename)

    # Take input for tours from several salesmen
    salesmen_tours = []
    num_salesmen = int(input("Enter the number of salesmen: "))
    for i in range(num_salesmen):
        tour = input(f"Enter tour for salesman {i+1} (space-separated): ").strip().split(' ')
        salesmen_tours.append(tour)

    # Plot the tours of multiple salesmen
    plot_salesmen_tours(city_coordinates, salesmen_tours)
