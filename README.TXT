================================================================================

                           UPDATE USAGE HOW TO

UPDATE PROCESS:

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

9.  Read list of files from update-resulting.xml

10. For each file do

10.1.  Download file from
         <update><source>/<platform>/<currentVersion>/<install><file><name>
       to temp directory.

10.2.  Check file size and file hash.
       If file size or hash invalid, go to Step 10.1. with the same file name.
       If check fails N times, show message "Update error." and exit.

11. Move all files to temp directory for result update.

12. Rename update-resulting.xml to update-current.xml and copy to application
      directory.

13. If not all needed versions downloaded, go to Step 1.

14. Wait for application exiting.

15. Copy all files from temp directory for result update to application
      directory.



================================================================================
                           SIMPLE UPDATE FILE

<?xml version="1.0"?>
<update>
 <currentVersion>1.0.1</currentVersion>
 <previousVersion>1.0.0</previousVersion>
 <platform>windows</platform>
 <source>http://update/url</source>

 <application-main-binary>
  <name>$APP_FILENAME</name>
  <size></size>
  <hash></hash>
  <permissions></permissions>
 </application-main-binary>

 <install>
  <file>
   <name>$APP_FILENAME</name>
   <hash>$UPDATED_APP_HASH</hash>
   <size>$UPDATED_APP_SIZE</size>
   <permissions>0755</permissions>
  </file>
  <file>
   <name>$UPDATER_FILENAME</name>
   <hash>$UPDATER_HASH</hash>
   <size>$UPDATER_SIZE</size>
   <permissions>0755</permissions>
  </file>
 </install>
 <uninstall>
  <!-- TODO - List some files to uninstall here !-->
  <file>file-to-uninstall.txt</file>
 </uninstall>
</update>