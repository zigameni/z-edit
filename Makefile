# Target: zedit
# This rule compiles zedit.c into an executable named "zedit"
zedit: zedit.c  
	$(CC) zedit.c -o zedit -Wall -Wextra -pedantic -std=c99  # Compile with warnings and C99 standard


