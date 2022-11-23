# uld
Dynamic ELF Loader for ARM microcontrollers

## Supported Architectures
- ARM under heavy development
- ARMT slightly more stable, but still under heavey development

#
See the `test` directory for usage examples.

### ARM
### ARM Thumb/2
  - long calls
  Use a formula:
  ```c
  void* malloc(size_t)  __attribute__((long_call));
  void free(void*) __attribute__((long_call));
  ```