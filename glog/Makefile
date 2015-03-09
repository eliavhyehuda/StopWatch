.phony: all clean

COMPILE_DIR = glog-0.3.3
INSTALL_DIR = glog
LIB_GLOG = ../glog/lib/libglog.a

all: $(LIB_GLOG)

$(LIB_GLOG):
	cd $(COMPILE_DIR) && ./configure --prefix=`pwd`/../$(INSTALL_DIR)
	cd $(COMPILE_DIR) && $(MAKE)
	cd $(COMPILE_DIR) && $(MAKE) install

clean:
	cd $(COMPILE_DIR) && $(MAKE) clean
	-rm -Rf $(INSTALL_DIR)
