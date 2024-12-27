cmd_/home/nat/operating_system/lab3/modules.order := {   echo /home/nat/operating_system/lab3/lab3.ko; :; } | awk '!x[$$0]++' - > /home/nat/operating_system/lab3/modules.order
