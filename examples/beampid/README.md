# LKBeamPID Example

## Overview

`LKBeamPID` is a fitting and yield–extraction tool for 2D beam PID histograms.  
Each axis of the pid distribution is assumed to follow a Gaussian-like distribution with different widths, and multiple beam components may overlap.  
The user selects PID center points, and the program fits a 2D Gaussian to determine the yield and statistical uncertainty.

The 2D Gaussian function used in the fit has six parameters:

```
y = A * exp( -1/2 * [ 
      (( (x - x0)*cosθ + (y - y0)*sinθ ) / σx )^2
    + (( (y - y0)*cosθ - (x - x0)*sinθ ) / σy )^2
] )
```

Parameters: A, x0, y0, σx, σy, θ

---

## Included Example Files

For demonstration purposes, two small example files are provided:

- **example_chkf2run0083gen.root**
  Contains a small tree with sample PID-related branches.
  Useful for testing the “List files / Select file” workflow.

- **example_pid.root**
  Contains a ready-made 2D PID histogram for quick fitting tests.
  Useful for testing the “Use gPad” workflow directly.

These files allow you to try both usage modes without requiring external data.

---

## Configuration

A parameter file named `config.mac` controls the behavior of the analysis.  
It is written assuming that a root file is obtained from a CRIB using `ch...gen.root` style input file, so modify the parameters if your setting differs.

---

## Usage

There are two main ways to use the program.

---

## Method 1 — Using an Already-Drawn Beam PID Histogram

1. Draw the 2D PID histogram in ROOT.
2. Open the GUI by typing following on the root prompt: LKBeamPIDControl pid
3. Click **"3 Use gPad"** to attach the GUI to the current histogram canvas.
4. Click **"4 Select centers"** and select PID centers with the mouse. Double-click the final point to finish.
5. Click **"6 Fit total"** to perform the full 2D Gaussian fit.
6. Click **"7 Make summary"** to generate a summary file. The file name is automatically determined from the run number.

---

## Method 2 — Loading Files Directly From a Directory

1. Open the GUI by typing following on the root prompt: LKBeamPIDControl pid
2. Click **"1 List files"** to scan the current directory and its subdirectories for files that match the `file_format` pattern defined in `config.mac`.
3. Click **"2 Select file"**, then type a file index number into the input box at the top and press **Enter**.
4. Continue the same workflow as in Method 1 (steps 4–6). After finishing one run, return to step 3 to select another file and repeat the analysis.

---

## Output

- Fitted 2D Gaussian parameters for each selected PID region
- Extracted counts and uncertainties
- Automatically generated summary files named according to the run number
