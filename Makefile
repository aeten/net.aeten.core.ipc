SOURCE_VERSION = 1.7
JFLAGS ?= -g:source,lines,vars -encoding utf8
CFLAGS ?= -g
TOUCH_DIR = .touch


all: compile jar eclipse src

# Sources
SRC = core ipc
src: $(SRC)
ipc:: ipc.jni core
ipc.jni:
	gcc -nocpp -std=gnu99 -fPIC -shared $(CFLAGS) $(IPC_HEADERS) src/net.aeten.core.ipc/net/aeten/core/ipc/jni_socket.c -o src/net.aeten.core.ipc/net/aeten/core/ipc/linux-x86_64/libjnisocket.so
	gcc -nocpp -std=gnu99 -fPIC -shared $(CFLAGS) $(IPC_HEADERS) src/net.aeten.core.ipc/net/aeten/core/ipc/jni_ioctl.c -o src/net.aeten.core.ipc/net/aeten/core/ipc/linux-x86_64/libjniioctl.so


# COTS
COTS = jcip.annotations slf4j slf4j.simple
cots: $(COTS)
core::             jcip.annotations slf4j
jcip.annotations::
slf4j::
slf4j.simple::     slf4j

clean:
	$(RM) -rf $(BUILD_DIR) $(DIST_DIR) $(GENERATED_DIR) $(TOUCH_DIR)

SRC_DIRS = src/
MODULES = $(SRC) $(COTS)
include Java-make/java.mk

IPC_HEADERS = -I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/linux -Isrc/net.aeten.core
