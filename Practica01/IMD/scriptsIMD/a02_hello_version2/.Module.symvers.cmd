cmd_/home/carlos/IMD/scriptsIMD/a02_hello_version2/Module.symvers :=  sed 's/ko$$/o/'  /home/carlos/IMD/scriptsIMD/a02_hello_version2/modules.order | scripts/mod/modpost -m -a    -o /home/carlos/IMD/scriptsIMD/a02_hello_version2/Module.symvers -e -i Module.symvers -T - 