# Decisions

- C++20 and CMake: standard filesystem/ownership facilities with reproducible builds, no custom framework.
- OpenCV 4.9 minimum, 4.13.0 verified dependency target: buffer-based FaceDetectorYN loading avoids narrow Windows path handling. MSVC x64 selected because installed MinGW fails to launch.
- YuNet 2023mar FP32: small modern ONNX detector exposed directly by OpenCV. Haar cascades are simpler but less flexible under pose/lighting variation; large external inference runtimes add deployment cost. No numerical comparison was performed here.
- CPU and single thread: simple lifecycle and portable baseline. Optimize only after profiling real camera workloads.
- 320×320 default: moderate processing cost; larger input may retain small-face detail with increased latency. Direct resizing is easy to verify; aspect distortion is documented.
- Threshold 0.8, NMS 0.3, top-K 5000: configurable score filtering with fixed standard suppression parameters. Dimension multiples of 32 reduce incompatible input-shape risk.
- No automatic runtime downloads: explicit preparation script, hash-verified model, local operation.
- No license was chosen for original project code without the owner's preference. Third-party licenses remain applicable.

Sources: https://docs.opencv.org/4.9.0/df/d20/classcv_1_1FaceDetectorYN.html and https://github.com/opencv/opencv_zoo/tree/main/models/face_detection_yunet .

OpenCV generated include paths require /utf-8 under MSVC when sources live in this Unicode directory. Static CRT is matched explicitly by FACE_STATIC_CRT; use OFF for a DLL-runtime OpenCV distribution.

OpenCV precompiled headers are disabled: CMake/MSVC generated corrupted Unicode paths in those headers. The dependency source build succeeds with /utf-8 and precompiled headers disabled.
