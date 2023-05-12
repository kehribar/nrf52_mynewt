# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------
APP_NAME=main

# ...
touch ./apps/$APP_NAME/src/main.c && newt build $1 && newt size $1 && \
  ../scripts/cmsis_gdb.sh ./bin/targets/$1/app/apps/$APP_NAME/$APP_NAME.elf
