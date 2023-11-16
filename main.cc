#include "v8.h"

#include "src/Deskjs.hpp"

int main(int argc, char *argv[])
{
  char *filename = argv[1];
  auto *deskjs = new DeskJs();
  std::unique_ptr<v8::Platform> platform =
      deskjs->initializeV8(argc, argv);

  deskjs->initializeVM();
  deskjs->InitializeProgram(filename);
  deskjs->Shutdown();

  return 0;
}