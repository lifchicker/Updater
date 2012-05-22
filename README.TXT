================================================================================
                                 ABOUT

Updater is an update and launch application and tools for your software.
Updater written on C++ with Qt libs usage.
Tools written on Python.

================================================================================
			    DEPENDENCIES
1. CMake (minimal version 2.6)
2. Qt (tested on 4.8.1). Required packages: QtCore, QtGui, QtNetwork.
3. Python (tesed on 2.7.3)

================================================================================
                           HOW TO USE IT

1. Replace UPDATER_TEMP_DIR_NAME in mainapplication.cpp file by your 
    application name.
    All temp files will be stored in 
C:\Documents and Settings\username\Local Settings\Temp\UPDATER_TEMP_DIR_NAME
    folder on Windows and
/tmp/UPDATER_TEMP_DIR_NAME
    directory on Linux

2. Build Updater:
2.1. On Linux:
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ../
	make
2.2. Cross compilation for Windows under Linux:
	cd build
	./build
	make
2.3. On Windows : google how to compile Qt application via CMake.

3. Put Updater and all dependencies to, for example, v1.0 and v2.0 of
    your application directory.

4. Read tools/README.txt to know how to use tools/create-update-file.py

5. Use tools/create-update-file.py to create full file list for v1.0
    cd tools
    ./create-update-file.py full

6. Use tools/create-update-file.py to create diff from v1.0 and v2.0

7. Put update.xml files to directories
7.1. update.xml from full put to v1.0
7.2. update.xml from diff put to v2.0
7.3. update.xml from diff put to platform directory
7.4. update.xml from full put to v1.0 installer as update-current.xml

8. Upload all files to your update server

For more information read section "UPDATE PROCESS".

================================================================================
				UPDATE PROCESS

1.  UpdateClient downloads new update.xml from
    <update><source>/<platform>/update.xml url
    and say it update-latest.xml

2.  UpdateClient compares <currentVersion> from update-latest.xml
    and update-current.xml (just downloaded and last updated respectively).
    If they are equal, then exit.

3.  UpdateClient checks <currentVersion> of update-current.xml and
    <previousVersion> from just downloaded update-latest.xml file.
    If versions are equal say update-latest.xml as update-resulting.xml and
    go to Step 5.

<!-- TODO: make algorithm for update from all previous versions at one time -->
4.1.  If versions doesn't equal, UpdateClient downloads previous version of
        update.xml from
        <update><source>/<platform>/<previousVersion>/update.xml
        and say it as update-latest.xml
4.2.  Go to Step 2.

5.  Using update-resulting.xml as file for update.

6.  For all files do

6.1.  Download file from
        <update><source>/<platform>/<currentVersion>/<install><file><name>
      to temp directory.

6.2.  Check file size and file hash.
      If file size or hash invalid, go to Step 6.1 with the same file name.
      If check fails N times, throw update error and exit.

7.  Copy all files to application directory.

8.  Rename update-resulting.xml to update-current.xml and copy to application
      directory.

9.  Go to Step 1.

================================================================================

                           UPDATE PROCESS ALGORITHM

1.  Read header from update-current.xml from application directory

2.  Download update.xml from
      <update><source>/<platform>/update.xml
    to temp directory and say it update-latest.xml

3.  Read header from update-latest.xml

4.  Compare <currentVersion> from update-latest.xml and update-current.xml.
    If they are equal and flag "installUpdates" is false, show "No updates"
      and exit.

5.  Check <currentVersion> of update-current.xml and <previousVersion> from
    update-latest.xml file.
    If versions are equal say update-latest.xml as update-resulting.xml and
    go to Step 7.

6.1.  If versions doesn't equal, download previous version of update.xml from
        <update><source>/<platform>/<previousVersion>/update.xml
      and say it as update-latest.xml

6.2.  Go to Step 3.

7.  If flag "installUpdates" is false, show dialog "New version found. Update?"
    and set flag to result of dialog choice.

8.  If flag "installUpdates" is false, exit.

9. For each file do

9.1.  Download file from
         <update><source>/<platform>/<currentVersion>/<install><file><name>
       to temp directory.

9.2.  Check file size and file hash.
       If file size or hash invalid, go to Step 10.1. with the same file name.
       If check fails N times, show message "Update error." and exit.


10. Rename update-resulting.xml to update-current.xml and copy to application
      directory.

11. Move all files to temp directory for result update.

12. If not all needed versions downloaded, go to Step 1.

13. Wait for application exiting.

14. Copy all files from temp directory for result update to application
      directory.



================================================================================
                           SIMPLE UPDATE FILE

<?xml version="1.0" ?>
   <update>
    <currentVersion>1.0.1</currentVersion>
    <previousVersion>1.0.0</previousVersion>
    <platform>windows</platform>
    <source>http://update.your.app/url</source>
    
    <application-main-binary>
     <file>
      <name>app.exe</name>
      <size>23049</size>
      <hash>a627ac81657487dc4206bb0d28d1699a</hash>
      <permissions>0755</permissions>
     </file>
    </application-main-binary>
    
    <install>
     <file>
      <name>app.exe</name>
      <size>23049</size>
      <hash>a627ac81657487dc4206bb0d28d1699a</hash>
      <permissions>0755</permissions>
     </file>
     <file>
      <name>README.txt</name>
      <size>16</size>
      <hash>bb25ef10dafca9498ba827000e834382</hash>
      <permissions>0644</permissions>
     </file>
     <file>
      <name>directory/file.txt</name>
      <size>26</size>
      <hash>c1d0fe90d578084dbcf2d5b0b5d63413</hash>
      <permissions>0644</permissions>
     </file>
    </install>
   </update>
