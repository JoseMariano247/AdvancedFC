import tkinter as tk
from tkinter import messagebox
import subprocess
import os
import matplotlib.pyplot as plt
import numpy as np

# Function to compile the C++ code using g++
def compile_cpp_code(cpp_file):
    try:
        # Compile the C++ code using g++
        compile_command = f"gcc {cpp_file} -o banana"
        
        result = subprocess.run(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Check if there are any compilation errors
        if result.returncode != 0:
            messagebox.showerror("Compilation Error", f"Error compiling C++ code: {result.stderr.decode()}")
            return None
        else:
            return "compiled_program"
    except Exception as e:
        messagebox.showerror("Error", f"Error compiling C++ code: {str(e)}")
        return None

# Function to run the compiled C++ code with selected reactions
def run_cpp_code(react_program, selected_reactions, parameters):
    try:
        # Prepare the command with selected reactions and parameters
        command = [react_program] + selected_reactions + parameters
        
        # Run the compiled C++ program
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Check if the C++ program ran successfully
        if result.returncode != 0:
            messagebox.showerror("Error", f"Error running C++ code: {result.stderr.decode()}")
        else:
            messagebox.showinfo("Success", "C++ code executed successfully")
            # Assuming the output is saved as output.txt after running the C++ code
            generate_plots('output.txt')  # Call the plot generation function
    except Exception as e:
        messagebox.showerror("Error", f"Error: {str(e)}")

# Function to generate plots from the output.txt
def generate_plots(file_path):
    try:
        # Load the data from the file (assuming it's a space-separated text file)
        data = np.loadtxt(file_path)
        
        # Example plot (modify as per your data structure)
        plt.plot(data[:, 0], data[:, 1])
        plt.title("Recombination Plot")
        plt.xlabel("X-axis Label")
        plt.ylabel("Y-axis Label")
        plt.grid(True)
        
        # Display the plot
        plt.show()
    except Exception as e:
        messagebox.showerror("Error", f"Error generating plots: {str(e)}")

# Function to get input from GUI and run C++ code
def on_run_button_click():
    cpp_file = r"C:\\Users\\jmcm2\Desktop\\Monte_Carlo_Vasco\\test.cpp"  # Path to the C++ file

    # Compile the C++ code
    compiled_program = compile_cpp_code(cpp_file)
    
    if compiled_program:
        # Get selected reactions (multiple reactions can be selected)
        selected_reactions = [reaction for reaction, var in reaction_vars.items() if var.get()]
        
        # Get the reaction parameters
        parameters = [param_entry.get() for param_entry in parameter_entries]
        
        if not selected_reactions:
            messagebox.showerror("Error", "Please select at least one reaction")
            return
        # Run the compiled C++ code with these inputs
        run_cpp_code(compiled_program, selected_reactions, parameters)
        
# Set up the Tkinter GUI
root = tk.Tk()
root.title("Plasma Recombination GUI")

# Reaction options (you can expand this as needed)
reaction_vars = {}
reaction_label = tk.Label(root, text="Select Reactions:")
reaction_label.pack()

reaction_choices = ["Physisorption", "Chemiosorption",
                    "Eley-Rideal recombination", "Langmuir-Hinshelwood recombination"]  # Modify as per actual reactions
for choice in reaction_choices:
    var = tk.BooleanVar()
    reaction_vars[choice] = var
    tk.Checkbutton(root, text=choice, variable=var).pack()

# Parameters input
parameter_label = tk.Label(root, text="Enter Reaction Parameters:")
parameter_label.pack()

parameter_entries = []
for i in range(len(reaction_choices)):  # Modify the range to match the number of parameters for your reactions
    param_entry = tk.Entry(root)
    param_entry.pack()
    parameter_entries.append(param_entry)

# Button to run the code
run_button = tk.Button(root, text="Run Code", command=on_run_button_click)
run_button.pack()

# Start the Tkinter event loop
root.mainloop()
