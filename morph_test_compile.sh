 g++ `pkg-config --cflags --libs opencv` -I ${GTEST_DIR}/include morph_test.cpp ${GTEST_DIR}/lib/libgtest.a -lpthread -o morph_test_debug.out -Wno-write-strings -Wall
