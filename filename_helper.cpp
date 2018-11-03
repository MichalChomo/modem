//
// Created by michal on 26.10.2018.
//

#include "filename_helper.h"

std::string FilenameHelper::getFilenameWithoutExtension(std::string &filename) {
    unsigned long dotPos = filename.find('.');
    if (dotPos != std::string::npos) {
        return filename.substr(0, dotPos);
    }
    return filename;
}

std::string
FilenameHelper::addExtensionToFilename(std::string filename, FilenameHelper::FileExtension fileExtension) {
    return filename.append(".").append(getFileExtensionString(fileExtension));
}

std::string FilenameHelper::getFileExtensionString(FilenameHelper::FileExtension fileExtension) {
    switch (fileExtension) {
        case FileExtension::TXT:
            return "txt";
        case FileExtension::WAV:
            return "wav";
        default:
            return "";
    }
}