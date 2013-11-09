CXXFLAGS += -Wall -g -O0

all: flux-analysis install

flux-analysis: main.o analysis.o base.o statistics.cpp cmdline-parser.o system-class.o unknown-with-options.o
	g++ -Wall -o $@ $^ 
clean:
	rm flux-analysis
	rm main.o
	rm analysis.o
	rm base.o
	rm statistics.o
	rm system-class.o
install: 
	mkdir -p ~/Science/flux-analysis/graphs
	cp ./flux-analysis ~/Science/flux-analysis
	cp ./scripts/* ~/Science/flux-analysis
	cp ./configs/* ~/Science/flux-analysis
	cp ./3rdparty/* ~/Science/flux-analysis
myza:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat -t Myza.txt -F myza -u 6 -n 1.8 2
myza1:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat -o Myza.bin -F myza -u 6 -n 1.8 2 
myza2:
	./flux-analysis -i Myza.bin -t Myza.txt -q 2 -n 1.25 0.012
test1:
	./flux-analysis -f dataIPFAN-08122012.txt -F ipf -t 2plot.txt -n 2.4 120 --simple-detection config.txt	 -s "ss.txt" -M "stat.txt" -z +4 -K 30
plain:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat  -F myza -t Myza-plain.txt -u 6
