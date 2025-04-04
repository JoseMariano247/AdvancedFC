import tkinter as tk
from tkinter import messagebox
import subprocess
import os
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

colors_colourblind = np.array(["blue", "black", "orange", "cyan", "palevioletred", "lime"])

# ------------------------------------------------
# Functions for Compilation, Running, and Plotting
# ------------------------------------------------

def compile_cpp_code(cpp_file, header_file):
    try:
        if not header_file:
            header_file = "Plasma-Surface-Recombination.h"
        compile_command = f"g++ -I inc2 {cpp_file} -o banana"
        result = subprocess.run(
            compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
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
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1)
        for line in iter(process.stdout.readline, ''):
            print(line, end='')
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
        data = pd.read_csv(file_path, delimiter='\t')
        time = data.iloc[:, 0]
        species_columns = data.columns[1:]
        
        plt.figure(figsize=(10, 6))
        for i, col in enumerate(species_columns):
            plt.plot(time, data[col]/np.max(data[col]), label=col, color=colors_colourblind[i % len(colors_colourblind)])
        
        plt.xlabel("Time", fontsize=14)
        plt.ylabel("Concentration", fontsize=14)
        plt.xscale("log")
        plt.legend(fontsize=12)
        plt.grid(True)
        plt.show()
    except Exception as e:
        messagebox.showerror("Error", f"Error generating plots: {str(e)}")

# --------------------------------------------
# Global variable to store reaction selections
# --------------------------------------------
selected_reactions_global = []

# --------------------------------------------
# Dictionary mapping reaction to parameter definitions
# --------------------------------------------
reaction_parameters = {
    "Basic": [
        {"label": "Constant of A → B", "default": "0.1"},
        {"label": "Constant of B → A", "default": "0.05"},
        {"label": "A Concentration", "default": "10000"},
        {"label": "B Concentration", "default": "5000"},
        {"label": "Stop Time", "default": "80.0"}
    ],
    "Physisorption": [
        {"label": "General Parameter Tw", "default": "200"},
        {"label": "General Parameter Tg", "default": "500"},
        {"label": "General Parameter M", "default": "16e-3"},
        {"label": "Sticking Probability for Physisorption", "default": "1.0"},
        {"label": "Desorption Frequency", "default": "10e15"},
        {"label": "Desorption Barrier", "default": "30e3"},
        {"label": "A Concentration", "default": "1e5"},
        {"label": "Fᵥ Concentration", "default": "1.5e5"},
        {"label": "Aₚ Concentration", "default": "0.0"},
        {"label": "Stop Time", "default": "5e-13"}
    ],
    "Chemisorption": [
        {"label": "General Parameter Tw", "default": "200"},
        {"label": "General Parameter Tg", "default": "500"},
        {"label": "General Parameter M", "default": "16e-3"},
        {"label": "Sticking Probability for Chemisorption", "default": "1.0"},
        {"label": "Pre-Exp. Factor for Recomb.", "default": "1.0"},
        {"label": "Recomb. Barrier", "default": "17.5e3"},
        {"label": "A Concentration", "default": "1e5"},
        {"label": "Sᵥ Concentration", "default": "3e3"},
        {"label": "Aₛ Concentration", "default": "0.0"},
        {"label": "A₂ Concentration", "default": "0.0"},
        {"label": "Stop Time", "default": "5e-13"}
    ],
    "Surface Diffusion": [
        {"label": "General Parameter Tw", "default": "200"},
        {"label": "General Parameter Tg", "default": "500"},
        {"label": "General Parameter M", "default": "16e-3"},
        {"label": "Diffusion Frequency", "default": "10e13"},
        {"label": "Diffusion Barrier", "default": "15e3"},
        {"label": "Aₚ Concentration", "default": "0.0"},
        {"label": "Aₛ Concentration", "default": "0.0"},
        {"label": "Fᵥ Concentration", "default": "1.5e5"},
        {"label": "Sᵥ Concentration", "default": "3e3"},
        {"label": "Stop Time", "default": "5e-13"}
    ],
    "Langmuir-Hinshelwood recombination": [
        {"label": "General Parameter Tw", "default": "200"},
        {"label": "General Parameter Tg", "default": "500"},
        {"label": "General Parameter M", "default": "16e-3"},
        {"label": "Diffusion Frequency", "default": "10e13"},
        {"label": "Diffusion Barrier", "default": "15e3"},
        {"label": "Pre-Exp. Factor for Recomb.", "default": "1.0"},
        {"label": "Recomb. Barrier", "default": "17.5e3"},
        {"label": "Recomb. Barrier for Two Physisorbed Atoms", "default": "0.0"},
        {"label": "Aₚ Concentration", "default": "0.0"},
        {"label": "Aₛ Concentration", "default": "0.0"},
        {"label": "Fᵥ Concentration", "default": "1.5e5"},
        {"label": "Sᵥ Concentration", "default": "3e3"},
        {"label": "A₂ Concentration", "default": "0.0"},
        {"label": "Stop Time", "default": "5e-13"}
    ]
}

# Fixed mapping for concentration labels to species names.
label_to_species = {
    "A Concentration": "A",
    "B Concentration": "B",
    "Aₚ Concentration": "Af",
    "Fᵥ Concentration": "Fv",
    "Aₛ Concentration": "As",
    "Sᵥ Concentration": "Sv",
    "A₂ Concentration": "A2"
}

fixedOrder = ["A", "B", "Af", "As", "Fv", "Sv", "A2"]

