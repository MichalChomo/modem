//
// Created by michal on 26.10.2018.
//

#ifndef MODEM_ARGUMENTPARSER_H
#define MODEM_ARGUMENTPARSER_H


#include <string>

class FilenameHelper {
public:

    enum FileExtension {
        NONE, TXT, WAV
    };

    /**
     * Removes file extension from the filename. If it doesn't have an extension, returns it with no change.
     * @param filename
     * @return Filename without extension.
     */
    static std::string getFilenameWithoutExtension(std::string &filename);

    static std::string addExtensionToFilename(std::string &filename, FileExtension fileExtension);

private:
    static std::string getFileExtensionString(FileExtension fileExtension);

};


#endif //MODEM_ARGUMENTPARSER_H