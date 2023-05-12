# -----------------------------------------------------------------------------
# 
# Flash firmware image and attach GDB session
# -----------------------------------------------------------------------------
arm-none-eabi-gdb $1 \
  -ex "set confirm off" \
  -ex "target extended-remote localhost:3333" \
  -ex "file $1" \
  -ex "load" \
  -ex "monitor reset init" \
  -ex "continue"
