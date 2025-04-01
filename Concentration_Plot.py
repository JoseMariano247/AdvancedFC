# Import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the data from output.txt
data = pd.read_csv('Concentrations.txt', delimiter='\t')
data_1 = pd.read_csv("Concentrations_Runge.txt", delimiter='\t')
data_2 = pd.read_csv("Concentrations_Runge_Ch.txt", delimiter='\t')
data_3 = pd.read_csv("Concentrations_Monte_Carlo_Ch.txt", delimiter='\t')
data_4 = pd.read_csv("Concentrations_Runge_Ph.txt", delimiter='\t')
data_5 = pd.read_csv("Concentrations_MC_Ph.txt", delimiter='\t')
data_6 = pd.read_csv("Recombination_RK.txt", delimiter='\t')
data_7 = pd.read_csv("Recombination_MC.txt", delimiter='\t')
data_8 = pd.read_csv("Real_Test.txt", delimiter='\t')
data_9 = pd.read_csv("Real_Test_MC.txt", delimiter='\t')

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

time_8 = data_8.iloc[:, 0]
A_r_2    = data_8.iloc[:, 1]
Fv_r_2   = data_8.iloc[:, 2]
Af_r_2   = data_8.iloc[:, 3]       
Sv_r_2   = data_8.iloc[:, 4]   
As_r_2   = data_8.iloc[:, 5]      
A2_r_2   = data_8.iloc[:, 6]

time_9 = data_9.iloc[:, 0]
theta_F = data_9.iloc[:, 1]
theta_S = data_9.iloc[:, 2]

# Create the plot
plt.figure(figsize=(9, 16))
plt.plot(time, A, linestyle='-', color='black', label='Concentration of $A$')
plt.plot(time_1, A_1, linestyle='--', color='orange', label='Concentration of $A$')
plt.plot(time, B, linestyle='-', color='blue', label='Concentration of $B$')
plt.plot(time_1, B_1, linestyle='--', color='cyan', label='Concentration of $B$')

plt.xlabel('Time', fontsize=14)
plt.ylabel('Concentration', fontsize=14)
#plt.title('Concentration vs Time')
plt.xscale('log')
plt.legend(fontsize=12)
plt.grid(True)
plt.show()

# Create the plot
plt.figure(figsize=(9, 16))
plt.plot(time_2, A_2, linestyle='-', color='black', label='Concentration of $A$')
plt.plot(time_3, A_3, linestyle='--', color='cyan', label='Concentration of $A$')
plt.plot(time_2, Sv, linestyle='-', color='blue', label='Concentration of $S_v$')
plt.plot(time_3, Sv_1, linestyle='--', color='black', label='Concentration of $S_v$')
plt.plot(time_2, As, linestyle='-', color='orange', label='Concentration of $A_s$')
plt.plot(time_3, As_1, linestyle='--', color='blue', label='Concentration of $A_s$')
plt.plot(time_2, A2, linestyle='-', color='cyan', label='Concentration of $A_2$')
plt.plot(time_3, A2_1, linestyle='--', color='orange', label='Concentration of $A_2$')

plt.xlabel('Time', fontsize=14)
plt.ylabel('Concentration', fontsize=14)
#plt.title('Concentration vs Time')
plt.xscale('log')
plt.legend(fontsize=12)
plt.grid(True)
plt.show()

# Create the plot
plt.figure(figsize=(9, 16))
plt.plot(time_4, A_4, linestyle='-', color='black', label='Concentration of $A$')
plt.plot(time_5, A_5, linestyle='--', color='orange', label='Concentration of $A$')
plt.plot(time_4, Fv, linestyle='-', color='orange', label='Concentration of $F_v$')
plt.plot(time_5, Fv_1, linestyle='--', color='blue', label='Concentration of $F_v$')
plt.plot(time_4, Af, linestyle='-', color='blue', label='Concentration of $A_f$')
plt.plot(time_5, Af_1, linestyle='--', color='black', label='Concentration of $A_f$')

plt.xlabel('Time', fontsize=14)
plt.ylabel('Concentration', fontsize=14)
#plt.title('Concentration vs Time')
plt.xscale('log')
plt.legend(fontsize=12)
plt.grid(True)
plt.show()

plt.figure(figsize=(9, 16))
plt.plot(time_6, A_r, linestyle='-', color='black', label='Concentration of $A$')
plt.plot(time_7, A_r_1, linestyle='--', color='cyan', label='Concentration of $A$')
#plt.plot(time_6, Fv_r, linestyle='-', color='r', label='Concentration of Fv')
plt.plot(time_6, Af_r, linestyle='-', color='blue', label='Concentration of $A_f$')
plt.plot(time_7, Af_r_1, linestyle='--', color='black', label='Concentration of $A_f$')
#plt.plot(time_6, Sv_r, linestyle='-', color='yellow', label='Concentration of Sv')
plt.plot(time_6, As_r, linestyle='-', color='orange', label='Concentration of $A_s$')
plt.plot(time_7, As_r_1, linestyle='--', color='blue', label='Concentration of $A_s$')
plt.plot(time_6, A2_r, linestyle='-', color='cyan', label='Concentration of $A_2$')
plt.plot(time_7, A2_r_1, linestyle='--', color='orange', label='Concentration of $A_2$')
#plt.plot(time_7, Fv_r_1, linestyle='--', color='b', label='Concentration of Fv of MC')
#plt.plot(time_7, Sv_r_1, linestyle='--', color='black', label='Concentration of Sv of MC')

plt.xlabel('Time', fontsize=14)
plt.ylabel('Concentration', fontsize=14)
plt.xscale('log')
#plt.yscale('log')
#plt.title('Concentration vs Time')
plt.legend(fontsize=12)
plt.grid(True)
plt.show()

plt.figure(figsize=(9, 16))

plt.plot(time_8, A_r_2, linestyle='--', color='black', label='Concentration of $A$')
#plt.plot(time_8, Fv_r_2, linestyle='--', color='b', label='Concentration of $F_v$')
plt.plot(time_8, Af_r_2, linestyle='--', color='blue', label='Concentration of $A_f$')
#plt.plot(time_8, Sv_r_2, linestyle='--', color='black', label='Concentration of $S_v$')
plt.plot(time_8, As_r_2, linestyle='--', color='orange', label='Concentration of $A_s$')
plt.plot(time_8, A2_r_2, linestyle='--', color='cyan', label='Concentration of $A_2$')

plt.xlabel('Time', fontsize=14)
plt.ylabel('Concentration', fontsize=14)
plt.xscale('log')
#plt.title('Concentration vs Time')
plt.legend(fontsize=12)
plt.grid(True)
plt.show()

plt.figure(figsize=(9, 16))

plt.plot(time_9, theta_F, linestyle='-', color='black', label=r'$\theta_F$')
plt.plot(time_9, theta_S, linestyle='-', color='orange', label=r'$\theta_S$')

plt.xlabel('Time', fontsize=14)
plt.ylabel('Concentration', fontsize=14)
plt.xscale('log')
#plt.title('Concentration vs Time')
plt.legend(fontsize=12)
plt.grid(True)
plt.show()