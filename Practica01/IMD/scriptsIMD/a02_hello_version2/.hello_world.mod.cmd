cmd_/home/carlos/IMD/scriptsIMD/a02_hello_version2/hello_world.mod := printf '%s\n'   hello_version.o | awk '!x[$$0]++ { print("/home/carlos/IMD/scriptsIMD/a02_hello_version2/"$$0) }' > /home/carlos/IMD/scriptsIMD/a02_hello_version2/hello_world.mod