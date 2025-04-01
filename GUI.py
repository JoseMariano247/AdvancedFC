import tkinter as tk
from tkinter import messagebox
import subprocess
import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys

# ----------------------------
# Functions for Compilation, Running, and Plotting
# ----------------------------

def compile_cpp_code(cpp_file, header_file):
    try:
        # Default header file if none provided
        if not header_file:
            header_file = "test.h"
        compile_command = f"g++ {cpp_file} {header_file} -o banana"
        result = subprocess.run(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if result.returncode != 0:
            messagebox.showerror("Compilation Error", f"Error compiling C++ code:\n{result.stderr.decode()}")
            return None
        else:
            return "./banana"
    except Exception as e:
        messagebox.showerror("Error", f"Error compiling C++ code: {str(e)}")
        return None     

def run_cpp_code(react_program, selected_reactions, parameters):
    try:
        command = [react_program] + selected_reactions + parameters

        # Start the process with Popen, enabling line-buffered text mode
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1)
        
        # Print output in real time to the terminal
        for line in iter(process.stdout.readline, ''):
            print(line, end='')  # Print each line as it comes
            
        process.stdout.close()
        process.wait()
        error_text = process.stderr.read()
        if process.returncode != 0:
            messagebox.showerror("Error", f"Error running C++ code:\n{error_text}")
        else:
            messagebox.showinfo("Success", "C++ code executed successfully. Check the terminal for output.")
            output_file = "output.txt"
            if os.path.exists(output_file):
                generate_plots(output_file)
            else:
                print("Output file not found; plot generation skipped.")
    except Exception as e:
        messagebox.showerror("Error", f"Error: {str(e)}")

def generate_plots(file_path):
    try:
        if selected_reactions_global[0] == "Basic":
            data = pd.read_csv(file_path, delimiter='\t')
            time = data.iloc[:, 0]
            A = data.iloc[:, 1]
            B = data.iloc[:, 2]
            plt.figure(figsize=(9, 16))
            plt.plot(time, A, linestyle='-', color='black', label='Concentration of A')
            plt.plot(time, B, linestyle='-', color='blue', label='Concentration of B')
            plt.xlabel('Time', fontsize=14)
            plt.ylabel('Concentration', fontsize=14)
            plt.xscale('log')
            plt.legend(fontsize=12)
            plt.grid(True)
            plt.show()

        if selected_reactions_global[0] == "Physisorption":
            data = pd.read_csv(file_path, delimiter='\t')
            time = data.iloc[:, 0]
            A = data.iloc[:, 1]
            F_v = data.iloc[:, 2]
            A_p = data.iloc[:, 3]  
            plt.figure(figsize=(9, 16))
            plt.plot(time, A, linestyle='-', color='black', label='Concentration of A')
            plt.plot(time, F_v, linestyle='-', color='blue', label='Concentration of Fᵥ')
            plt.plot(time, A_p, linestyle='-', color='orange', label='Concentration of Aₚ')
            plt.xlabel('Time', fontsize=14)
            plt.ylabel('Concentration', fontsize=14)
            plt.xscale('log')
            plt.legend(fontsize=12)
            plt.grid(True)
            plt.show()

        if selected_reactions_global[0] == "Chemisorption":
            data = pd.read_csv(file_path, delimiter='\t')
            time = data.iloc[:, 0]
            A = data.iloc[:, 1]
            S_v = data.iloc[:, 2]
            A_s = data.iloc[:, 3]
            A_2 = data.iloc[:, 4]   
            plt.figure(figsize=(9, 16))
            plt.plot(time, A, linestyle='-', color='black', label='Concentration of A')
            plt.plot(time, S_v, linestyle='-', color='blue', label='Concentration of Sᵥ')
            plt.plot(time, A_s, linestyle='-', color='orange', label='Concentration of Aₛ')
            plt.plot(time, A_2, linestyle='-', color='cyan', label='Concentration of A₂')
            plt.xlabel('Time', fontsize=14)
            plt.ylabel('Concentration', fontsize=14)
            plt.xscale('log')
            plt.legend(fontsize=12)
            plt.grid(True)
            plt.show()

        if selected_reactions_global[0] == "Surface Diffusion":
            data = pd.read_csv(file_path, delimiter='\t')
            time = data.iloc[:, 0]
            A_p = data.iloc[:, 1]
            A_s = data.iloc[:, 2]
            F_v = data.iloc[:, 3]
            S_v = data.iloc[:, 4]   
            plt.figure(figsize=(9, 16))
            plt.plot(time, A_p, linestyle='-', color='black', label='Concentration of Aₚ')
            plt.plot(time, A_s, linestyle='-', color='blue', label='Concentration of Aₛ')
            plt.plot(time, F_v, linestyle='--', color='orange', label='Concentration of Fᵥ')
            plt.plot(time, S_v, linestyle='-', color='cyan', label='Concentration of Sᵥ')
            plt.xlabel('Time', fontsize=14)
            plt.ylabel('Concentration', fontsize=14)
            plt.xscale('log')
            plt.legend(fontsize=12)
            plt.grid(True)
            plt.show()

        if selected_reactions_global[0] == "Langmuir-Hinshelwood recombination":
            data = pd.read_csv(file_path, delimiter='\t')
            time = data.iloc[:, 0]
            A_p = data.iloc[:, 1]
            A_s = data.iloc[:, 2]
            F_v = data.iloc[:, 3]
            S_v = data.iloc[:, 4]
            A_2 = data.iloc[:, 5]    
            plt.figure(figsize=(9, 16))
            plt.plot(time, A_p, linestyle='-', color='black', label='Concentration of Aₚ')
            plt.plot(time, A_s, linestyle='-', color='blue', label='Concentration of Aₛ')
            plt.plot(time, F_v, linestyle='-', color='orange', label='Concentration of Fᵥ')
            plt.plot(time, S_v, linestyle='-', color='cyan', label='Concentration of Sᵥ')
            plt.plot(time, A_2, linestyle='-', color='palevioletred', label='Concentration of A₂')
            plt.xlabel('Time', fontsize=14)
            plt.ylabel('Concentration', fontsize=14)
            plt.xscale('log')
            plt.legend(fontsize=12)
            plt.grid(True)
            plt.show()

    except Exception as e:
        messagebox.showerror("Error", f"Error generating plots: {str(e)}")

