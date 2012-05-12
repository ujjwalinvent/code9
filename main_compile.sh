 g++ `pkg-config --cflags --libs opencv` -I ${GTEST_DIR}/include main.cpp ${GTEST_DIR}/lib/libgtest.a -lpthread -o main -Wno-write-strings -Wall -DDO_TEST

