# C++ face detection

Local webcam face detection using C++20, OpenCV and YuNet. Draws multiple face boxes, confidence, smoothed application FPS and detection milliseconds. Q/Escape or closing the window exits. No recognition, recording, uploads or runtime network calls.

## Windows build and run

Prerequisites: Visual Studio 2022 with Desktop development with C++, Windows SDK, CMake 3.24+, OpenCV 4.9+ with core/imgproc/highgui/videoio/objdetect/dnn. The verified Windows build uses MSVC 19.44 and OpenCV 4.13.0 built locally. MinGW binaries bundled with CodeBlocks fail to start here; official MSVC OpenCV binaries cannot be mixed with MinGW.

Run PowerShell from the project:

```powershell
Set-Location -LiteralPath '<your-clone-directory>\face-detection'
cmake -S . -B build -G 'Visual Studio 17 2022' -A x64 -DOpenCV_DIR=build-opencv-current
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
./build/Release/face_detection.exe --check-model
./build/Release/face_detection.exe
```

The final command activates your webcam. Close other camera applications and allow desktop camera access in Windows Settings. There is no automatic recording. Relative model paths resolve from the current working directory. Quote paths containing spaces; Unicode model paths are supported.

Options (separate option and value, no `=`):

| Option | Default | Validation |
|---|---|---|
| `--camera` | 0 | integer 0–255 |
| `--model` | models/face_detection_yunet_2023mar.onnx | readable nonempty model, at most 16 MiB |
| `--threshold` | 0.8 | finite number in (0,1] |
| `--width`, `--height` | 320 each | multiples of 32, 32–2048 |
| `--check-model` | off | synthetic black-frame inference, no camera |
| `--help` | off | usage, no model/camera required |

Example: `./build/Release/face_detection.exe --camera 1 --threshold 0.7 --width 640 --height 480`.

## Dependencies from scratch

The project does not fetch dependencies automatically during configure or at runtime. The following source-build recipe uses official OpenCV 4.13.0. Downloads are explicit. Local `deps` and `build*` directories are ignored.

```powershell
New-Item -ItemType Directory -Force deps | Out-Null
Invoke-WebRequest 'https://github.com/opencv/opencv/archive/refs/tags/4.13.0.zip' -OutFile deps/opencv-4.13.0.zip
Expand-Archive -LiteralPath deps/opencv-4.13.0.zip -DestinationPath deps
cmake -S deps/opencv-4.13.0 -B build-opencv-current -G 'Visual Studio 17 2022' -A x64 '-DBUILD_LIST=core,imgproc,highgui,videoio,objdetect,dnn' -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_opencv_apps=OFF -DBUILD_SHARED_LIBS=OFF -DWITH_IPP=OFF -DWITH_OPENCL=OFF -DWITH_FFMPEG=OFF -DWITH_MSMF=ON -DWITH_PROTOBUF=ON -DBUILD_PROTOBUF=ON -DOPENCV_DNN_OPENCL=OFF -DENABLE_PRECOMPILED_HEADERS=OFF '-DCMAKE_CXX_FLAGS=/utf-8' '-DCMAKE_C_FLAGS=/utf-8'
cmake --build build-opencv-current --config Release --parallel 4
./scripts/get-model.ps1
```

The model script verifies the official LFS SHA-256 before replacing an existing model. Download its MIT license from the source listed in THIRD_PARTY.md if it is not already present.

Core-only tests need no OpenCV or webcam:

```powershell
cmake -S . -B build-core-current -G 'Visual Studio 17 2022' -A x64 -DFACE_BUILD_APP=OFF
cmake --build build-core-current --config Release
ctest --test-dir build-core-current -C Release --output-on-failure
```

For another OpenCV installation, pass its directory containing OpenCVConfig.cmake to `-DOpenCV_DIR=...`. Shared builds require the matching OpenCV DLL directory on PATH. Keep compiler, architecture and build configuration consistent. Linux builds should use ordinary CMake and a system OpenCV development package; that platform has not been tested.

## Verification status

Windows x64 Release build and all three CTest groups passed. Synthetic inference passed at 320x320 and 640x480, including an absolute Unicode model path. The user confirmed normal live webcam detection. Low-light/multiple-face behavior, disconnect handling, all quit controls, and live performance measurements remain unverified.

## Troubleshooting and limits

Missing model: run the model script or supply `--model`. Invalid model: replace with the verified ONNX file; an LFS pointer is not a model. Missing OpenCV: set OpenCV_DIR and build matching x64 libraries. Camera failure: check device index, Windows permissions, and applications holding the camera. An unreadable frame ends the app with exit code 1. Help and successful shutdown return 0; other errors return 1.

Faces can be missed or falsely detected, especially with low light, small faces, profile views and occlusion. Confidence is a model score, not a calibrated probability. Resizing to a square distorts nonsquare frames; boxes are correctly mapped back but accuracy can suffer. Increase input dimensions or match the camera aspect ratio to assess accuracy/speed tradeoffs. No performance guarantee is made.

See [plan](docs/PLAN.md), [architecture](docs/ARCHITECTURE.md), [decisions](docs/DECISIONS.md), [security](docs/SECURITY.md), [testing](docs/TESTING.md), and [third-party notices](THIRD_PARTY.md).


MSVC runtime: this source build uses static CRT, matched by FACE_STATIC_CRT=ON. For an OpenCV build using the DLL runtime, configure the application with -DFACE_STATIC_CRT=OFF. Runtime mismatches cause linker errors.
