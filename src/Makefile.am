AUTOMAKE_OPTIONS = foreign

# -------------------------------------------------
# Program
# -------------------------------------------------
bin_PROGRAMS = m68k

# -------------------------------------------------
# Subdirectories
# -------------------------------------------------
SUBDIRS = cpu debug io system tools

# -------------------------------------------------
# Compiler flags
# -------------------------------------------------
AM_CPPFLAGS =

# -------------------------------------------------
# UI resources (optional, platform dependent)
# -------------------------------------------------
if USE_UI_WIN32
m68k_RES = system/ui/@UI_DIR@/resources.o
else
m68k_RES =
endif

# -------------------------------------------------
# Linker input
# -------------------------------------------------
m68k_LDADD = \
	io/graphic/libgraphic.a \
	io/ide/libide.a \
	system/ui/@UI_DIR@/libui.a \
	io/libio.a \
	io/prom/libprom.a \
	io/prom/fs/libfs.a \
	io/prom/fs/hfs/libhfs.a \
	io/prom/fs/hfsplus/libhfsplus.a \
	io/pic/libpic.a \
	io/pci/libpci.a \
	io/macio/libmacio.a \
	io/nvram/libnvram.a \
	io/cuda/libcuda.a \
	io/3c90x/lib3c90x.a \
	io/rtl8139/librtl8139.a \
	io/usb/libusb.a \
	io/serial/libserial.a \
	cpu/mango68k/libcpu.a \
	debug/libdebug.a \
	tools/libtools.a \
	system/libsystem.a \
	system/arch/@ARCH_DIR@/libsarch.a \
	system/osapi/@OSAPI_DIR@/libsosapi.a \
	$(m68k_RES) \
	@M68K_LDADD@

# -------------------------------------------------
# Linker flags
# -------------------------------------------------
m68k_LDFLAGS = @M68K_LDFLAGS@

# -------------------------------------------------
# Sources
# -------------------------------------------------
m68k_SOURCES = \
	main.cc \
	info.h \
	m68k_img.c \
	m68k_img.h \
	m68k_font.c \
	m68k_font.h \
	m68k_button_changecd.c \
	m68k_button_changecd.h \
	configparser.cc \
	configparser.h

# -------------------------------------------------
# Distribution helpers
# -------------------------------------------------
dist2: distdir
	$(AMTAR) chof - $(distdir) | BZIP2=$(BZIP2_ENV) bzip2 -c >$(distdir).tar.bz2
	$(am__remove_distdir)

dist-all2: dist2
