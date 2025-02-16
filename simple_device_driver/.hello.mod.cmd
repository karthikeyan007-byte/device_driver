savedcmd_/home/karthik/Device_driver/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/karthik/Device_driver/"$$0) }' > /home/karthik/Device_driver/hello.mod
