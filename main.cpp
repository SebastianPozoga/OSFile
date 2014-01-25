/* 
 * File:   main.cpp
 * Author: spozoga
 *
 * Created on December 27, 2013, 12:31 PM
 */

#include <cstdlib>
#include <iostream>
#include "OSF_PlainAPI.h"
#include "OSF_File.h"


using namespace std;

void ls_print(OSF_DirRecord* rec) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    if (OSF_Directory::isFile(rec)) {
        OSF_File* file = new OSF_File(plain->fs, rec->firstCluster);
        OSF_FileHeder header;
        file->readFileHeder(&header);
        //type
        cout << "(F)";
        //name
        cout << rec->name << " |";
        //owner
        cout << (header.permission & OSF_UR ? "UR|" : "");
        cout << (header.permission & OSF_UW ? "UW|" : "");
        cout << (header.permission & OSF_UX ? "UX|" : "");
        //group
        cout << (header.permission & OSF_GR ? "GR|" : "");
        cout << (header.permission & OSF_GW ? "GW|" : "");
        cout << (header.permission & OSF_GX ? "GX|" : "");
        //other
        cout << (header.permission & OSF_OR ? "OR|" : "");
        cout << (header.permission & OSF_OW ? "OW|" : "");
        cout << (header.permission & OSF_OX ? "OX|" : "");
        //owner id
        cout << " (" << header.owner << ")";
        cout << endl;
        //end
        delete file;
    } else {
        //type
        cout << "(D)";
        //name
        cout << rec->name << " - ";
        cout << endl;
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    cout << " --- OSFile command line system ---" << endl;
    cout << " Welcome. " << endl;
    cout << " Use 'help' to show command list.";

    string path("");

    char buffer[500];

    try {
        while (true) {
            cout << endl << path << "$: ";

            string command;
            cin >> command;

            //
            if (command == "new") {
                string yesno;
                cout << "Are you sure you want to delete the file system and create a new?? (y/n)" << endl;
                cin >> yesno;
                if (yesno != "y") {
                    cout << "Ok. You are not sure";
                    continue;
                }
                OSF_PlainAPI_createNew();
            }//
            else if (command == "cd") {
                cin >> path;
                if (path == "/") {
                    path = "";
                    continue;
                }
                OSF_PlainAPIData* plain = OSF_PlainAPI_init();
                OSF_DirectoryInterface* dir = plain->fs->getRootDir()->getDirectory(path);
                if (dir == NULL) {
                    cout << endl << " Direcotry:  " << path << " is not exist" << endl << endl;
                    path = "";
                    continue;
                }
                delete dir;
            }//
            else if (command == "ls") {
                OSF_Ls(path, &ls_print);
                cout << endl;
            }//
            else if (command == "write") {
                string filename;
                cin >> filename;
                cin >> buffer;
                OSF_FileHandle* handle = OSF_Open(path + "/" + filename, true);
                OSF_Write(handle, buffer, sizeof (buffer));
                OSF_Close(handle);
            }//
            else if (command == "read") {
                string filename;
                cin >> filename;
                OSF_FileHandle* handle = OSF_Open(path + "/" + filename, true);
                OSF_Read(handle, buffer, sizeof (buffer));
                OSF_Close(handle);
                cout << buffer;
            }//
            else if (command == "mkdir") {
                cin >> path;
                OSF_mkdir(path);
            }//
            else if (command == "setowner") {
                string filename;
                cin >> filename;
                OSF_OWNER owner;
                cin >> owner;
                OSF_chownFile(owner, path + "/" + filename);
            }//
            else if (command == "getowner") {
                string filename;
                cin >> filename;
                cout << OSF_chownFile(path + "/" + filename);
            }//
            else if (command == "setmod") {
                string filename;
                cin >> filename;
                OSF_OWNER mod;
                cin >> mod;
                OSF_chmodFile(mod, path + "/" + filename);
            }//
            else if (command == "getmod") {
                string filename;
                cin >> filename;
                cout << OSF_chmodFile(path + "/" + filename);
            }//
            else if (command == "rm") {
                string filename;
                cin >> filename;
                OSF_remove(path + "/" + filename);
            }//
            else if (command == "help") {
                cout << "Help:" << endl;
                cout << endl;
            }//
            else if (command == "exit") {
                cout << "GOODBYE" << endl;
                break;
            }//
            else {
                cout << "No know command: " << command << endl;
            }

        }
    } catch (OSF_Exception ex) {
        cout << "error: " << ex.getMessage();
    }

    OSF_PlainAPI_close();
    return 0;
}

