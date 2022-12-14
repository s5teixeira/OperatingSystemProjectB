/* Maria Bui, Ryan Valler, Steph Teixeira, Troy Josti */

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);

void main() {
	char line[80], buffer[512];

	makeInterrupt21();
	interrupt(0x21, 0, "Enter a line: ", 0, 0);
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);
	interrupt(0x21, 2, buffer, 30, 0);
	interrupt(0x21, 0, buffer, 0, 0);

	while(1);
}

void printString(char* chars) {
	while (*chars != 0x0) {
		interrupt(0x10, 0xe*256+*chars, 0, 0, 0);
		chars++;
	}
}

void printChar(char c) {
	interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

void readString(char* chars) {
	int i = 0;
	char key;

	while (i < 80) {
		key = interrupt(0x16, 0, 0, 0, 0);

		if (key == 0x8) {
			if (i != 0) {
				i--;
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
				interrupt(0x10, 0xe*256+' ', 0, 0, 0);
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
			}
		}
		else {
			chars[i] = key;

			if (key == 0xd) {
				interrupt(0x10, 0xe*256+0xd, 0, 0, 0);
				interrupt(0x10, 0xe*256+0xa, 0, 0, 0);
				chars[i+1] = 0xa;
				chars[i+2] = 0x0;
				return;
			}

			interrupt(0x10, 0xe*256+key, 0, 0, 0);
			i++;
		}
	}
}

void readSector(char* buffer, int sector) {
	interrupt(0x13, 0x2*256+1, buffer, 0*256+sector+1, 0*256+0x80);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	}
	else if (ax == 1) {
		readString(bx);
	}
	else if (ax == 2) {
		readSector(bx, cx);
	}
	else {
		printString("ERROR: Function does not exist.");
	}
}
