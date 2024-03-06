#detecta se o sistema é de 32 ou 64 bits
BITS_OPTION = -m64

####diretorios com as libs do cplex
CPLEXDIR  = /opt/ibm/ILOG/CPLEX_Studio201/cplex
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio201/concert
   
CPLEXLIBDIR   = $(CPLEXDIR)/lib/x86-64_linux/static_pic
CONCERTLIBDIR = $(CONCERTDIR)/lib/x86-64_linux/static_pic

#### define o compilador
CPPC = g++
#############################

#### opcoes de compilacao e includes
CCOPT = $(BITS_OPTION) -O3 -fPIC -fexceptions -DNDEBUG -DIL_STD -std=c++0x
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include #Include das classes que vamos utilizar
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I./include #-I Serve para o compilador identificar que isso é um include de arquivos
#############################

#### flags do linker
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -ldl 
#############################

#### diretorios com os source files e com os objs files
SRCDIR = src
#############################

#### lista de todos os srcs
#WildCard irá listar todos os arquivos com *.cpp
SRCS = $(wildcard $(SRCDIR)/*.cpp)
#############################

#### regra principal, gera o executavel
$(SRCDIR)/solver: $(SRCS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CPPC) $(BITS_OPTION) $(SRCS) -o $@ $(CCLNFLAGS) $(CCFLAGS)
############################