# --------------------------------
# First Window: Reaction Selection
# --------------------------------
def show_reaction_selection_window():
    reaction_win = tk.Tk()
    reaction_win.title("Select Reactions")
    tk.Label(reaction_win, text="Select Reactions:").pack(pady=10)
    
    reaction_vars = {}
    reaction_choices = list(reaction_parameters.keys())
    for choice in reaction_choices:
        var = tk.BooleanVar()
        reaction_vars[choice] = var
        tk.Checkbutton(reaction_win, text=choice, variable=var).pack(pady=2, anchor="w")
    
    def next_step():
        global selected_reactions_global
        selected = [r for r, var in reaction_vars.items() if var.get()]
        # Validation: "Basic" cannot be mixed with any other reaction.
        if "Basic" in selected and len(selected) > 1:
            messagebox.showerror("Error", "Basic reaction cannot be mixed with any other reaction.")
            return
        if not selected:
            messagebox.showerror("Error", "Please select at least one reaction")
        else:
            selected_reactions_global = selected
            reaction_win.destroy()
            show_parameter_window()  # Open dynamic parameters window
    
    tk.Button(reaction_win, text="Next", command=next_step).pack(pady=10)
    reaction_win.mainloop()

# ------------------------------------------
# Second Window: Dynamic Parameter Input with Scrollbar
# ------------------------------------------
def show_parameter_window():
    """
    Gathers parameters from selected reactions.
    Groups:
      - General Parameters: parameters that include "General Parameter".
      - Reaction Constants: parameters that do not include "General Parameter", "Stop Time", or "Concentration".
      - Concentrations: union of needed concentration species (using fixedOrder).
      - Stop Time: the stop time parameter, asked only once.
    """
    param_win = tk.Tk()
    param_win.title("Enter Simulation Parameters")
    param_win.geometry("400x500")

    # Create a canvas with scrollbar.
    canvas = tk.Canvas(param_win)
    scrollbar = tk.Scrollbar(param_win, orient="vertical", command=canvas.yview)
    scrollable_frame = tk.Frame(canvas)
    scrollable_frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
    canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
    canvas.configure(yscrollcommand=scrollbar.set)
    canvas.pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")
    
    # --- Grouping parameters ---
    general_params = {}
    constants = {}
    stop_time_param = None
    
    for reaction in selected_reactions_global:
        for param in reaction_parameters[reaction]:
            label = param["label"]
            if "Stop Time" in label:
                if stop_time_param is None:
                    stop_time_param = param
            elif "Concentration" in label:
                continue  # will be handled separately
            elif "General Parameter" in label:
                if label not in general_params:
                    general_params[label] = param
            else:
                if label not in constants:
                    constants[label] = param

    # Build union of concentration species from selected reactions.
    union_species = set()
    for reaction in selected_reactions_global:
        for param in reaction_parameters[reaction]:
            if "Concentration" in param["label"]:
                if param["label"] in label_to_species:
                    union_species.add(label_to_species[param["label"]])
    union_species_ordered = [s for s in fixedOrder if s in union_species]
    
    # For defaults of concentrations, choose the first occurrence.
    species_defaults = {}
    for reaction in selected_reactions_global:
        for param in reaction_parameters[reaction]:
            if "Concentration" in param["label"]:
                species = label_to_species.get(param["label"])
                if species and species not in species_defaults:
                    species_defaults[species] = param["default"]

    entry_widgets = []

    def create_group_frame(group_name, param_list):
        if not param_list:
            return
        frame = tk.LabelFrame(scrollable_frame, text=group_name, padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill="x")
        for param in param_list:
            tk.Label(frame, text=param["label"] + ":").pack(pady=2, anchor="w")
            entry = tk.Entry(frame)
            entry.insert(0, param["default"])
            entry.pack(pady=2, anchor="w", fill="x")
            entry_widgets.append(entry)

    # Create group for General Parameters first.
    create_group_frame("General Parameters", list(general_params.values()))
    
    # Then, create group for Reaction Constants.
    create_group_frame("Reaction Constants", list(constants.values()))
    
    # Create group for Concentrations.
    if union_species_ordered:
        frame = tk.LabelFrame(scrollable_frame, text="Concentrations", padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill="x")
        for species in union_species_ordered:
            tk.Label(frame, text=f"{species} Concentration:").pack(pady=2, anchor="w")
            entry = tk.Entry(frame)
            default_val = species_defaults.get(species, "")
            entry.insert(0, default_val)
            entry.pack(pady=2, anchor="w", fill="x")
            entry_widgets.append(entry)
    
    # Create group for Stop Time.
    if stop_time_param is not None:
        frame = tk.LabelFrame(scrollable_frame, text="Stop Time", padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill="x")
        tk.Label(frame, text=stop_time_param["label"] + ":").pack(pady=2, anchor="w")
        entry = tk.Entry(frame)
        entry.insert(0, stop_time_param["default"])
        entry.pack(pady=2, anchor="w", fill="x")
        entry_widgets.append(entry)
    
    def on_run():
        cpp_file = "src2/Plasma-Surface-Recombination.cpp"
        header_file = "inc2/Plasma-Surface-Recombination.h"
        compiled_program = compile_cpp_code(cpp_file, header_file)
        if not compiled_program:
            messagebox.showerror("Error", "Compilation failed.")
            return
        # The order passed to the C++ code will be:
        #   1) General Parameters,
        #   2) Reaction Constants,
        #   3) Concentrations (in fixed order, union_species_ordered),
        #   4) Then, the Stop Time.
        all_values = [e.get().strip() for e in entry_widgets]
        run_cpp_code(compiled_program, selected_reactions_global, all_values)
    
    tk.Button(scrollable_frame, text="Run Code", command=on_run).pack(pady=10)
    param_win.mainloop()

# Start the application
show_reaction_selection_window()
