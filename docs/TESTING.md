# Testing

Core tests cover valid/boundary settings, missing/unknown arguments, malformed/overflow integers, nonfinite/out-of-range thresholds, unsupported dimensions, unequal-axis scaling, clipping, offscreen/invalid boxes, and Unicode/empty/missing model files. Assertions are active in Release builds. Run CTest using README commands.

Integration: --check-model loads ONNX and runs black-frame inference without webcam/GUI. This checks model/API compatibility, not real face accuracy. Separately exercise --help, invalid options, missing model, and malformed ONNX. No camera should open in those paths.

Manual webcam acceptance (requires user): no face, one face, multiple faces; faces near edges/at different distances; profile/occlusion/low light; camera unavailable/disconnected; Q, Escape, and window-close; confirm camera indicator stops and no image files appear. These checks have not been performed automatically. Do not launch the webcam during unattended verification.

Benchmark: record CPU, OS, compiler, OpenCV/model version, camera resolution, input dimensions, threshold, and lighting/face count. Warm up 30 frames, observe at least 60 seconds, record display FPS and detection milliseconds. Compare 320×320 and matching-aspect larger inputs under the same scene. UI values are smoothed/instantaneous diagnostics, not stored benchmark statistics. No measured webcam performance is claimed.

Verification results are updated at delivery below.

## Verified results (2026-10-13, Asia/Manila)

- OpenCV 4.13.0 source build and application Release build succeeded on MSVC 19.44 x64.
- CTest: 3/3 passed (core, startup, model_inference).
- Startup tests check exact exit codes and diagnostics for help, unknown options, nonfinite threshold, missing model and malformed ONNX, without opening a camera.
- Direct --check-model passed at 320x320 and at 640x480 using an absolute model path containing Japanese characters.
- Published model SHA-256 matches the local file.
- Core-only Release tests passed independently of OpenCV.
- Application build had no compiler warnings. Dependency builds may emit upstream warnings.
- Webcam, GUI interaction, driver-disconnect behavior, sanitizers, non-Windows builds and live performance remain unverified.

User verification: normal live webcam face detection confirmed on 2026-10-13. Remaining manual cases and quantitative performance measurements are still pending.
