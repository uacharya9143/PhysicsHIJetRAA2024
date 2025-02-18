FastJetConfig = /Users/yichen/Programs/FastJet/install/bin/fastjet-config
FastJetConfig = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet/3.3.4-ghbfee/bin/fastjet-config
FJContrib = /cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/fastjet-contrib/1.044-ghbfee/

default: all

all: library/Messenger.o library/TauHelperFunctions3.o library/DrawRandom.o library/DrawRandom2.o binary/DHAdd binary/DHSet binary/DHQuery binary/DHRemove

# all: library/Messenger.o library/RhoCalculator.o library/JetProducer.o library/TauHelperFunction3.o library/DrawRandom.o library/DrawRandom2.o

library/TauHelperFunctions3.o: source/TauHelperFunctions3.cpp include/TauHelperFunctions3.h
	g++ source/TauHelperFunctions3.cpp -I. -c -o library/TauHelperFunctions3.o `root-config --cflags` -std=c++17

library/DrawRandom.o: source/DrawRandom.cpp include/DrawRandom.h
	g++ source/DrawRandom.cpp -I. -c -o library/DrawRandom.o `root-config --cflags` -std=c++17

library/DrawRandom2.o: source/DrawRandom2.cpp include/DrawRandom2.h
	g++ source/DrawRandom2.cpp -I. -c -o library/DrawRandom2.o `root-config --cflags` -std=c++17

library/Messenger.o: source/Messenger.cpp include/Messenger.h
	g++ source/Messenger.cpp -I. -c -o library/Messenger.o `root-config --cflags` -std=c++17

library/RhoCalculator.o: source/RhoCalculator.cpp include/RhoCalculator.h
	g++ source/RhoCalculator.cpp -I. -c -o library/RhoCalculator.o `$(FastJetConfig) --cxxflags` -std=c++17 \
		-I$(RootMacrosBase) \
		`root-config --cflags` -std=c++17

library/JetProducer.o: source/JetProducer.cpp include/JetProducer.h
	g++ source/JetProducer.cpp -I. -c -o library/JetProducer.o -std=c++17 \
		`$(FastJetConfig) --cxxflags` -I$(FJContrib)/include \
		-I$(RootMacrosBase) \
		`root-config --cflags` -std=c++17

binary/DHAdd: source/DHAdd.cpp
	mkdir -p binary
	g++ source/DHAdd.cpp -o binary/DHAdd -I.

binary/DHRemove: source/DHRemove.cpp
	mkdir -p binary
	g++ source/DHRemove.cpp -o binary/DHRemove -I.

binary/DHQuery: source/DHQuery.cpp
	mkdir -p binary
	g++ source/DHQuery.cpp -o binary/DHQuery -I.

binary/DHSet: source/DHSet.cpp
	mkdir -p binary
	g++ source/DHSet.cpp -o binary/DHSet -I.
