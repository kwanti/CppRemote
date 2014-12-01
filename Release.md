version 0.4
- Server will delete disconnected session objects to free resource.
- Update buffer_pool to better handle pool allocation.
- Fix issues compiling with boost 1.56 and 1.57.
- Fix issues compiling with MSVC2013.
- Add project files for MSVC2013.
- Tested with VC12, GCC4.9.1/MingGW64, GCC 4.8.1

Version 0.3
- Add filter element in bindings.
- Add zlib_filter to support compression.

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
