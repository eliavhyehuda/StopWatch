.phony: all clean

SRC_DIR = gtest-1.7.0
COMPILE_DIR = $(SRC_DIR)/make
INSTALL_DIR = gtest
INCLUDE_DIR = $(INSTALL_DIR)/include
LIB_DIR = $(INSTALL_DIR)/lib

GTEST_A = $(LIB_DIR)/gtest.a
GTEST_MAIN_A = $(LIB_DIR)/gtest_main.a

all: $(INSTALL_DIR) $(GTEST_A) $(GTEST_MAIN_A)

$(INSTALL_DIR):
	-mkdir $(INSTALL_DIR)
	-mkdir $(INCLUDE_DIR)
	-mkdir $(LIB_DIR)
	-cp -R $(SRC_DIR)/include/* $(INCLUDE_DIR)

$(GTEST_A):
	cd $(COMPILE_DIR) && $(MAKE) gtest.a
	cp $(COMPILE_DIR)/gtest.a $(GTEST_A)

$(GTEST_MAIN_A):
	cd $(COMPILE_DIR) && $(MAKE) gtest_main.a
	cp $(COMPILE_DIR)/gtest_main.a $(GTEST_MAIN_A)

clean:
	cd $(COMPILE_DIR) && $(MAKE) clean
	-rm -Rf $(INSTALL_DIR)
