savedcmd_gpio_module.mod := printf '%s\n'   gpio_module.o | awk '!x[$$0]++ { print("./"$$0) }' > gpio_module.mod
