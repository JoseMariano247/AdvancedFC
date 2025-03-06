# Import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt

# Read the data from output.txt
# Assuming the file is tab-delimited with a header like "Time    Concentration"
data = pd.read_csv('Concentrations.txt', delimiter='\t')
data_1 = pd.read_csv("Concentrations_Runge.txt", delimiter='\t')

# Display the first few rows to verify correct loading
print(data.head())

# Extract x and y values from the DataFrame
time = data.iloc[:, 0]  # first column: Time
A = data.iloc[:, 1]  # second column: Concentration A
B = data.iloc[:, 2]

# Extract x and y values from the DataFrame
time_1 = data_1.iloc[:, 0]  # first column: Time
A_1 = data_1.iloc[:, 1]  # second column: Concentration A
B_1 = data_1.iloc[:, 2]
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
