
## Install the Package Manager (vcpkg)
We will use vcpkg to manage dependencies. Instructions for installing can be found at the vcpkg [getting-started](https://vcpkg.io/en/getting-started.html) page but these are the basic steps. We will install it into C:\ for simplicity.

### 1: Change directory to the root
```
chdir C:\
```
### 2: Clone the vcpkg repo
```
git clone https://github.com/Microsoft/vcpkg.git
```
### 3: Run the bootstrap script to build vcpkg
```
.\vcpkg\bootstrap-vcpkg.bat
```
## Install the OpenCV3 package (3.4.16)
Enter the vcpkg root to access the executable
```
chdir vcpkg
```
Install the opencv3 package (with extra contrib) for x64 architecture
```
vcpkg.exe install opencv3[contrib]:x64-windows --recurse
```
## Install the DLib package (19.23)
Enter the vcpkg root to access the executable
```
chdir vcpkg
```
Install the dlib package for x64 architecture
```
vcpkg.exe install dlib:x64-windows --recurse
```

## Build the project
Navigate to the project root (where the CMakeLists.txt file is located) and initialize for x64 architecture
```
cmake -A x64 -S . -B "build64"
```
Then build the project (defaults to Debug)
```
cmake --build build64 --config Release
```
The executable and (required DLLs) will be found as:
```
 build64/Release/face_detection.exe
 ```
Before running copy the file ```image1.jpg``` from ```assets```. After running the file ```output.jpg``` contains the results. 

## Configuration
The folder ```assets``` contains the DNN model files:
* ```deploy.prototxt```
* ```res10_300x300_ssd_iter_140000_fp16.caffemodel```

These contain the network configuration and trained weights respectively and the detector is setup to use them
in CMakeLists.txt via variables.
* FACE_DETECTION_CONFIGURATION
* FACE_DETECTION_WEIGHTS

## Testing
At the moment the test images have to be copied from the ```assets``` folder into the executable folder. The filename is hardcoded in ```main.cpp```. Output always goes to ```output.png```