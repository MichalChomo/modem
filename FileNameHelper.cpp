//
// Created by michal on 26.10.2018.
//

#include "FileNameHelper.h"

std::string FileNameHelper::getFileNameWithoutExtension(std::string &fileName) {
    unsigned long dotPos = fileName.find('.');
    if (dotPos != std::string::npos) {
        return fileName.substr(0, dotPos);
    }
    return fileName;
}

std::string
FileNameHelper::addExtensionToFileName(std::string &fileName, FileNameHelper::FileExtension fileExtension) {
    return fileName.append(".").append(getFileExtensionString(fileExtension));
}

std::string FileNameHelper::getFileExtensionString(FileNameHelper::FileExtension fileExtension) {
    switch (fileExtension) {
        case FileExtension::TXT:
            return "txt";
        case FileExtension::WAV:
            return "wav";
        default:
            return "";
    }
}