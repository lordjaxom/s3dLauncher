s3dLauncher: s3dLauncher.cpp
	g++ -std=c++14 -Wall -DNDEBUG -DWIN32_LEAN_AND_MEAN -O2 -Wl,-subsystem,windows -o $@ $<
