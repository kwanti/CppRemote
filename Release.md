
Version 0.2
- Fix linux GCC build issues.
- Fix linux clang build issues.
- Fix issue when compile with boost 1.53.
- Fix bug remote_cast null pointer not returning null pointer.
- Add local_transport to support unix domain socket.
- Add ipc_transport to select named_pipe_transport when compile in window and local_transport when compile in linux.
- Tested with VC9, GCC 4.7/MinGW32, GCC 4.8.1 and clang 3.2.

Version 0.1
- Initial public release
- Tested with VC9 and GCC 4.7/MinGW32.
