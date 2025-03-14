/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


/*** data ***/

// Stores the original terminal settings to restore later
struct termios orig_termios;

/*** terminal ***/

// Function to handle errors and exit
void die(const char *s) { 
  perror(s);
  exit(1);
}

// Restores the original terminal settings
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

// Configures the terminal to raw mode
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode); // Ensures terminal settings are restored on exit
  
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // Disable special input processing
  raw.c_oflag &= ~(OPOST); // Disable output processing
  raw.c_cflag |= (CS8); // Set character size to 8 bits
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // Disable echo, canonical mode, and signals
  
  raw.c_cc[VMIN] = 0; // Minimum number of bytes needed before read() returns
  raw.c_cc[VTIME] = 1; // Timeout for read() set to 100ms
  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


/*** init ***/

int main() {
  enableRawMode(); // Enable raw mode for user input
  
  while (1) { // Infinite loop to read user input
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
    
    if (iscntrl(c)) {
      printf("%d\r\n", c); // Print control character codes
    } else {
      printf("%d ('%c')\r\n", c, c); // Print character with ASCII code
    }
    
    if (c == 'q') break; // Exit on 'q' key press
  }
  
  return 0;
}
