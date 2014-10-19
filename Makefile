SOURCE_VERSION = 1.7
JFLAGS ?= -g:source,lines,vars -encoding utf8
TOUCH_DIR = .touch


all: compile jar eclipse src

# Sources
SRC = core ipc
src: $(SRC)
ipc::          core

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

define pre.ipc
	$(eval JAVA_HOME = $(shell dirname $(shell dirname $(shell readlink -f `which $(JAVAC)`))))
	-mkdir --parents $(BUILD_DIR)/$(MODULE)/net/aeten/core/ipc/linux-x86_64/
	gcc -nocpp -std=gnu99 -fPIC -shared -I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/linux src/$(MODULE)/net/aeten/core/ipc/posix_socket_jni.c -o $(BUILD_DIR)/$(MODULE)/net/aeten/core/ipc/linux-x86_64/libposixsocket.so
endef
