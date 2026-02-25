
default: Execute ExecuteGraph
	bash Process.sh

Clean:
	rm -f *root *yaml *txt

Execute: TextToTree.cpp
	
	g++ TextToTree.cpp -o Execute `root-config --libs --cflags`

ExecuteGraph: ToGraph.cpp
	g++ ToGraph.cpp -o ExecuteGraph `root-config --libs --cflags` \

#Execute: ToGraph.cpp
#g++ ToGraph.cpp -o Execute `root-config --libs --cflags` \

#	-I$(ProjectBase)/CommonCode/include \
#	$(ProjectBase)/CommonCode/library/*.o 


