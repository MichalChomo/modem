//
// Created by michal on 26.10.2018.
//

#ifndef MODEM_ARGUMENTPARSER_H
#define MODEM_ARGUMENTPARSER_H


#include <string>

class FileNameHelper {
public:

    enum FileExtension {
        NONE, TXT, WAV
    };

    /**
     * Removes file extension from the filename. If it doesn't have an extension, returns it with no change.
     * @param fileName
     * @return Filename without extension.
     */
    static std::string getFileNameWithoutExtension(std::string &fileName);

    static std::string addExtensionToFileName(std::string &fileName, FileExtension fileExtension);

private:
    static std::string getFileExtensionString(FileExtension fileExtension);

};


#endif //MODEM_ARGUMENTPARSER_H