BINDIR ?= /usr/local/bin
UDEV_RULESDIR ?= /etc/udev/rules.d
UDEV_LIBDIR ?= /lib/udev
all:
install:
	install -C gingagui-minipc-walk $(BINDIR)
	install -C 10-local.rules $(UDEV_RULESDIR)
	install -C mount_usb $(UDEV_LIBDIR)
	install -C umount_usb $(UDEV_LIBDIR)
	install -C trydhcp $(BINDIR)
	install -C getip $(BINDIR)
	udevadm control --reload
uninstall:
	rm -f $(BINDIR)/gingagui-minipc-walk
	rm -f $(UDEV_RULESDIR)/10-local.rules
	rm -f $(UDEV_LIBDIR)/mount_usb
	rm -f $(UDEV_LIBDIR)/umount_usb
	rm -f $(BINDIR)/trydhcp
	rm -f $(BINDIR)/getip
	udevadm control --reload
.PHONY: all install uninstall
