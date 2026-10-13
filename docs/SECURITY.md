# Security and privacy

Webcam activation follows successful configuration/model loading. The console announces it, and the window title identifies the active camera. Frames and detections stay in process memory; nothing is saved or uploaded. Camera indicator behavior depends on hardware/Windows. OS paging/crash dumps are outside application control.

Trust boundaries: CLI arguments, local model files, camera driver output, and dependency acquisition. Numeric inputs are range/finite checked; model reads are bounded to 16 MiB; boxes are clipped and checked before rendering. No argument is passed to a shell. Windows arguments are read as Unicode and converted to UTF-8 for parsing; filesystem model loading uses native paths and passes model bytes to OpenCV.

An arbitrary ONNX file is parsed by third-party native code. A size limit does not make an untrusted model safe: use the official hash-verified model. Only official source URLs are used by preparation commands. The model's published LFS SHA-256 is checked; no independent published checksum was identified for the OpenCV source archive. TLS and the official tagged source are the trust basis for that archive. The model URL follows main but its bytes are pinned by SHA-256; unexpected updates fail closed.

Keep compiler/OpenCV updated after reviewing compatibility. OpenCV may emit native error diagnostics. No secrets, telemetry, identity matching, or recognition databases exist. Warnings are enabled. FACE_SANITIZE supports GCC/Clang address/undefined sanitizers; those configurations are unverified here. This document describes defenses and limits, not a security certification.
