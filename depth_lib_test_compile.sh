 g++ `pkg-config --cflags --libs opencv` -I ${GTEST_DIR}/include depth_lib_test.cpp ${GTEST_DIR}/lib/libgtest.a -lpthread -o depth_lib_test_debug.out -Wno-write-strings -Wall
