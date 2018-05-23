#def var,compile macro
CC = g++
LIBNAME = -lthostmduserapi -lthosttraderapi
LIBPATH = -L/home/zhangkai/ctp_linux/lib
INCLUDEPATH = -I/home/zhangkai/ctp_linux/include
# gcc will use this var
#CFLAGS = -Wall

OBJDIR = ../obj
VPATH = ./
SOURCE = $(wildcard $(VPATH)/*.cpp)
OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp,%.o,$(notdir $(SOURCE))))
OUTNAME = trader_main
#OBJS = ./obj/quote_spi.o ./obj/FileUtils.o ./obj/trade_spi.o ./obj/xtp_api_demo.o 
#INCLUDES = /usr/local/lib

#def command
main: ${OBJS}
	${CC} -o ${OUTNAME} ${OBJS} ${INCLUDEPATH} ${LIBNAME} ${LIBPATH}

#def command
clean:
	rm -f ${OBJS} ${OUTNAME}

${OBJDIR}/%.o: %.cpp
	gcc -c $< -o $@
