#!/usr/bin/python2.7
import os
import subprocess

__author__ = "Alexey 'lh' Antonov"


def testCopyFile():
    os.mkdir("test1")
    f = open("test1/file.txt", "w")
    f.write("Test file text")
    f.close()

    subprocess.call("./TestCopyFile")

    if os.path.exists("test2/file.txt"):
        print "passed"
    else:
        print "failed"

    os.remove("test1/file.txt")
    os.rmdir("test1")
    os.remove("test2/file.txt")
    os.rmdir("test2")


def testCopyFiles():
    os.mkdir("test1")
    for i in range(1, 3, 1):
        f = open("test1/file" + str(i) + ".txt", "w")
        f.write("Test file text number " + str(i))
        f.close()

    os.mkdir("test1/directory")
    for i in range(1, 3, 1):
        f = open("test1/directory/file" + str(i) + ".txt", "w")
        f.write("Test file in directory subdirectory text number " + str(i))
        f.close()

    subprocess.call("./TestCopyFiles")

    passed = True
    for i in range(1, 3, 1):
        passed &= os.path.exists("test2/file" + str(i) + ".txt")
    for i in range(1, 3, 1):
        passed &= os.path.exists("test2/directory/file" + str(i) + ".txt")

    if passed:
        print "passed"
    else:
        print "failed"

    for i in range(1, 3, 1):
        os.remove("test1/file" + str(i) + ".txt")
    for i in range(1, 3, 1):
        os.remove("test1/directory/file" + str(i) + ".txt")
    os.rmdir("test1/directory")
    os.rmdir("test1")

    for i in range(1, 3, 1):
        os.remove("test2/file" + str(i) + ".txt")
    for i in range(1, 3, 1):
        os.remove("test2/directory/file" + str(i) + ".txt")
    os.rmdir("test2/directory")
    os.rmdir("test2")


def testMoveFile():
    os.mkdir("test1")
    f = open("test1/file.txt", "w")
    f.write("Test file text")
    f.close()

    subprocess.call("./TestMoveFile")

    if (not os.path.exists("test1/file.txt")) and os.path.exists("test2/file.txt"):
        print "passed"
    else:
        print "failed"

    os.rmdir("test1")
    os.remove("test2/file.txt")
    os.rmdir("test2")

if __name__ == "__main__":
# Test UpdateClient::copyFile function
    testCopyFile()
    testCopyFiles()
    testMoveFile()