 g++ `pkg-config --cflags --libs opencv` -I ${GTEST_DIR}/include main2.cpp ${GTEST_DIR}/lib/libgtest.a -lpthread -o main2 -Wno-write-strings -Wall
