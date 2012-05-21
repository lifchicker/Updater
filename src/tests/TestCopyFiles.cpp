#include <iostream>

#include "updateclient.h"

#include <QDir>

int main(int,char**)
{
    std::cout << "Test UpdateClient::copyFiles function...";

    UpdateClient client;
    client.copyFiles(QDir::currentPath() + QDir::separator() + "test1",
                     QDir::currentPath() + QDir::separator() + "test2");

    return 0;
}

