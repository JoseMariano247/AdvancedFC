# Import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt

# Read the data from output.txt
data = pd.read_csv('Concentrations.txt', delimiter='\t')
data_1 = pd.read_csv("Concentrations_Runge.txt", delimiter='\t')
data_2 = pd.read_csv("Concentrations_Runge_Ch.txt", delimiter='\t')
data_3 = pd.read_csv("Concentrations_Monte_Carlo_Ch.txt", delimiter='\t')
data_4 = pd.read_csv("Concentrations_Runge_Ph.txt", delimiter='\t')
data_5 = pd.read_csv("Concentrations_MC_Ph.txt", delimiter='\t')
data_6 = pd.read_csv("Recombination_RK.txt", delimiter='\t')
data_7 = pd.read_csv("Real_Test.txt", delimiter='\t')

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

# Extract x and y values from the DataFrame
time_4 = data_4.iloc[:, 0]  # first column: Time
A_4 = data_4.iloc[:, 1]  # second column: Concentration A
Fv = data_4.iloc[:, 2]
Af = data_4.iloc[:, 3]

# Extract x and y values from the DataFrame
time_5 = data_5.iloc[:, 0]  # first column: Time
A_5 = data_5.iloc[:, 1]  # second column: Concentration A
Fv_1 = data_5.iloc[:, 2]
Af_1 = data_5.iloc[:, 3]

time_6 = data_6.iloc[:, 0]
A_r    = data_6.iloc[:, 1]
Fv_r   = data_6.iloc[:, 2]
Af_r   = data_6.iloc[:, 3]       
Sv_r   = data_6.iloc[:, 4]   
As_r   = data_6.iloc[:, 5]      
A2_r   = data_6.iloc[:, 6]

time_7 = data_7.iloc[:, 0]
A_r_1    = data_7.iloc[:, 1]
Fv_r_1   = data_7.iloc[:, 2]
Af_r_1   = data_7.iloc[:, 3]       
Sv_r_1   = data_7.iloc[:, 4]   
As_r_1   = data_7.iloc[:, 5]      
A2_r_1   = data_7.iloc[:, 6]

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

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(time_4, A_4, linestyle='-', color='b', label='Concentration of A')
plt.plot(time_4, Fv, linestyle='-', color='r', label='Concentration of Fv')
plt.plot(time_4, Af, linestyle='-', color='orange', label='Concentration of Af')
plt.plot(time_5, A_5, linestyle='--', color='orange', label='Concentration of A for MC')
plt.plot(time_5, Fv_1, linestyle='--', color='b', label='Concentration of Fv for MC')
plt.plot(time_5, Af_1, linestyle='--', color='r', label='Concentration of Af for MC')

plt.xlabel('Time')
plt.ylabel('Concentration')
plt.title('Concentration vs Time')
plt.legend()
plt.grid(True)
plt.show()


plt.figure(figsize=(8, 6))
#plt.plot(time_6, A_r, linestyle='-', color='b', label='Concentration of A')
#plt.plot(time_6, Fv_r, linestyle='-', color='r', label='Concentration of Fv')
#plt.plot(time_6, Af_r, linestyle='-', color='orange', label='Concentration of Af')
#plt.plot(time_6, Sv_r, linestyle='-', color='yellow', label='Concentration of Sv')
#plt.plot(time_6, As_r, linestyle='-', color='black', label='Concentration of As')
#plt.plot(time_6, A2_r, linestyle='-', color='pink', label='Concentration of A2')
plt.plot(time_7, A_r_1, linestyle='--', color='pink', label='Concentration of A of MC')
plt.plot(time_7, Fv_r_1, linestyle='--', color='b', label='Concentration of Fv of MC')
plt.plot(time_7, Af_r_1, linestyle='--', color='yellow', label='Concentration of Af of MC')
plt.plot(time_7, Sv_r_1, linestyle='--', color='black', label='Concentration of Sv of MC')
plt.plot(time_7, As_r_1, linestyle='--', color='orange', label='Concentration of As of MC')
plt.plot(time_7, A2_r_1, linestyle='--', color='r', label='Concentration of A2 of MC')

plt.xlabel('Time')
plt.ylabel('Concentration')
plt.xscale('log')
#plt.yscale('log')
plt.title('Concentration vs Time')
plt.legend()
plt.grid(True)
plt.show()