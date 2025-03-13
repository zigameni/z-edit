
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // using it to set the terminal atributes
#include <unistd.h>

struct termios orig_termios;

// Leave the terminal attributes the way we found them
void disableRawMode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); 
}

void enableRawMode()
{
  // Get the current terminal attributes for the standard input
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode); // runs at the end of the program, comes from stdlib

  struct termios raw = orig_termios;

  // Modify the terminal attributes to disable echoing of characters
  // ECHO flag controls the display of typed characters
  raw.c_lflag &= ~(ECHO | ICANON);  // Disable ECHO flag (won't display typed characters)

  // Apply the modified attributes to the terminal
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);  // Apply changes immediately, flushing the output buffer
}

int main() {
  enableRawMode();
  char c;
  while(read(STDIN_FILENO, &c, 1)==1 && c != 'q'){
    if(iscntrl(c)){// checks if the character is a control character, coms from ctype
      printf("%d\n", c);
    }else {
      printf("%d ('%c')\n", c, c);
    }
  }
  return 0;
}
