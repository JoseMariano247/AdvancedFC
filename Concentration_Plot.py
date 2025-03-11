# Import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt

# Read the data from output.txt
data = pd.read_csv('Concentrations.txt', delimiter='\t')
data_1 = pd.read_csv("Concentrations_Runge.txt", delimiter='\t')
data_2 = pd.read_csv("Concentrations_Runge_Ch.txt", delimiter='\t')
data_3 = pd.read_csv("Concentrations_Monte_Carlo_Ch.txt", delimiter='\t')

# Extract x and y values from the DataFrame
time = data.iloc[:, 0]  # first column: Time
A = data.iloc[:, 1]  # second column: Concentration A
B = data.iloc[:, 2]

# Extract x and y values from the DataFrame
time_1 = data_1.iloc[:, 0]  # first column: Time
A_1 = data_1.iloc[:, 1]  # second column: Concentration A
B_1 = data_1.iloc[:, 2]


# Extract x and y values from the DataFrame
time_2 = data_2.iloc[:, 0]  # first column: Time
A_2 = data_2.iloc[:, 1]  # second column: Concentration A
Sv = data_2.iloc[:, 2]
As = data_2.iloc[:, 3]
A2 = data_2.iloc[:, 4]

print(data_3)

# Extract x and y values from the DataFrame
time_3 = data_3.iloc[:, 0]  # first column: Time
A_3 = data_3.iloc[:, 1]  # second column: Concentration A
Sv_1 = data_3.iloc[:, 2]
As_1 = data_3.iloc[:, 3]
A2_1 = data_3.iloc[:, 4]

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(time, A, linestyle='-', color='b', label='Concentration of A')
plt.plot(time, B, linestyle='-', color='r', label='Concentration of B')
plt.plot(time_1, A_1, linestyle='--', color='black', label='Concentration of A Runge')
plt.plot(time_1, B_1, linestyle='--', color='yellow', label='Concentration of B Runge')

plt.xlabel('Time')
plt.ylabel('Concentration')
plt.title('Concentration vs Time')
plt.legend()
plt.grid(True)
plt.show()

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(time_2, A_2, linestyle='-', color='b', label='Concentration of A')
plt.plot(time_2, Sv, linestyle='-', color='r', label='Concentration of Sv')
plt.plot(time_2, As, linestyle='-', color='orange', label='Concentration of As')
plt.plot(time_2, A2, linestyle='-', color='green', label='Concentration of A2')
plt.plot(time_3, A_3, linestyle='--', color='b', label='Concentration of A of MC')
plt.plot(time_3, Sv_1, linestyle='--', color='r', label='Concentration of Sv of MC')
plt.plot(time_3, As_1, linestyle='--', color='orange', label='Concentration of As of MC')
plt.plot(time_3, A2_1, linestyle='--', color='green', label='Concentration of A2 of MC')

plt.xlabel('Time')
plt.ylabel('Concentration')
plt.title('Concentration vs Time')
plt.legend()
plt.grid(True)
plt.show()


