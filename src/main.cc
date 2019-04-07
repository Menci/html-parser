#include <iostream>
#include <fstream>

#include "Arguments.h"
#include "HTMLDocument.h"

int main(int argc, char *argv[]) {
    Arguments arguments = parseArguments(argc, argv);

    HTMLDocument document = arguments.inputFile == '-' ? HTMLDocument(std::cin)
                                                       : HTMLDocument(std::ifstream(arguments.inputFile));
    document.inspect();
    std::cout << document.getArticleContent() << std::endl;
}
