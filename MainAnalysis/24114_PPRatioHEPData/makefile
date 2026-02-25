


default: Execute
	bash Process.sh

Clean:
	rm -f *yaml *txt *root

Execute: ToGraph.cpp
	g++ ToGraph.cpp -o Execute `root-config --libs --cflags` \
	-I$(ProjectBase)/CommonCode/include \
	$(ProjectBase)/CommonCode/library/*.o 
