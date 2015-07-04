SOURCE_VERSION = 1.7
JFLAGS ?= -g:source,lines,vars -encoding utf8
CFLAGS ?= -g
TOUCH_DIR = .touch

IPC_HEADERS = -I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/linux -Isrc/net.aeten.core
IPC_JNI = net.aeten.core.ipc/net/aeten/core/ipc
IPC_JNI_SRC = src/$(IPC_JNI)
IPC_JNI_LIB_DIR = build/$(IPC_JNI)/linux-x86_64
IPC_JNI_LIB = $(IPC_JNI_LIB_DIR)/libjnisocket.so $(IPC_JNI_LIB_DIR)/libjniioctl.so

all: compile jar eclipse src

# Sources
SRC = core ipc
src: $(SRC)
ipc:: ipc.jni core
ipc.jni: $(IPC_JNI_LIB)

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

.SECONDEXPANSION:
$(IPC_JNI_LIB): %: $$(shell ( cd $(IPC_JNI_SRC) && find . -mindepth 1 -maxdepth 1 -type f -name '*.c'|awk -v target=$$$$(basename $$@) '{sub ("./","",$$$$0)} {req=$$$$0} {gsub (/_|\.c/,"",req)} {if (target == "lib"req".so") {print "$(IPC_JNI_SRC)/"$$$$0}}' ))
	-@mkdir --parent $(@D)
	gcc -std=gnu99 -fPIC -shared $(CFLAGS) $(IPC_HEADERS) $< -o $@

