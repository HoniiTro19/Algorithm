#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main() {
  const char *filename = "example.txt";
  const int filesize = 4096;

  int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    cerr << "cannot open file" << endl;
    return 1;
  }

  if (ftruncate(fd, filesize) == -1) {
    cerr << "cannot adjust file size" << endl;
    close(fd);
    return 1;
  }

  char *mapped_data = static_cast<char *>(
      mmap(nullptr, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (mapped_data == MAP_FAILED) {
    cerr << "cannot map file" << endl;
    close(fd);
    return 1;
  }

  const char *message = "Hello, World!";
  strncpy(mapped_data, message, strlen(message));
  if (msync(mapped_data, filesize, MS_SYNC) == -1) {
    cerr << "cannot sync mapped data" << endl;
  }
  if (munmap(mapped_data, filesize) == -1) {
    cerr << "cannot unmap data" << endl;
  }
  close(fd);
  return 0;
}