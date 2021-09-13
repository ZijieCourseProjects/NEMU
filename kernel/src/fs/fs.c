#include "common.h"
#include <sys/ioctl.h>
#include "string.h"
typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

typedef struct {
  bool opened;
  uint32_t offset;
} Fstate;


#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

Fstate fileState[NR_FILES +3];

int fs_ioctl(int fd, uint32_t request, void *p) {
	assert(request == TCGETS);
	return (fd >= 0 && fd <= 2 ? 0 : -1);
}

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);
void serial_printc(char);

/* TODO: implement a simplified file system here. */

int fs_open(const char *pathname, int flags){
  int i;
  for(i = 0; i < NR_FILES; i++){
	if (strcmp(pathname, file_table[i].name) == 0){
	  fileState[i + 3].opened = true;
	  fileState[i + 3].offset = 0;
	  return i + 3;
	}
  }
  return -1;
}

int fs_read(int fd, void *buf, int len){
  assert(fd > 2);
  assert(fileState[fd].opened);
  int id = fd - 3;
  if(fileState[fd].offset >= file_table[id].size) return 0;
  int l = len;
  if(fileState[id].offset + l >= file_table[id].size){
	l = file_table[id].size - fileState[fd].offset;
  }
  if (l == 0) return 0;
  memset(buf + l, 0, len - l);
  ide_read(buf, file_table[id].disk_offset + fileState[fd].offset, l);
  fileState[fd].offset += l;
  return l;
}

int fs_close(int fd){
  assert(fd < NR_FILES + 3);
  fileState[fd].opened = false;
  fileState[fd].offset = 0;
  return 0;
}

int fs_lseek(int fd, int offset, int whence){
  switch (whence){
	case SEEK_SET: {
	  fileState[fd].offset = offset;
	  return offset;
	}
	case SEEK_CUR: {
	  fileState[fd].offset += offset;
	  return fileState[fd].offset;
	}
	case SEEK_END: {
	  fileState[fd].offset = file_table[fd - 3].size + offset;
	  return fileState[fd].offset;
	}
  }
  return -1;
}

int fs_write(int fd,void *buf,int len){
  if(fd==2 || fd==1){
	int i;
	char *b = (char*)buf;
	for(i = 0; i < len; i++){
	  serial_printc(*(b));
	  ++b;
	}
	return len;
  }
  ide_write(buf,file_table[fd-3].disk_offset+fileState[fd].offset,len);
  fileState[fd].offset+=len;
  return len;
}