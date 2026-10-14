# MRI DICOM image Segmentation — Qt 6 Application

> **Project status: Work in Progress (WIP) — Not Ready for Contributions**

This repository contains an experimental Qt 6 application for loading an axial brain MRI DICOM image, running brain structure segmentation model, and displaying the segmentation overlay.

The project is under active development. The code, dependencies, and segmentation pipeline may change significantly.
This repository is published for visibility and development tracking only.

## 1. Prerequisites

Install the following software:

* Python 3.12.3
* Git.
* Qt 6 and the Python Qt bindings used by the application (`PySide6` or `PyQt6`).

## 2. Clone the repository

Open a terminal and run:

```bash
git clone https://github.com/laaurm/DataVisualization-Qt6.git
cd DataVisualization-Qt6
```

## 3. Create a Python virtual environment

### Windows

```powershell
python -m venv .venv
.venv\Scripts\activate
```

### Linux / macOS

```bash
python3 -m venv .venv
source .venv/bin/activate
```

Upgrade pip:

```bash
python -m pip install --upgrade pip
```

## 4. Install the dependencies

If the repository contains a `requirements.txt` file, run:

```bash
pip install -r requirements.txt
```

Otherwise, install the application's dependencies according to the project documentation.

For a PySide6 application, the Qt bindings can be installed with:

```bash
pip install PySide6
```

For a PyQt6 application, use:

```bash
pip install PyQt6
```

Use the binding already used by the project; do not install both unless required.

Additional packages may be necessary for DICOM processing and image handling:

```bash
pip install pydicom numpy pillow
```

Install the appropriate machine-learning dependencies and download the required pretrained model weights before running segmentation.

## 5. Activate virtual environement

```bash
source .venv/bin/activate
```

## 5. Build and Run the Application

You can compile the source code and launch the Qt 6 application using the following commands from the project root directory.

### Configure the project

```bash
cmake -S . -B build -G Ninja
```

This configures the project using CMake and generates Ninja build files in the `build/` directory.

### Compile the application

```bash
cmake --build build
```

This compiles the source code and generates the application executable.

### Launch the application

```bash
./build/DataVisualization
```

The Qt 6 application will launch. You can then select a DICOM image and run the segmentation pipeline.

**Prerequisites:** Make sure CMake, Ninja, a compatible C++ compiler, and the required Qt 6 development libraries are installed before building the project.

## 6. Use the application

1. Launch the Qt 6 application.
2. Select or open a brain MRI DICOM image. There are some in the folder /data.
3. The segmentation operation should start
4. Verify that the image is displayed in the application.

## 7. Running the segmentation script independently

It is possible to run the DICOM segmentation script, it may also be possible to run it without launching the GUI:

```bash
python algo/segmentation.py "/path/to/brain_image.dcm"
```

Replace the path with the location of your DICOM file.