# ----------------------------
# Global variable to store reaction selections
# ----------------------------
selected_reactions_global = []

# ----------------------------
# First Window: Reaction Selection
# ----------------------------
def show_reaction_selection_window():
    reaction_win = tk.Tk()
    reaction_win.title("Select Reactions")

    tk.Label(reaction_win, text="Select Reactions:").pack(pady=10)

    reaction_vars = {}
    reaction_choices = [
        "Basic",
        "Physisorption",
        "Chemisorption",
        "Surface Diffusion",
        "Langmuir-Hinshelwood recombination"
    ]
    for choice in reaction_choices:
        var = tk.BooleanVar()
        reaction_vars[choice] = var
        tk.Checkbutton(reaction_win, text=choice, variable=var).pack(pady=2)

    def next_step():
        global selected_reactions_global
        selected = [r for r, var in reaction_vars.items() if var.get()]
        if not selected:
            messagebox.showerror("Error", "Please select at least one reaction")
        else:
            selected_reactions_global = selected
            reaction_win.destroy()
            show_parameter_window()  # Open the next window

    tk.Button(reaction_win, text="Next", command=next_step).pack(pady=10)
    reaction_win.mainloop()

# ------------------------------------------
# Second Window: Input Additional Parameters
# ------------------------------------------
def show_parameter_window():
    
    if selected_reactions_global[0] == "Basic":
        param_win = tk.Tk()
        param_win.title("Enter Simulation Parameters (Basic)")

        # --- Rate Constants Frame ---
        rate_frame = tk.Frame(param_win)
        rate_frame.pack(pady=(10, 10))
        tk.Label(rate_frame, text="Enter Reaction Rate Constants:").pack()
        tk.Label(rate_frame, text="Constant of A → B:").pack(pady=5)
        r1_param_entry = tk.Entry(rate_frame)
        r1_param_entry.pack(pady=5)
        tk.Label(rate_frame, text="Constant of B → A:").pack(pady=5)
        r2_param_entry = tk.Entry(rate_frame)
        r2_param_entry.pack(pady=5)

        # --- Concentrations Frame ---
        conc_frame = tk.Frame(param_win)
        conc_frame.pack(pady=(10, 10))
        tk.Label(conc_frame, text="Enter Concentrations:").pack()
        tk.Label(conc_frame, text="A Concentration:").pack(pady=5)
        a_conc_entry = tk.Entry(conc_frame)
        a_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="B Concentration:").pack(pady=5)
        b_conc_entry = tk.Entry(conc_frame)
        b_conc_entry.pack(pady=5)

        # --- Stop Time Frame ---
        time_frame = tk.Frame(param_win)
        time_frame.pack(pady=(10, 10))
        tk.Label(time_frame, text="Enter Stop Time:").pack(pady=5)
        time_entry = tk.Entry(time_frame)
        time_entry.pack(pady=5)

        def on_run():
            cpp_file = "test.cpp"
            header_file = "test.h"
            compiled_program = compile_cpp_code(cpp_file, header_file)
            if compiled_program:
                r1_param = r1_param_entry.get() or "0.1"
                r2_param = r2_param_entry.get() or "0.05"
                a_conc = a_conc_entry.get() or "10000"
                b_conc = b_conc_entry.get() or "5000"
                time_val = time_entry.get() or "80.0"
                parameters = [r1_param, r2_param, a_conc, b_conc, time_val]
                run_cpp_code(compiled_program, selected_reactions_global, parameters)
            else:
                messagebox.showerror("Error", "Compilation failed.")
        
        tk.Button(param_win, text="Run Code", command=on_run).pack(pady=10)
        param_win.mainloop()

    elif selected_reactions_global[0] == "Physisorption":
        param_win = tk.Tk()
        param_win.title("Enter Simulation Parameters (Physisorption)")

        # --- Rate Constants Frame ---
        rate_frame = tk.Frame(param_win)
        rate_frame.pack(pady=(10, 10))
        tk.Label(rate_frame, text="Enter Reaction Rate Constants:").pack()
        tk.Label(rate_frame, text="Constant of A + Fᵥ → Aₚ:").pack(pady=5)
        r1_param_entry = tk.Entry(rate_frame)
        r1_param_entry.pack(pady=5)
        tk.Label(rate_frame, text="Constant of Aₚ → A + Fᵥ:").pack(pady=5)
        r2_param_entry = tk.Entry(rate_frame)
        r2_param_entry.pack(pady=5)

        # --- Concentrations Frame ---
        conc_frame = tk.Frame(param_win)
        conc_frame.pack(pady=(10, 10))
        tk.Label(conc_frame, text="Enter Concentrations:").pack()
        tk.Label(conc_frame, text="A Concentration:").pack(pady=5)
        a_conc_entry = tk.Entry(conc_frame)
        a_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Fᵥ Concentration:").pack(pady=5)
        fv_conc_entry = tk.Entry(conc_frame)
        fv_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Aₚ Concentration:").pack(pady=5)
        af_conc_entry = tk.Entry(conc_frame)
        af_conc_entry.pack(pady=5)

        # --- Stop Time Frame ---
        time_frame = tk.Frame(param_win)
        time_frame.pack(pady=(10, 10))
        tk.Label(time_frame, text="Enter Stop Time:").pack(pady=5)
        time_entry = tk.Entry(time_frame)
        time_entry.pack(pady=5)

        def on_run():
            cpp_file = "test.cpp"
            header_file = "test.h"
            compiled_program = compile_cpp_code(cpp_file, header_file)
            if compiled_program:
                r1_param = r1_param_entry.get() or "0.1"
                r2_param = r2_param_entry.get() or "0.05"
                a_conc = a_conc_entry.get() or "10000"
                fv_conc = fv_conc_entry.get() or "5000"
                af_conc = af_conc_entry.get() or "0.0"
                time_val = time_entry.get() or "80.0"
                parameters = [r1_param, r2_param, a_conc, fv_conc, af_conc, time_val]
                run_cpp_code(compiled_program, selected_reactions_global, parameters)
            else:
                messagebox.showerror("Error", "Compilation failed.")
        
        tk.Button(param_win, text="Run Code", command=on_run).pack(pady=10)
        param_win.mainloop()

    elif selected_reactions_global[0] == "Chemisorption":
        param_win = tk.Tk()
        param_win.title("Enter Simulation Parameters")

        # --- Rate Constants Frame ---
        rate_frame = tk.Frame(param_win)
        rate_frame.pack(pady=(10, 10))
        tk.Label(rate_frame, text="Enter Reaction Rate Constants:").pack()
        tk.Label(rate_frame, text="Constant of A + Sᵥ → Aₛ:").pack(pady=5)
        r1_param_entry = tk.Entry(rate_frame)
        r1_param_entry.pack(pady=5)
        tk.Label(rate_frame, text="Constant of A + Aₛ → A₂ + Sᵥ:").pack(pady=5)
        r2_param_entry = tk.Entry(rate_frame)
        r2_param_entry.pack(pady=5)

        # --- Concentrations Frame ---
        conc_frame = tk.Frame(param_win)
        conc_frame.pack(pady=(10, 10))
        tk.Label(conc_frame, text="Enter Concentrations:").pack()
        tk.Label(conc_frame, text="A Concentration:").pack(pady=5)
        a_conc_entry = tk.Entry(conc_frame)
        a_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Sᵥ Concentration:").pack(pady=5)
        sv_conc_entry = tk.Entry(conc_frame)
        sv_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Aₛ Concentration:").pack(pady=5)
        af_conc_entry = tk.Entry(conc_frame)
        af_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="A₂ Concentration:").pack(pady=5)
        a2_conc_entry = tk.Entry(conc_frame)
        a2_conc_entry.pack(pady=5)

        # --- Stop Time Frame ---
        time_frame = tk.Frame(param_win)
        time_frame.pack(pady=(10, 10))
        tk.Label(time_frame, text="Enter Stop Time:").pack(pady=5)
        time_entry = tk.Entry(time_frame)
        time_entry.pack(pady=5)

        def on_run():
            cpp_file = "test.cpp"
            header_file = "test.h"
            compiled_program = compile_cpp_code(cpp_file, header_file)
            if compiled_program:
                r1_param = r1_param_entry.get() or "0.1"
                r2_param = r2_param_entry.get() or "0.05"
                a_conc = a_conc_entry.get() or "10000"
                sv_conc = sv_conc_entry.get() or "5000"
                as_conc = af_conc_entry.get() or "0.0"
                a2_conc = a2_conc_entry.get() or "0.0"
                time_val = time_entry.get() or "80.0"
                parameters = [r1_param, r2_param, a_conc, sv_conc, as_conc, a2_conc, time_val]
                run_cpp_code(compiled_program, selected_reactions_global, parameters)
            else:
                messagebox.showerror("Error", "Compilation failed.")
        
        tk.Button(param_win, text="Run Code", command=on_run).pack(pady=10)
        param_win.mainloop()

    elif selected_reactions_global[0] == "Surface Diffusion":
        param_win = tk.Tk()
        param_win.title("Enter Simulation Parameters")

        # --- Rate Constants Frame ---
        rate_frame = tk.Frame(param_win)
        rate_frame.pack(pady=(10, 10))
        tk.Label(rate_frame, text="Enter Reaction Rate Constants:").pack()
        tk.Label(rate_frame, text="Constant of Aₚ + Sᵥ → Fᵥ + Aₛ:").pack(pady=5)
        r1_param_entry = tk.Entry(rate_frame)
        r1_param_entry.pack(pady=5)

        # --- Concentrations Frame ---
        conc_frame = tk.Frame(param_win)
        conc_frame.pack(pady=(10, 10))
        tk.Label(conc_frame, text="Enter Concentrations:").pack()
        tk.Label(conc_frame, text="Aₚ Concentration:").pack(pady=5)
        af_conc_entry = tk.Entry(conc_frame)
        af_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Aₛ Concentration:").pack(pady=5)
        as_conc_entry = tk.Entry(conc_frame)
        as_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Fᵥ Concentration:").pack(pady=5)
        fv_conc_entry = tk.Entry(conc_frame)
        fv_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Sᵥ Concentration:").pack(pady=5)
        sv_conc_entry = tk.Entry(conc_frame)
        sv_conc_entry.pack(pady=5)

        # --- Stop Time Frame ---
        time_frame = tk.Frame(param_win)
        time_frame.pack(pady=(10, 10))
        tk.Label(time_frame, text="Enter Stop Time:").pack(pady=5)
        time_entry = tk.Entry(time_frame)
        time_entry.pack(pady=5)

        def on_run():
            cpp_file = "test.cpp"
            header_file = "test.h"
            compiled_program = compile_cpp_code(cpp_file, header_file)
            if compiled_program:
                r1_param = r1_param_entry.get() or "0.1"
                af_conc = af_conc_entry.get() or "10000"
                sv_conc = sv_conc_entry.get() or "5000"
                fv_conc = fv_conc_entry.get() or "0.0"
                as_conc = as_conc_entry.get() or "0.0"
                time_val = time_entry.get() or "80.0"
                parameters = [r1_param, af_conc, sv_conc, fv_conc, as_conc, time_val]
                run_cpp_code(compiled_program, selected_reactions_global, parameters)
            else:
                messagebox.showerror("Error", "Compilation failed.")
        
        tk.Button(param_win, text="Run Code", command=on_run).pack(pady=10)
        param_win.mainloop()

    elif selected_reactions_global[0] == "Langmuir-Hinshelwood recombination":
        param_win = tk.Tk()
        param_win.title("Enter Simulation Parameters")

        # --- Rate Constants Frame ---
        rate_frame = tk.Frame(param_win)
        rate_frame.pack(pady=(10, 10))
        tk.Label(rate_frame, text="Enter Reaction Rate Constants:").pack()
        tk.Label(rate_frame, text="Constant of Aₚ + Aₛ → A₂ + Sᵥ + Fᵥ:").pack(pady=5)
        r1_param_entry = tk.Entry(rate_frame)
        r1_param_entry.pack(pady=5)
        tk.Label(rate_frame, text="Constant of Aₚ + Aₚ → A₂ + Fᵥ + Fᵥ:").pack(pady=5)
        r2_param_entry = tk.Entry(rate_frame)
        r2_param_entry.pack(pady=5)

        # --- Concentrations Frame ---
        conc_frame = tk.Frame(param_win)
        conc_frame.pack(pady=(10, 10))
        tk.Label(conc_frame, text="Enter Concentrations:").pack()
        tk.Label(conc_frame, text="Aₚ Concentration:").pack(pady=5)
        af_conc_entry = tk.Entry(conc_frame)
        af_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Aₛ Concentration:").pack(pady=5)
        as_conc_entry = tk.Entry(conc_frame)
        as_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Fᵥ Concentration:").pack(pady=5)
        fv_conc_entry = tk.Entry(conc_frame)
        fv_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="Sᵥ Concentration:").pack(pady=5)
        sv_conc_entry = tk.Entry(conc_frame)
        sv_conc_entry.pack(pady=5)
        tk.Label(conc_frame, text="A₂ Concentration:").pack(pady=5)
        a2_conc_entry = tk.Entry(conc_frame)
        a2_conc_entry.pack(pady=5)

        # --- Stop Time Frame ---
        time_frame = tk.Frame(param_win)
        time_frame.pack(pady=(10, 10))
        tk.Label(time_frame, text="Enter Stop Time:").pack(pady=5)
        time_entry = tk.Entry(time_frame)
        time_entry.pack(pady=5)

        def on_run():
            cpp_file = "test.cpp"
            header_file = "test.h"
            compiled_program = compile_cpp_code(cpp_file, header_file)
            if compiled_program:
                r1_param = r1_param_entry.get() or "0.01"
                r2_param = r2_param_entry.get() or "0.1"
                af_conc = af_conc_entry.get() or "10000"
                sv_conc = sv_conc_entry.get() or "0.0"
                fv_conc = fv_conc_entry.get() or "0.0"
                as_conc = as_conc_entry.get() or "1000.0"
                a2_conc = a2_conc_entry.get() or "0.0"
                time_val = time_entry.get() or "80.0"
                parameters = [r1_param, r2_param, af_conc, sv_conc, fv_conc, as_conc, a2_conc, time_val]
                run_cpp_code(compiled_program, selected_reactions_global, parameters)
            else:
                messagebox.showerror("Error", "Compilation failed.")
        
        tk.Button(param_win, text="Run Code", command=on_run).pack(pady=10)
        param_win.mainloop()

show_reaction_selection_window()
