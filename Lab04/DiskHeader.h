// An abstract representation of a disk header 
struct disk_header {
	int initialDirection;
	int initialBlock;
	int currentBlock;
	int nextBlock;
};

void init_disk_header(struct disk_header * dheader, int starting_pos, int direction){
	dheader->initialDirection = direction;
	dheader->initialBlock = starting_pos;
	dheader->currentBlock = starting_pos;
}

void reset_disk_header(struct disk_header * dheader) {
	dheader->currentBlock = dheader->initialBlock;
}

int abs(int number) {
	return (number < 0) ? (-1 * number) : number;
}
