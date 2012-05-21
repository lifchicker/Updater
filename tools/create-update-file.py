#!/usr/bin/python2

__author__ = 'valor'

import os, sys
import hashlib
from xml.dom.minidom import Document
from sets import Set

class MakeXML:
    APP_FILE_PATH        = "app_name"
    CURRENT_VERSION_PATH = "/full/path/to/current/version"
    PREVIOUS_VERSION_PATH= "/full/path/to/previous/version"
    CURRENT_VERSION      = "1.1"
    PREVIOUS_VERSION     = "1.0"
    PLATFORM             = "Linux"
    SOURCE               = "http://update.your.app/url"

    def __init__(self):
        pass

    def make_xml_list(self, args):
        doc = Document()

        # create root element
        root = doc.createElement("update")
        doc.appendChild(root)

        # create header section
        current_version = doc.createElement("currentVersion")
        previous_version = doc.createElement("previousVersion")
        platform = doc.createElement("platform")
        source = doc.createElement("source")

        cv_text = doc.createTextNode(self.CURRENT_VERSION)
        pv_text = doc.createTextNode(self.PREVIOUS_VERSION)
        p_text  = doc.createTextNode(self.PLATFORM)
        s_text  = doc.createTextNode(self.SOURCE)

        current_version.appendChild(cv_text)
        root.appendChild(current_version)
        previous_version.appendChild(pv_text)
        root.appendChild(previous_version)
        platform.appendChild(p_text)
        root.appendChild(platform)
        source.appendChild(s_text)
        root.appendChild(source)

        # create application_main section

        app_main_binary = doc.createElement("application-main-binary")
        file = doc.createElement("file")

        name = doc.createElement("name")
        size = doc.createElement("size")
        hash = doc.createElement("hash")
        perm = doc.createElement("permissions")

        mainBinaryPath = self.CURRENT_VERSION_PATH + "/" + self.APP_FILE_PATH
        name.appendChild(doc.createTextNode(self.APP_FILE_PATH))
        size.appendChild(doc.createTextNode(str(self.get_fsize(mainBinaryPath))))
        hash.appendChild(doc.createTextNode(self.get_hash(mainBinaryPath)))
        perm.appendChild(doc.createTextNode(self.get_permission(mainBinaryPath)))

        file.appendChild(name)
        file.appendChild(size)
        file.appendChild(hash)
        file.appendChild(perm)

        app_main_binary.appendChild(file)
        root.appendChild(app_main_binary)

        # create install section

        install_section = doc.createElement("install")

        result_file_list = []

        if args == "full":
            result_file_list = self.curr_file_list()
            print "build full file list..."
        elif args == "diff":
            result_file_list = self.make_diff()

        for file_list in result_file_list:

            f_section = doc.createElement("file")

            f_name = doc.createElement("name")
            f_size = doc.createElement("size")
            f_hash = doc.createElement("hash")
            f_perm = doc.createElement("permissions")

            f_name.appendChild(doc.createTextNode(self.get_rpath(file_list, self.CURRENT_VERSION_PATH)))
            f_size.appendChild(doc.createTextNode(str(self.get_fsize(file_list))))
            f_hash.appendChild(doc.createTextNode(self.get_hash(file_list)))
            f_perm.appendChild(doc.createTextNode(self.get_permission(file_list)))

            f_section.appendChild(f_name)
            f_section.appendChild(f_size)
            f_section.appendChild(f_hash)
            f_section.appendChild(f_perm)

            install_section.appendChild(f_section)

        root.appendChild(install_section)

        self.write_xml(doc)

    def prev_file_list(self):
        file_list = dict()
        for (path, dirs, files) in os.walk(self.PREVIOUS_VERSION_PATH):
            for file in files:
                f = os.path.join(path, file)
                file_list.setdefault(self.get_rpath(f, self.PREVIOUS_VERSION_PATH), self.get_hash(f))
        return file_list

    def curr_file_list(self):
        file_list = []
        for (path, dirs, files) in os.walk(self.CURRENT_VERSION_PATH):
            for file in files:
                f = os.path.join(path, file)
                file_list.append(f)
        return file_list

    def make_diff(self):
        prev_list  = self.prev_file_list()
        result_list = Set()
        for curr_item in self.curr_file_list():
            if self.get_rpath(curr_item,self.CURRENT_VERSION_PATH) not in prev_list:
                print 'new file in new version -> '+ curr_item + " hash :"+ self.get_hash(curr_item) + "\n"
                result_list.add(curr_item)
            else:
                if self.get_hash(curr_item) != prev_list.get(self.get_rpath(curr_item, self.CURRENT_VERSION_PATH)):
                    print 'different hash -> '+ curr_item
                    result_list.add(curr_item)
        return result_list

    def write_xml(self,doc):
        f = open('update.xml', 'w')
        doc.writexml(f, indent="   ", addindent=" ", newl="\n")
        f.close()

    def get_hash(self,file):
        f = open(file, "r")
        bin = f.read()
        f.close()
        m = hashlib.md5()
        m.update(bin)
        return m.hexdigest()

    def get_fsize(self,file):
        return os.path.getsize(file)

    def get_permission(self,file):
        st = oct(os.stat(file).st_mode)[3:]
        return st

    def get_rpath(self,file,root):
        return os.path.relpath(file, start=root)

if __name__ == "__main__":
    app = MakeXML()
    app.make_xml_list(sys.argv[1])