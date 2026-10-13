# Architecture

main.cpp owns startup, timing and shutdown. core.cpp validates CLI settings, reads bounded model bytes using filesystem paths, and provides hardware-independent geometry. camera.cpp owns VideoCapture through RAII. detector.cpp owns FaceDetectorYN and runs CPU inference. render.cpp owns the display window, overlays and quit controls; destruction suppresses cleanup exceptions.

Data flow: camera BGR frame → resize to configured input dimensions → FaceDetectorYN → validate float rows → scale/clip boxes → overlay on original frame → display. Frame memory is released/reused by OpenCV; no persistent image storage is created.

YuNet handles DNN preprocessing internally; do not manually grayscale, normalize, swap RGB channels or create a blob. Output is N×15 float values: x/y/width/height, five landmark pairs, score at column 14. Landmarks are unused. Nonfinite/invalid boxes and scores are discarded.

Coordinates use sx=frame_width/input_width and sy=frame_height/input_height. Left/top use floor; right/bottom use ceil; edges are clipped before integer conversion. A nonpositive clipped area is discarded. Direct resizing uses independent axis scales, without letterboxing.

Detection milliseconds include resizing, inference and result mapping, excluding rendering/capture. FPS uses elapsed time between successive inference completions (including intervening display/capture), exponentially smoothed with alpha 0.1. First-frame FPS is a startup estimate. Single-threaded processing avoids application queues, although camera drivers may buffer frames.

All startup/loop exceptions reach main's error boundary and return 1. Camera and window destructors release resources on normal exit and unwinding. A driver-blocked read cannot be guaranteed to return promptly; forced termination depends on the OS. No background threads or network client are present.
