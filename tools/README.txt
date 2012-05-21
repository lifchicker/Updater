Tool for creation update.xml file.
Usage:
    ./create-update-file.py diff|full

diff - for generation update.xml for update
full - for generation update.xml with all files in current version

================================================================================
                    HOW TO USE

1. Change vars in create-update-file.py to your:
    APP_FILE_PATH         - main binary file name
    CURRENT_VERSION_PATH  - full path to your current version
    PREVIOUS_VERSION_PATH - full path to your previous version
    CURRENT_VERSION       - current version
    PREVIOUS_VERSION      - previous version
    PLATFORM              - platform string
    SOURCE                - url for update download page

2. Run create-update-file.py script to create update.xml

3. Upload files to your update server
