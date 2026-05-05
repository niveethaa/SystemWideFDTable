CC = gcc
CFLAGS = -Wall -Werror -std=gnu99

all: showFDtables

showFDtables: cscb09_A2.c read_proc.c display_FDTables.c read_proc.h display_FDTables.h file_info.h
	$(CC) $(CFLAGS) -o showFDtables cscb09_A2.c read_proc.c display_FDTables.c

clean:
	rm -f showFDtables
