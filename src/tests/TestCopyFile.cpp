#include <iostream>

#include "updateclient.h"

#include <QDir>

int main(int,char**)
{
    std::cout << "Test UpdateClient::copyFile function...";

    UpdateClient client;
    client.copyFile(QDir::currentPath() + QDir::separator() + "test1",
                    QDir::currentPath() + QDir::separator() + "test2",
                    "file.txt");

    return 0;
}

