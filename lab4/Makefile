obj-m += lab4.o

PWD := $(CURDIR)

KEY_PATH := /home/nat/operating_system/lab4/key.priv
CERT_PATH := /home/nat/operating_system/lab4/cert.x509

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@$(MAKE) sign-module
sign-module:
	@$(shell /usr/src/linux-headers-$(shell uname -r)/scripts/sign-file sha256 $(KEY_PATH) $(CERT_PATH) lab4.ko)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
