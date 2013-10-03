BINDIR ?= /usr/local/bin
UDEV_RULESDIR ?= /etc/udev/rules.d
UDEV_LIBDIR ?= /lib/udev
KEYMAPDIR ?= /etc/rc_keymaps
all:
install:
	install -C 10-local.rules $(UDEV_RULESDIR)
	install -C getip $(BINDIR)
	install -C gingagui-minipc-walk $(BINDIR)
	install -C mount_usb $(UDEV_LIBDIR)
	install -C rc6_mce $(KEYMAPDIR)
	install -C trydhcp $(BINDIR)
	install -C umount_usb $(UDEV_LIBDIR)
	udevadm control --reload
	ir-keytable -c -w rc6_mce
uninstall:
	rm -f $(BINDIR)/getip
	rm -f $(BINDIR)/gingagui-minipc-walk
	rm -f $(BINDIR)/trydhcp
	rm -f $(KEYMAPDIR)/rc6_mce
	rm -f $(UDEV_LIBDIR)/mount_usb
	rm -f $(UDEV_LIBDIR)/umount_usb
	rm -f $(UDEV_RULESDIR)/10-local.rules
	udevadm control --reload
.PHONY: all install uninstall

